/*
 * C implementation of hexdump functions
 * CSF Assignment 2
 * Ariel Bao
 * rbao4@jhu.edu
 */

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"


/**
  * Read up to 16 bytes from standard input into data_buf.
  *
  * Parameter - the char array where the read chars will be stored
  *
  * Returns - the number of characters read.
  */
long hex_read(char data_buf[]) {
    long total_chars = 0;
    total_chars = read(0, data_buf, 16);
    data_buf[16] = '\0';
    return total_chars;
}

/**
  * Write given nul-terminated string to standard output.
  *
  * Parameter - the string which will be written into 2-digit hex
  *             numbers char by char
  *
  */
void hex_write_string(const char s[]){
    size_t size = 0; // only for write()'s return value
    int length = 0;
    char *ptr = (char *) s;
    while (*ptr != '\0') {
        length++;
        ptr++;
    }
    size = write(1, s, length);
    (void) size;
}


/**
  * Format a long value as an offset string consisting of exactly 8
  * hex digits.  The formatted offset is stored in sbuf, which must
  * have enough room for a string of length 8.
  *
  * Parameters -
  *      offset: the long value that is to be written as a hex offset string
  *      sbuf: the char array where the string representation of the offset
  *            value will be stored
  *
  */
void hex_format_offset(long offset, char sbuf[]) {
    long temp = offset;
    int next = 0;
    int i = 7;
    for (i= 7; i >= 0; i--) {
        next = temp % (1 << 4);
        temp = temp >> 4;
        sbuf[i] = (next < 10) ? (char) 48 + next : (char) 97 + (next - 10);
        // a digit or a letter. ascii 48 = '0', 97 = 'a'
    }
    for (int j = i; j >= 0; j--) {
        sbuf[j] = '0';
    }
    sbuf[8] = '\0';
}

/**
  * Format a byte value (in the range 0-255) as string consisting
  * of two hex digits. The string is stored in sbuf.
  *
  * Parameters -
  *      byteval: the byte number to be converted
  *      sbuf: the char array to store the 2-char string representation 
  *            of the byte
  *
  */
void hex_format_byte_as_hex(long byteval, char sbuf[]) {
    int ones_digit = byteval & 0xF;
    sbuf[1] = (ones_digit < 10) ? (char) (48 + ones_digit) : (char) (97 + (ones_digit - 10));
    int tens_digit = (byteval >> 4) & 0xF;
    sbuf[0] = (tens_digit < 10) ? (char) (48 + tens_digit) : (char) (97 + (tens_digit - 10));
    sbuf[2] = '\0';
}

/**
  * Convert a byte value (in the range 0-255) to a printable character
  * value.  If byteval is already a printable character, it is returned
  * unmodified.  If byteval is not a printable character, then the
  * ASCII code for '.' should be returned.
  *
  * Parameter - the byteval to be printed
  * 
  * Return: the long val of the ascii value of the printable char
  *
  */
long hex_to_printable(long byteval) {
    // not printable
    if (byteval < 0x20 || byteval > 0x7E) {
        return 46L; // 46 is the ASCII code for '.'
    }
    return byteval;
}
