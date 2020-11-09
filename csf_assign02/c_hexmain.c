/*
 * C implementation of hexdump main function
 * CSF Assignment 2
 * Ariel Bao
 * rbao4@jhu.edu
 */

#include "hexfuncs.h"  // this is the only header file which may be included!

/**
  * This is the main function of C implementation of hexdump.
  * Return - set to 0 if correctly executed
  */
int main(void) {
    // count for the first /n so start reading from the next line
    // char c = getchar();
    // save the next <= 16 chars read
    char data_buf[17];
    for (int i = 0; i < 17; i++) {
        data_buf[i] = '0';
    }
    // char position
    long offset = 0;
    // string for char position
    char posbuf[9];
    for (int i = 0; i < 9; i++) {
        posbuf[i] = '0';
    }
    // for hex string for each char (0-255)
    char hexbuf[3];
        for (int i = 0; i < 3; i++) {
        hexbuf[i] = '0';
    }
    // the total number of chars read
    int char_num = hex_read(data_buf);
    // space count (for line alignment)
    int space_count = 49;
    // read until end of stdin
    while (char_num > 0) {
        // print position
        hex_format_offset(offset, posbuf);
        hex_write_string(posbuf);
        hex_write_string(": ");
        // output 2-digit hex + space for all
        for (int i = 0; i < char_num; i++) {
            hex_format_byte_as_hex(data_buf[i], hexbuf);
            hex_write_string(hexbuf);
            hex_write_string(" ");   
            space_count -= 3;
        }
        // output the remaining spaces
        while (space_count > 0) {
            hex_write_string(" ");
            space_count--;
        }
        for (int i = 0; i < char_num; i++) {
            char temp[2];
            temp[0] = (char) hex_to_printable(data_buf[i]);
            temp[1] = '\0';
            hex_write_string(temp);
        }
        hex_write_string("\n");     
        space_count = 49; //reset
        offset += char_num;
        char_num = hex_read(data_buf);
    }
    return 0;
}
