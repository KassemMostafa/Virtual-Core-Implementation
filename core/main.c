#include <stdio.h>


void decode()
{
    int line = 0x00184800;
    int line2 = 0xffffff;
    int line3 = 0x1000000;
    printf("Value of a: Hex: %X, Decimal: %d \n",line,line);
    printf("Value of a: Hex: %X, Decimal: %d \n",line2,line2);
    printf("Value of a: Hex: %X, Decimal: %d \n",line3,line3);
    //All cases : read bit 31 to 0 == left to right Big Endian
    // Case 1 : Not BCC 
    //bit 31 to 28 => bcc => check 2.5.2 for values => 1 nibble => 4 bits
    //bit 27 to  25 => always to 0 => + bit 24  == 1 nibble => reading bits 24 to 27 for 1 nibble
    //bit 24  => Immediate value flag => Immediate Value flag
    //bit 23 to 20 => opcode => 1 nibble => check table 2.5.4 for values
    //bit 19 to 16 => ope1 => 1 nibble
    //bit 15 to 12 => ope2 => 1 nibble
    //bit 11 to 8 => dest => 1 nibble
    //bit 7 to 0 => Immediate Value => 1 byte

    

    //Example writing right to left cause most significant is 31 all the way left
    // MOV r1, r2
    //1 : 00000000 => Immediate Value bits 0 to 7 reverse is same to have 7 to 0
    //2 : 0001 =>  dest bytes 8 to 11 => 1 => 0001 =>   reverse 1000
    //3 : 0010 => ope2 12 to 15 => 2 => 0010 => reverse 0100
    //4 : 0001 => ope1 16 to 19 => 1 => 0001 => reverse 1000
    //5 : 1000 => opcode 20 to 23 => 8 => 1000 => reverse 0001
    //6 : 0 => immediate value flag + 000  bit 24 to 27 => reverse 0000
    //7 : 0000 => bcc bit 28 to 31 => 0000
    // Before reverse : left to right => bit 0 to 31 => 0000 0000 0001 0010 0001 1000 0000 0000
    // After reverse : left to right => bit 31 to 0 => 0000 0000 0001 1000 0100 1000 0000 0000
    // Expected Hex : 0x00184800 => correspond à after reverse
    // Expected Binary Big Endian: 0000 0000 0001 1000 0100 1000 0000 0000

    // Hex Converted the same way => What changes is binary input little vs big


    


    // Case 2 :  BCC => bit 0 to 26 => offset => offset and bit 27 => positive (0) or negative (1) => total : 28 bits => 7 bytes  
    
}

int main() {
    printf("Hello, World!\n");
    decode();
    return 0;
}
