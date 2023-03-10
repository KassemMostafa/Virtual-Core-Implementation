#include <stdio.h>
#include <stdlib.h>

struct instruction{
    int r[16];
    int immediate_Value;
    int immediate_Value_Flag;
    int BCC;
    int opCode;
    int First_op;
    int Second_op;
    int destination_Register;
    int PC;
};


char* fetch(int PC) //BCC géré dans fetch with PC, lecture du binaire et gestion du PC selon si l'instruction est un BCC ou pas => SI BCC => calcul du nouveau BC et lecture de la bonne ligne selon l'offset, SI pas de BCC, envoi de toute la ligne à decode
//Info à trouver/calculer dans fetch => BCC, offset, bloc d'instruction (sans decoder) et PC

{
    char *buffer;
    FILE *ptr;
    long filelen;
    printf("ok");
    ptr = fopen("binary.bin","rb");  // r for read, b for binary


    fseek(ptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(ptr);             // Get the current byte offset in the file
    rewind(ptr);
    buffer = (char *)malloc(filelen * sizeof(char));
    fread(buffer, filelen, 1, ptr);
    fclose(ptr);
    for (int i = 0; i < filelen; ++i) {
        printf("\n%x %d \n", buffer[i], i);
    }
    return buffer;
}

void decode(char *buffer) //Prend une instruction non PCC, découpe l'instruction et met chaque demi octet dans la bonne case de la structure instruction et puis appel execute
{
    struct instruction info;
    // int line = 0x00184800;
    // int line2 = 0xffffff;
    // int line3 = 0x1000000;
    // printf("Value of a: Hex: %X, Decimal: %d \n",line,line);
    // printf("Value of a: Hex: %X, Decimal: %d \n",line2,line2);
    // printf("Value of a: Hex: %X, Decimal: %d \n",line3,line3);
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
    //1 : 00000000 => Immediate Value bits 0 to 7 reverse is same to have 7 to 0 => 0x00
    //2 : 0001 =>  dest bytes 8 to 11 => 1 => 0001 =>   reverse 1000 => 0x8
    //3 : 0010 => ope2 12 to 15 => 2 => 0010 => reverse 0100 => 0x4
    //4 : 0001 => ope1 16 to 19 => 1 => 0001 => reverse 1000 => 0x8
    //5 : 1000 => opcode 20 to 23 => 8 => 1000 => reverse 0001 => 0x1
    //6 : 0 => immediate value flag + 000  bit 24 to 27 => reverse 0000 => 0x0
    //7 : 0000 => bcc bit 28 to 31 => 0000 => 0x0
    // Before reverse : left to right => bit 0 to 31 => 0000 0000 0001 0010 0001 1000 0000 0000
    // After reverse : left to right => bit 31 to 0 => 0000 0000 0001 1000 0100 1000 0000 0000
    // Expected Hex : 0x00184800 => correspond à after reverse :::
                // 0x00 => 0x0 : bcc, 0x0 : IV flag buffer[0]
                //0x18 => 0x1 : opcode, 0x8 : ope1 buffer[1]
                //0x48 => 0x4 : ope2, Ox8 : dest buffer[2]
                //0x00 => IV buffer[3]
    
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[0],buffer[0]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[1],buffer[1]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[2],buffer[2]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[3],buffer[3]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[0],buffer[0]);
    buffer[0] = 0x80; // B offset with positive offset 
    if (buffer[0] == 0x00 || buffer[0] == 0x01) //no BCC , if à supprimer
    {
        printf("Buffer 0 => BCC|IV flag: %x|%x \n",buffer[0] & 0x10, buffer[0] & 0x01);
        info.BCC = buffer[0] & 0x10;
        info.immediate_Value_Flag = buffer[0] & 0x01;
        printf("Buffer 1 => opcode|ope1: %x|%x \n",buffer[1] >>4 & 0xF, buffer[1] & 0x0F);
        info.opCode = buffer[1] >> 4 & 0xF;
        info.First_op = buffer[1] & 0x0F;
        printf("Buffer 2 => ope2|dest: %x|%x \n",buffer[2] >> 4 & 0xF,buffer[2] & 0x0F);
        buffer[3] = 0xFF;
        printf("Buffer 3 => IV: %x\n", buffer[3] & 0xFF);
        
    }
    else
    {
        printf("BCC");
        
    }
    //Rappel bit shifting
    // A = 60 => 0011 1100


    // A << 2 => shift to the left by 2
    // 0011 1100 << 2 => 1111 0000
    // A << 4 => 
    // 0011 1100 << 4 => 1100 0011
    // A >> 2 => shift to the right by 2
    // 0011 1100 >>2 => 0000 1111

    // Case 2 :  BCC => bit 0 to 26 => offset => offset and bit 27 => positive (0) or negative (1) => total : 28 bits => 7 bytes  
    
}


void main(int argc, char *argv[]) {
    char *buffer;
    int PC =0;
    printf("Usage exemple : BIN_NAME <CODE> <STATE> (VERBOSE)");
    buffer = fetch(PC);
    decode(buffer);
    free(buffer);

    /*if (argc == 3){
        int r0 = 0x10;
        printf( "%x \n",r0);
        printf( "%d",r0);
        return 0;

    }
    if (argc == 4){
        int r0 = 0x10;
        printf( "%x \n",r0);
        printf( "%d",r0);
        return 0;

    }
    else{
        printf("Usage exemple : BIN_NAME <CODE> <STATE> (VERBOSE)");
    }*/

}
