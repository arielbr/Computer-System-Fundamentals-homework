/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Student: Ariel Bao
 * email: rbao4@jhu.edu
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include "apint.h"


/**
  * This function creates an ApInt ptr from a uint64_t object.
  *
  * Parameter: the uint74_t value whose value is used to create a
  *            new ApInt object.
  * Return: a pointer to the new ApInt object created.
  */
ApInt *apint_create_from_u64(uint64_t val) {
    ApInt *apint = (ApInt *)malloc(sizeof(ApInt));
    apint->size = (size_t)1;
    apint->arr_long = (uint64_t *)malloc(sizeof(uint64_t));
    *(apint->arr_long) = val;
    return apint;
}

/**
  * Helper func: change 16 hex numbers into a uint64_t.
  * 
  * Parameter: a pointer to the front of a 16-hex16 char array to be 
  *            converted to a uint64_t value in this function
  * 
  * Return: a uint64_t value created from the conversion
  */
uint64_t hex_to_uint64 (char hex) {
    char values[] = "0123456789abcdef";
    char *ptr = strchr(values, tolower(hex));
    int index = ptr - values;
    ptr = NULL;
    free(ptr);
    return (uint64_t) index;
}


/**
  * This function creates an ApInt ptr from a hexademical object.
  * Note on sequence: read in groups of 16, if has remainder that is treated 
  * as a single group from the highest/first few chars in hex. More significant
  * numbers (first few of hex) are stored in the higher indices of the apint arr.
  * 
  * Parameter: a pointer to the front of a hex-char array to be
  *            converted.
  * 
  * Return: an ApInt pointer pointing to the ApInt object created
  *         which has the same value as the passed in char array.
  */
ApInt *apint_create_from_hex(const char *hex) {
    size_t num_char = (size_t) strlen(hex);
    size_t num_group = 0;
    num_group = num_char % 16 == 0 ? num_char / 16 : num_char / 16 + 1;

    ApInt *apint = (ApInt *)malloc(sizeof(ApInt));
    apint->size = num_group;
    apint->arr_long = (uint64_t *)malloc(sizeof(uint64_t) * num_group);

    // this tracks which uint64_t array in the arr_long we are adding to
    uint64_t temp = 0;
    size_t i = 0;
    for (i = 0; i < num_char; i++) {
        // ith least sig number
        char ch = hex[num_char - 1 - i];
        temp += hex_to_uint64(ch) << ((i % 16) * 4);
        // last hex for conversion of the curr uint64_t, increment after
        if (i % 16 == 15) {
            apint->arr_long[(i / 16)] = temp;
            temp = 0;
            // reached most sig number and hex len is multiple of 16
            if (i == num_char - 1) {
                return apint;
            }
        }
    }
    
    apint->arr_long[num_group - 1] = temp;
    return apint;
}

/**
  * This functions deallocates the memory of arr_long in an ApInt
  * object and the ApInt pointer itself.*curr_64_ptr
  * 
  * Parameter: an ApInt pointer pointing to the obj to be destroyed.
  */
void apint_destroy(ApInt *ap) {
    // free the array of uint64_first
    free(ap->arr_long);
    // free ap itself
    free(ap);
}


/**
  * This function returns the nth uint64_t in ap.
  * 
  * Parameter: 
  *       ap - a pointer to the ApInt obj whicg we'll get bits from
  *       n - the nth smallest 64 bits to return (eg n = 0 means
  *           to return bits 0~63)
  * 
  * Return: an uint64_t value representing the values of the 64
  *         returned bits.
  */
uint64_t apint_get_bits(ApInt *ap, unsigned n) {
    if (n >= ap->size) {
        return 0UL;   
    }
    return *(ap->arr_long + n);
}

/**
  * Returns the position of the most sig bit set to 1 in the
  * representation of ap.
  * 
  * Parameter: a pointer to the ApInt object to operate on.
  * 
  * Return: the position of the most significant bit set to 1.
  *         returns 0 if value of the passed in ApInt obj is 0.
  */
int apint_highest_bit_set(ApInt *ap) {
    // ap has value 0
    if (ap->size == 1 && *(ap->arr_long) == (uint64_t) 0) {
        return -1;
    }
    // check the actual size without leading zeros
    int s = ap->size - 1;
    while (s >= 0) {
        if (ap->arr_long[s] > (uint64_t) 0) {
            break;
        }
        s--;
    }
    if (s < 0) {
        return -1;
    }

    // this bit_num is relative to the rightmost bit in most_sig
    int bit_num = -1;
    uint64_t *curr = ap->arr_long + s;
    uint64_t most_sig = *curr;
    while (most_sig != 0) {
        most_sig = most_sig >> 1;
        bit_num++;
    }    
    bit_num += 64 * s;
    return bit_num;
}

/**
  * Returns a pointer to an ApInt instance formed by shifting each
  * bit of *ap one position to the left.
  * 
  * Parameter: a pointer to an ApInt object to operates shifts on.
  * 
  * Return: a pointer to an ApInt whose value is resulted from
  * shifting ap in parameter 1 bit to the left.
  */
ApInt *apint_lshift(ApInt *ap) {
    // used to account if each uint64_t number's shift overflows into the one before it.
    bool overflow_last = false;
    bool overflow_curr = false;
    uint64_t *ptr = ap->arr_long;
    int highest_bit = apint_highest_bit_set(ap);
    if (highest_bit < 0) {
        highest_bit = 0;
    }
    ApInt *new_ap = (ApInt *)malloc(sizeof(ApInt));
    new_ap->size = (uint64_t) (highest_bit + 1) < (uint64_t) ap->size * 64 ? ap->size : ap->size + 1;

    new_ap->arr_long = (uint64_t *)malloc(sizeof(uint64_t) * new_ap->size);
    uint64_t *ptr_new_ap = new_ap->arr_long;

    // track each uint64_t in arrays in the given ap
    size_t i = 0;
    // least sig uint64_t to most sig uint64_t
    for (i = 0; i < ap->size; i++) {
        overflow_last = overflow_curr;
        ptr_new_ap[i] = overflow_last ? ptr[i] + 1 : ptr[i];
        // is the first bit of curr 1
        overflow_curr = ptr_new_ap[i] >= ((uint64_t)1 << 63) ? true : false;
        ptr_new_ap[i] = ptr_new_ap[i] << 1;
    }
    if (new_ap->size > ap->size) {
        new_ap->arr_long[new_ap->size - 1] = 1UL;
    }
    // delete memory
    //free(ptr);
    return new_ap;
}

/**
  * Returns a pointer to an ApInt instance formed by shifting each
  * bit of *ap n position to the left.
  * 
  * Parameters:
  *       ap - a pointer to an ApInt object to operate shifts on.
  *       n - the number of left bits to shift by.
  * 
  * Return: a pointer to a new ApInt object whose value is resulted
  *         from n bits shifts to the left from the ApInt object
  *         pointed to by parameter ap.
  */
ApInt *apint_lshift_n(ApInt *ap, unsigned n) {
    // calulate total number of uint64_t 0's that follow
    size_t group_zeros = n / 64;

    int remaining_shifts = n % 64;
    int highest_bit = apint_highest_bit_set(ap);
    
    size_t new_size = ((highest_bit >= 0) && (highest_bit) % 64 + remaining_shifts >= 64) ? ap->size + group_zeros + 1 : ap->size + group_zeros;

    ApInt *new_ap = (ApInt *)malloc(sizeof(ApInt));
    new_ap->size = new_size;
    new_ap->arr_long = (uint64_t *)malloc(sizeof(int64_t) * new_size);
    uint64_t temp = 0;

    // trailing zeros
    for (size_t i = 0; i < group_zeros; i++) {
        new_ap->arr_long[i] = 0UL; 
    }

    // non zero part
    for (size_t i = 0; i < ap->size; i++) {
        uint64_t cur = ap->arr_long[i];
        new_ap->arr_long[i + group_zeros] = (cur << remaining_shifts) + temp;
        temp = ap->arr_long[i] >> (64 - remaining_shifts);   
    }

    if (highest_bit >= 0 && (highest_bit) % 64 + remaining_shifts >= 64) {
        new_ap->arr_long[new_ap->size - 1] = temp;
    }

    return new_ap;
}

/**
  * Helper func: this function convers a 8-bit number to a char.
  * 
  * Parameter: the uint8_t number to convert.
  * 
  * Return: the char converted to.
  */
char byte_to_char(uint8_t c) {
    char ch_arr[16] = "0123456789abcdef";
    return ch_arr[c];
}


/**
  * Convert an ApInt to a hex number.
  * 
  * Parameter: a pointer to the ApInt object to convert to hex.
  * 
  * Return: a pointer to the front of the char array storing
  *         hex numbers converted into.
  */
char *apint_format_as_hex(ApInt *ap) {
    // call highest bit sets
    int bit_count = apint_highest_bit_set(ap) + 1;
    // check special case zero
    if (bit_count == 0) {
        char *hex = (char *)malloc(sizeof(char) * 2);
        hex[0] = '0';
        hex[1] = '\0';
        return hex;    
    }

    // number of chars, 4 multiple or remaining ones
    size_t char_count = bit_count % 4 == 0 ? bit_count >> 2 : (bit_count >> 2) + 1;
    char *all_chars = (char *) malloc (sizeof(char) * (char_count + 1)); 
    for (size_t c = 0; c < char_count; c++) {
        int uint_index = c / 16;
        int index_in_uint = c % 16;
        *(all_chars + char_count - 1 - c) = byte_to_char((*(ap->arr_long + uint_index) >> (4 * index_in_uint)) % 16);
    }
    *(all_chars + char_count) = '\0';
    return all_chars;
}

/**
  * Computes the sum of a and b, two ApInts into another ApInt.
  * 
  * Parameters:
  *       a - pointer to the first ApInt to be added.
  *       b - pointer to the second ApInt to be added.
  * 
  * Return: a pointer to another ApInt object which has the
  *         value of summing a and b's ApInte values.
  */
ApInt *apint_add(const ApInt *a, const ApInt *b) {
    ApInt *new_ap = (ApInt *)malloc(sizeof(ApInt));
    // track the overflow boolean of last addition an current addition
    bool overflowed_last = false;
    bool overflowed_curr = false;
    uint64_t curr_sum;
    new_ap->size = (a->size >= b->size) ? a->size : b->size;
    new_ap->arr_long = (uint64_t *) malloc(sizeof(uint64_t) * new_ap->size);
    for (size_t i = 0; i < new_ap->size; i++) {
        overflowed_last = overflowed_curr;

        if (i < a->size && i < b->size) {
            curr_sum = a->arr_long[i] + b->arr_long[i];
            curr_sum = overflowed_last ? curr_sum + 1 : curr_sum;
            overflowed_curr = (curr_sum < a->arr_long[i] || curr_sum < b->arr_long[i]) ? true : false;
            new_ap->arr_long[i] = curr_sum;
            overflowed_last = false;
        } else if (i < a->size) {
            curr_sum = overflowed_last ? a->arr_long[i] + 1 : a->arr_long[i];
            overflowed_curr = a->arr_long[i] == ~0UL && overflowed_last? true : false;
            overflowed_last = false;
            new_ap->arr_long[i] = curr_sum;            
        } else if (i < b->size) {
            curr_sum = overflowed_last ? b->arr_long[i] + 1 : b->arr_long[i];
            overflowed_curr = b->arr_long[i] == ~0UL && overflowed_last? true : false;
            overflowed_last = false;
            new_ap->arr_long[i] = curr_sum;       
        }
        if (i == new_ap->size - 1) {
            break;
        }
    }
    
    // if there will be one extra bit overflow
    if (overflowed_curr) {
        ApInt* larger = (ApInt *)malloc(sizeof(ApInt));
        larger->size = new_ap->size + 1;
        larger->arr_long = (uint64_t *)malloc(sizeof(uint64_t) * (new_ap->size + 1));
        *larger->arr_long = (uint64_t) 1;
        for (size_t i = 0; i < new_ap->size; i++) {
            larger->arr_long[i] = new_ap->arr_long[i];
        }
        *(larger->arr_long + larger->size - 1) = (uint64_t) 1;
        free(new_ap->arr_long);
        new_ap->arr_long = NULL;
        free(new_ap);
        new_ap = NULL;
        return larger;
    }
    return new_ap;
}

/**
  * Subtract the second value from the first.
  * 
  * Parameters:
  *        a - a pointer to the ApInt obj being subtracted from.
  *        b - a pointer to the ApInt obj to subtract.
  * 
  * Return: a pointer to another ApInt whose value equals the
  *         differences of ApInts pointed to by a and b. Returns
  *         0 if a < b.
  */
ApInt *apint_sub(const ApInt *a, const ApInt *b) {
    // returns null if second is larger
    if (apint_compare(a, b) < 0) {
        return NULL;
    }
    ApInt *new_ap = (ApInt *)malloc(sizeof(ApInt));
    new_ap->arr_long = (uint64_t *)malloc(sizeof(uint64_t) * a->size);
    uint64_t *curr_new = new_ap->arr_long;
    new_ap->size = a->size;
    // deep copy a into new_ap
    for (size_t i = 0; i < a->size; i++) {
        *(new_ap->arr_long + i) = *(a->arr_long + i);
    }
    // subtract zero
    if (b->size == 1 && *b->arr_long == (uint64_t) 0) { 
        return new_ap;
    }
    bool borrow_digit = false;
    // subtract b from new_ap
    for (size_t i = 0; i < a->size; i++) {
        curr_new[i] = borrow_digit ? curr_new[i] - 1 : curr_new[i];
        borrow_digit = false;
        if (i >= b->size) {
            // negative overflow
            borrow_digit = a->arr_long[i] == 0UL ? true : false;
        } else {
            // negative overflow
            borrow_digit = new_ap->arr_long[i] < b->arr_long[i] ? true : false;
            curr_new[i] -= b->arr_long[i];
            }
        }        
    return new_ap;
}

/**
  * Compares the value of two ApInt. 
  * 
  * Parameters:
  *     left - pointer to the first ApInt to be compared.
  *     right - pointer to the second ApInt to be compared.
  * 
  * Return: an integer, negative if first smaller, positive 
  *         if right is smaller, 0 if equal.
  */
int apint_compare(const ApInt *left, const ApInt *right) {
    size_t size_a = left->size;
    size_t size_b = right->size;
    // left smaller
    if (size_a < size_b) {
        return -1;
    } else if (size_a > size_b) {
    // left larger
        return 1;
    }
    // same number of uint64_t, compare one by one from most sig until difference
    uint64_t *curr_a = left->arr_long + size_a - 1;
    uint64_t *curr_b = right->arr_long + size_b - 1;
    for (size_t i = 0; i < size_a; i++) {
        if (*curr_a < *curr_b) {
            return -1;
        } else if (*curr_a > *curr_b) {
            return 1;
        }
        curr_a--;
        curr_b--;
    }
    return 0;
}
