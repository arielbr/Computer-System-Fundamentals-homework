// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
  //FILE *fp1;
  //FILE *fp2;
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void testWithFiles(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);
  //TEST(testWithFiles);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  /*for (int i = 0; i < 16; i++) {
    buf[i] = '\0';
  }*/
  hex_format_offset(1L, buf);
  ASSERT(0 == strcmp(buf, "00000001"));
    //added
  hex_format_offset(2L, buf);
  ASSERT(0 == strcmp(buf, "00000002"));
  hex_format_offset(15L, buf);
  ASSERT(0 == strcmp(buf, "0000000f"));
  hex_format_offset(16L, buf);
  ASSERT(0 == strcmp(buf, "00000010"));
  hex_format_offset(268435456L, buf);
  ASSERT(0 == strcmp(buf, "10000000"));
  hex_format_offset(4294967295L, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
  hex_format_offset(2790387671L, buf);
  ASSERT(0 == strcmp(buf, "a651efd7"));
  hex_format_offset(1247278768L, buf);
  ASSERT(0 == strcmp(buf, "4a57f6b0"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  //for (int i = 0; i < strlen(buf); i++) {
  ASSERT(0 == strcmp(buf, "48"));
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f"));
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72"));
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c"));
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64"));
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));
  hex_format_byte_as_hex(objs->test_data_1[13], buf);
  ASSERT(0 == strcmp(buf, "0a"));
  hex_format_byte_as_hex((char) 0x80, buf);
  ASSERT(0 == strcmp(buf, "80"));
  hex_format_byte_as_hex((char) 0, buf);
  ASSERT(0 == strcmp(buf, "00"));
  hex_format_byte_as_hex((char) 1, buf);
  ASSERT(0 == strcmp(buf, "01"));
  hex_format_byte_as_hex((char) 15, buf);
  ASSERT(0 == strcmp(buf, "0f"));
  hex_format_byte_as_hex((char) 0xEE, buf);
  printf("%s", buf);
  ASSERT(0 == strcmp(buf, "ee")); 
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('e' == hex_to_printable(objs->test_data_1[1]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[2]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[3]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[4]));
  ASSERT(',' == hex_to_printable(objs->test_data_1[5]));
  ASSERT(' ' == hex_to_printable(objs->test_data_1[6]));
  ASSERT('w' == hex_to_printable(objs->test_data_1[7]));
  ASSERT('o' == hex_to_printable(objs->test_data_1[8]));
  ASSERT('r' == hex_to_printable(objs->test_data_1[9]));
  ASSERT('l' == hex_to_printable(objs->test_data_1[10]));
  ASSERT('d' == hex_to_printable(objs->test_data_1[11]));
  ASSERT('!' == hex_to_printable(objs->test_data_1[12]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}

//added 
/**
int compare_file(FILE *fp1, FILE *fp2) {
    char ch1 = getc(fp1);
    char ch2 = getc(fp2);
    while (ch1 != EOF && ch2 != EOF) {
        if (ch1 == EOF || ch1 == EOF) {
            return 1; //not equal
        }
        if (ch1 != ch2){
            return 1;
        }
        ch1 = getc(fp1);
        ch2 = getc(fp2);
    }
    return 0;
}

void testWithFiles(TestObjs *objs) {
    int diff = 0;
    objs->fp1 = fopen("test1.txt", "r");
    objs->fp2 = fopen("test1_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    objs->fp1 = fopen("test2.txt", "r");
    objs->fp2 = fopen("test2_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    objs->fp1 = fopen("test3.txt", "r");
    objs->fp2 = fopen("test3_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    objs->fp1 = fopen("test4.txt", "r");
    objs->fp2 = fopen("test4_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    objs->fp1 = fopen("test5.txt", "r");
    objs->fp2 = fopen("test5_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    objs->fp1 = fopen("test6.txt", "r");
    objs->fp2 = fopen("test6_actual.txt", "r");
    diff = compare_file(objs->fp1, objs->fp2);
    ASSERT(diff == 0);

    fclose(objs->fp1);
    fclose(objs->fp2);
}*/
