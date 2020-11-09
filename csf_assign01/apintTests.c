/*
 * CSF Assignment 1
 * Unit tests for arbitrary-precision integer data type
 * Student: Ariel Bao
 * email: rbao4@jhu.edu
 */


/*
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
    // added
    ApInt *ap1000;
    ApInt *max1_mixed;
    ApInt *hex_small;
    ApInt *test;
    ApInt *long_ones;
    ApInt *all_eight;
    ApInt *very_long;
    ApInt *max_one_more;
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testLshiftN(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
void testLshift(TestObjs *objs);
// new
void testHexToUint64();

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}


	TEST(testCreateFromU64);
	TEST(testHighestBitSet);
	TEST(testLshiftN);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);
    TEST(testLshift);
    // new
    void test();

	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_u64(0UL);
	objs->ap1 = apint_create_from_u64(1UL);
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
    // added cases
    objs->ap1000 = apint_create_from_u64(1000UL);
    objs->max1_mixed = apint_create_from_u64(0xffffffffFFFfffffUL);
    objs->hex_small = apint_create_from_hex("1f");
    objs->test = apint_create_from_hex("7274c0d3f0a9163ad2607c6d00f3a2");
    objs->long_ones = apint_create_from_hex("11111111111111111111111111111111"); //32 one's (2 uint32)
    objs->all_eight = apint_create_from_u64(0x8888888888888888UL);
    objs->very_long = apint_create_from_hex("9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088");
    objs->max_one_more = apint_create_from_hex("10000000000000000");
	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
    //added
    apint_destroy(objs->ap1000);
    apint_destroy(objs->max1_mixed);
    apint_destroy(objs->hex_small);
    apint_destroy(objs->test);
    apint_destroy(objs->long_ones);  
    apint_destroy(objs->all_eight);
    apint_destroy(objs->very_long);
    apint_destroy(objs->max_one_more);


	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
	ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
    // added
	ASSERT(1000UL == apint_get_bits(objs->ap1000, 0));
	ASSERT(0x8888888888888888UL == apint_get_bits(objs->all_eight, 0));
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
    // added cases
	ASSERT(4 == apint_highest_bit_set(objs->hex_small));
	ASSERT(9 == apint_highest_bit_set(objs->ap1000));
	ASSERT(63 == apint_highest_bit_set(objs->max1_mixed)); 
 	ASSERT(118 == apint_highest_bit_set(objs->test)); // 7274c0d3f0a9163ad2607c6d00f3a2
	ASSERT(124 == apint_highest_bit_set(objs->long_ones)); //11111111111111111111111111111111
	ASSERT(63 == apint_highest_bit_set(objs->all_eight));
	ASSERT(279 == apint_highest_bit_set(objs->very_long)); // 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088
}

void testLshiftN(TestObjs *objs) {
	ApInt *result;
	result = apint_lshift_n(objs->ap0, 17);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

    // new
	result = apint_lshift_n(objs->ap0, 0);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap1, 17);
	ASSERT(0x20000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift_n(objs->ap110660361, 17);
	ASSERT(0xD3116120000UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

    result = apint_create_from_hex("7274c0d3f0a9163ad2607c6d00f3a2");
    ASSERT(0x3ad2607c6d00f3a2UL == apint_get_bits(result, 0));
    ASSERT(0x7274c0d3f0a916UL == apint_get_bits(result, 1));
    ASSERT(0UL == apint_get_bits(result, 2));
	apint_destroy(result);

    // new 
    result = apint_lshift_n(objs->hex_small, 2);
	ASSERT(0x7cUL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);
    
    result = apint_lshift_n(objs->ap1, 64);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(1UL == apint_get_bits(result, 1));
	apint_destroy(result);

    result = apint_lshift_n(objs->ap0, 64);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

    // shift 64 bits to create one more uint64
    result = apint_lshift_n(objs->max1_mixed, 64);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0xffffffffFFFfffffUL == apint_get_bits(result, 1));
	apint_destroy(result);

    // test +1 uint64_t case
    result = apint_lshift_n(objs->max1_mixed, 2);
	ASSERT(0xfffffffffffffffcUL == apint_get_bits(result, 0));
	ASSERT(3UL == apint_get_bits(result, 1));
	apint_destroy(result);

    result = apint_lshift_n(objs->all_eight, 7);
    ASSERT(0x44 == apint_get_bits(result, 1));
	apint_destroy(result);

    // shifts > 64 bits
    result = apint_lshift_n(objs->very_long, 150);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	ASSERT(0x7f1d878022000000UL == apint_get_bits(result, 2));
	ASSERT(0x299c55f7b5725b88UL == apint_get_bits(result, 3));
	ASSERT(0xe0c653b28dec73cdUL == apint_get_bits(result, 4));
	ASSERT(0x645edc6c5999f89aUL == apint_get_bits(result, 5));
	ASSERT(0x269852a0a94bUL == apint_get_bits(result, 6));
	apint_destroy(result);
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);
    /* 0 < 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088*/
	ASSERT(apint_compare(objs->ap0, objs->very_long) < 0);
    /* 7274c0d3f0a9163ad2607c6d00f3a2 < 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088 */
	ASSERT(apint_compare(objs->test, objs->very_long) < 0);
    /* 0xFFFFFFFFFFFFFFFFUL = 0xffffffffFFFfffffUL */
	ASSERT(apint_compare(objs->max1, objs->max1_mixed) == 0);
    /* 7274c0d3f0a9163ad2607c6d00f3a2 <  11111111111111111111111111111111 */
	ASSERT(apint_compare(objs->test, objs->long_ones) < 0);
}

void testFormatAsHex(TestObjs *objs) {
	char *s;

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);

    ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1_mixed))));
	free(s);

    // new
    ASSERT(0 == strcmp("7274c0d3f0a9163ad2607c6d00f3a2", (s = apint_format_as_hex(objs->test))));
	free(s);

    ASSERT(0 == strcmp("11111111111111111111111111111111", (s = apint_format_as_hex(objs->long_ones))));
	free(s);

    ASSERT(0 == strcmp("8888888888888888", (s = apint_format_as_hex(objs->all_eight))));
	free(s);

    ASSERT(0 == strcmp("9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088", (s = apint_format_as_hex(objs->very_long))));
	free(s);

    ASSERT(0 == strcmp("3e8", (s = apint_format_as_hex(objs->ap1000))));
	free(s);
}

void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
    sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
    
    // new
    /* d5bb8eec4805bda8fef9412a376b0b3fbb076e6adc392de5f627021f7b00358f + ea8df69c564f7e6789a757c336096cd45a91469ad146ff9ddb32c09743be5400 = 1c04985889e553c1088a098ed6d7478141598b505ad802d83d159c2b6bebe898f*/
    ApInt *a, *b;
    a = apint_create_from_hex("d5bb8eec4805bda8fef9412a376b0b3fbb076e6adc392de5f627021f7b00358f");
    b = apint_create_from_hex("ea8df69c564f7e6789a757c336096cd45a91469ad146ff9ddb32c09743be5400");
    sum = apint_add(a, b);
    ASSERT(0 == strcmp("1c04985889e553c1088a098ed6d7478141598b505ad802d83d159c2b6bebe898f",
		(s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	apint_destroy(a);
	apint_destroy(b);
    free(s);

    /* 8c63b4fe252fef66bd9247fbdaad7a2bd3920ee7 + d5bb8eec4805bda8fef9412a376b0b3fbb076e6adc392de5f627021f7b00358f = d5bb8eec4805bda8fef9412ac3cec03de0375dd199cb75e1d0d47c4b4e924476 */
    a = apint_create_from_hex("d5bb8eec4805bda8fef9412a376b0b3fbb076e6adc392de5f627021f7b00358f");
    b = apint_create_from_hex("8c63b4fe252fef66bd9247fbdaad7a2bd3920ee7");
    sum = apint_add(a, b);
    ASSERT(0 == strcmp("d5bb8eec4805bda8fef9412ac3cec03de0375dd199cb75e1d0d47c4b4e924476",
		(s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	apint_destroy(b);
	apint_destroy(a);
    free(s);

    /*9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088 + 1 = 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0089*/
    sum = apint_add(objs->very_long, objs->ap1);
	ASSERT(0 == strcmp("9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0089", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

    /* ffffffffffffffff + ffffffffffffffff = 1fffffffffffffffe */
    sum = apint_add(objs->max1, objs->max1_mixed);
	ASSERT(0 == strcmp("1fffffffffffffffe", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
}

void testSub(TestObjs *objs) {
	ApInt *a, *b, *diff;
	char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 0 can't be represented because it is negative */
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));

    // new
	/* test involving larger values */
	a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	diff = apint_sub(a, b);
	ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
		(s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	apint_destroy(b);
	apint_destroy(a);
	free(s);

	/* subtracting 0 from ffffffffffffffff is ffffffffffffffff */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0088 is 9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0087 */
	diff = apint_sub(objs->very_long, objs->ap1);
	ASSERT(0 == strcmp("9a614a82a52d917b71b16667e26b83194eca37b1cf34a67157ded5c96e21fc761e0087", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

	/* subtracting test from very_long is 9a614a82a52d917b71b16667e26b83194eca37b15cbfe59d6735bf8e9bc180091d0ce6 */
	diff = apint_sub(objs->very_long, objs->test);
	ASSERT(0 == strcmp("9a614a82a52d917b71b16667e26b83194eca37b15cbfe59d6735bf8e9bc180091d0ce6", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);

/**
	diff = apint_sub(objs->max_one_more, objs->ap1);
    ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	free(s);
*/

	/* subtracting very_long from test is NULL */
	diff = apint_sub(objs->test, objs->very_long);
	ASSERT(NULL == apint_sub(objs->ap0, objs->ap1));
}

// new tests
void testLshift(TestObjs *objs) {
	ApInt *result;
	result = apint_lshift(objs->ap0);
	ASSERT(0UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->hex_small);
	ASSERT(0x3eUL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->ap1);
	ASSERT(2UL == apint_get_bits(result, 0));
	ASSERT(0UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->max1);
	ASSERT(0xfffffffffffffffeUL == apint_get_bits(result, 0));
	ASSERT(1UL == apint_get_bits(result, 1));
	apint_destroy(result);

	result = apint_lshift(objs->test);
	ASSERT(0x75a4c0f8da01e744UL == apint_get_bits(result, 0));
	ASSERT(0xe4e981a7e1522cUL == apint_get_bits(result, 1));
	apint_destroy(result);
}

