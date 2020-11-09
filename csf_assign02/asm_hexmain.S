/*
 * Hexdump main function, assembly language version
 * CSF Assignment 2
 * Ariel Bao
 * rbao4@jhu.edu
 */

.section .rodata
    str1: .string ": "
    str2: .string " "
    newlinestr: .string "\n"

.section .bss
    data_buf: .space 17
    pos_buf:  .space 9
    hex_buf:  .space 3
    printable_char: .space 2

    .section .text

    .globl main

/**
  * This is the main function of x86 implementation of hexdump.
  * Return - set to 0 if correctly executed
  */
main:
    subq $8, %rsp
    /* put nul-termanators */ 
    movq $data_buf, %r8
    movq $16, %r9
    movb $0, (%r8, %r9)

    movq $pos_buf, %r8
    movq $8, %r9
    movb $0, (%r8, %r9)    

    movq $hex_buf, %r8
    movq $2, %r9
    movb $0, (%r8, %r9)

    movq $printable_char, %r8
    movq $1, %r9
    movb $0, (%r8, %r9) 

    movq $0, %r8 /* initialize offset num (long) */
    movq $49, %r9 /* %r9 reserved for num space count in a line */

    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    /* Start reading! */
    jmp .LreadBy16

/**
  * Read up to 16 bytes from standard input into data_buf, and output offset str
  *
  */
.LreadBy16:

    /* Read another 16 chars*/
    movq $data_buf, %rdi
    call hex_read
    /* read 16 chars */

    /* check if there are any chars left */
    cmpq $0, %rax
    jle .Ldone

    movq %rax, %r10 /* reserved for char_num in this read */
    /* get the offset string to posbuf */
    movq %r8, %rdi
    movq $pos_buf, %rsi
    call hex_format_offset
    addq %r10, %r8 /* update new offset */
   
    /* write offset string to stdout */
    movq $pos_buf, %rdi
    call hex_write_string

    /* write ": " string to stdout */
    movq $str1, %rdi
    call hex_write_string

    movq $0, %r13  /* counter for chars to output */
    jmp .LwriteHex

/**
  * Write 2-digit hex numbers of the stdin input
  *
  */
.LwriteHex:
    cmpq %r10, %r13
    jge .LoutputSpace

    pushq %r12
    /* output 2-digit hex string for each char */
    movq $data_buf, %r12
    movb (%r12, %r13), %dil
    popq %r12
    movq $hex_buf, %rsi
    call hex_format_byte_as_hex

    /* write hex string returned to stdout */
    movq $hex_buf, %rdi
    call hex_write_string

    /* write " " string to stdout */   
    movq $str2, %rdi
    call hex_write_string

    addq $1, %r13 /* hex num to read */
    subq $3, %r9 /* num space */
    jmp .LwriteHex

/**
  * Output remaining spaces to make sure the line lengths align 
  *
  */
.LoutputSpace: 
    movq $0, %r14 /* initialize to print the printable chars */
    cmpq $0, %r9
    jle .LoutputPrintable

    /* write " " string to stdout */
    movq $str2, %rdi
    call hex_write_string
    
    subq $1, %r9
    jmp .LoutputSpace

/**
  * Output the converted/printable string of the stdin input.
  *
  */
.LoutputPrintable:
    /* compare remaining # chars to input w total chars in this read r10 - total chars read this round*/
    cmpq %r10, %r14
    jge .Lreset

    /* move data_buf[i] to printable_char */
    pushq %r12
    movq $data_buf, %r12
    movb (%r12, %r14), %dil
    popq %r12

    /* store printable char to data to printable_char[0] */
    call hex_to_printable
    pushq %r13
    movq $printable_char, %r13
    movb %al, (%r13) 

    /* end the string with a nul terminator*/
    movb $0, 1(%r13)
    popq %r13

    /* write printable_char string to stdout */
    movq $printable_char, %rdi
    call hex_write_string

    addq $1, %r14
    jmp .LoutputPrintable

/**
  * Reset values to enter the next while loop.
  *
  */
.Lreset:
    /* write "\n" string to stdout */
    movq $newlinestr, %rdi
    call hex_write_string

    movq $49, %r9 /* reset num space */
    jmp .LreadBy16

/**
  * End the program
  * 
  */
.Ldone:
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    addq $8, %rsp
    ret
