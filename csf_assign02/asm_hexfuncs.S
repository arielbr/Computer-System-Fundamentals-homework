/*
 * Assembly language implementation of hexdump functions 
 * CSF Assignment 2
 * Ariel Bao
 * rbao4@jhu.edu
 */

.section .rodata

    hex: .string "0123456789abcdef"

.section .bss
        char_buf: .space 2

.globl hex_to_printable
.globl hex_format_byte_as_hex
.globl hex_format_offset
.globl hex_read
.globl hex_write_string

.section .text

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
    hex_to_printable:
        subq $8, %rsp
        pushq %r12
        pushq %r13
        movq $0x20, %r12 /* space */
        cmpq %r12, %rdi
        jl .LreplaceDot
        movq $0x7E, %r13  /* tilda */
        /* if out of range, is not printable, go to print '.' */
        cmpq %r13, %rdi
        jg .LreplaceDot
        /* if reached here, is pritable */
        movq %rdi, %rax
        popq %r13
        popq %r12
        addq $8, %rsp
        ret

    /**
      * Add a '.' to the buffer if the char is not printable
      *
      */
    .LreplaceDot:
        movq $46, %rax /* not printable, return '.'*/
        popq %r13
        popq %r12
        addq $8, %rsp
        ret

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
    hex_format_byte_as_hex:
        subq $8, %rsp
        pushq %r12
        movb %dil, %r12b
        /* get the four rightmost bits from %r10 */
        andb $0xF, %r12b
        /* right shift to get the higher bits */
        shrb $4, %dil
        andb $0xF, %dil
        /* remainder is the ones digit */
        cmpb $10, %r12b
        jl .Lonesdigit_number
        jmp .Lonesdigit_letter 

    /**
      * Add the one's digit number to the buffer if a digit.
      *
      */
    .Lonesdigit_number:
        /* save the corresponding number into the ones digit: (char) 48 + ones_digit*/
        addq $48, %r12
        movb %r12b, 1(%rsi)
        cmpb $10, %dil
        jl .Ltensdigit_number
        jmp .Ltensdigit_letter

    /**
      * Add the one's digit number to the buffer if a letter.
      *
      */
    .Lonesdigit_letter:
        /* save the corresponding letter into the ones digit: (char) 97 + (ones_digit - 10) */
        addq $87, %r12
        movb %r12b, 1(%rsi)
        cmpb $10, %dil
        jl .Ltensdigit_number
        jmp .Ltensdigit_letter

    /**
      * Add the ten's digit number to the buffer if a digit.
      *
      */
    .Ltensdigit_number:
    /* save the corresponding letter into the tens digit: (char) 48 + tens_digit */
        addb $48, %dil
        movb %dil, 0(%rsi)
        /* add nul terminator*/
        movq $0, 2(%rsi)
        popq %r12 
        addq $8, %rsp
        ret

    /**
      * Add the ten's digit number to the buffer if a letter.
      *
      */
    .Ltensdigit_letter:
        /* save the corresponding letter into the tens digit: (char) 97 + (ones_digit - 10) */
        addb $87, %dil
        movb %dil, 0(%rsi)
        /* add nul terminator*/
        movq $0, 2(%rsi)
        popq %r12 
        /*move back the rsp position*/
        addq $8, %rsp
        ret

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
    hex_format_offset:
        subq $8, %rsp
        pushq %r12
        pushq %r13
        pushq %r14
        pushq %r15
        /* %rdi is for long offset */
        /* %r12 for count of next char */
        movq $7, %r12
        /* long offset */
        movq %rdi, %r13
        jmp .LgetNextChar
        /* %rsi for char buf of size 9 */

    /**
      * Write the next char of the offset number to the char array buffer
      *
      */
    .LgetNextChar:
        /* if reaching before the front of array, finish*/
        cmpq $0, %r12
        jl .LhexFormatOffsetDone
        movq %r13, %r14
        and $0xF, %r14b
        movq $hex, %r15
        addb %r14b, %r15b
        pushq %r13
        movb (%r15), %r13b
        /* move the last byte to sbuf[i] or address of %rsi + %r12*/
        movb %r13b, (%rsi, %r12)
        popq %r13

        shrq $4, %r13
        /* decrement to the next char of the array*/

        subq $1, %r12
        jmp .LgetNextChar

    /**
      * Finish and return for the offset format function.
      *
      */
    .LhexFormatOffsetDone:
        /* end char string with nul terminator*/
        movb $0, 8(%rsi)
        popq %r15
        popq %r14
        popq %r13
        popq %r12
        addq $8, %rsp
        ret

    /**
      * Read up to 16 bytes from standard input into data_buf.
      *
      * Parameter - the char array where the read chars will be stored
      *
      * Returns - the number of characters read.
      */
    hex_read:
        subq $8, %rsp
        pushq %rdi
        pushq %r12
        movq %rdi, %r12

        /* read 16 chars */
        movq $0, %rax /* read */
        movq $0, %rdi /* stdin */
        movq %r12, %rsi /* buf */
        movq $16, %rdx /* count */
        syscall

        popq %r12
        popq %rdi
        addq $8, %rsp
        ret

    /**
      * Write given nul-terminated string to standard output.
      *
      * Parameter - the string which will be written into 2-digit hex
      *             numbers char by char
      *
      */
    hex_write_string:
        subq $8, %rsp
        pushq %r12
        pushq %r13
        pushq %r14
        pushq %r15
        movq $0, %r12
        movq %rdi, %r14
        jmp .LwriteChar

    /**
      * Write char by char to stdout.
      *
      */
    .LwriteChar:
        cmpb $0, (%r14, %r12)
        je .LwriteFinish
        movq $0, %r13
        movq $char_buf, %r15
        movb (%r14, %r12), %r13b
        movb %r13b, (%r15)
        movb $0, 1(%r15)
        /*write char to stdout*/
        movq $1, %rdi /* stdout */
        movq $char_buf, %rsi /* buf */
        movq $1, %rdx /* count */
        movq $1, %rax /* write */
        syscall
        
        addq $1, %r12
        jmp .LwriteChar

    /**
      * Finish the function to write a char array to stdout.
      *
      */
    .LwriteFinish:
        popq %r15
        popq %r14
        popq %r13
        popq %r12
        addq $8, %rsp
        ret

