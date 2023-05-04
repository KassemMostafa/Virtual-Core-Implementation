#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>

struct instruction{
    int BCC;
    int IV_Flag;
    int opCode;
    int ope1;
    int ope2;
    int dest;
    int IV;
    int offset; //j'ai remplacé PC par offset pour gérer le BCC, PC est interne à la fonction fetch
};


uint64_t r[16];
int FLAGS[6] = {0,0,0,0,0,0}; // {BEQ, BNE, BLE, BGE, BL, BG}


struct instruction decode(char *buffer) //Prend une instruction non PCC, découpe l'instruction et met chaque demi octet dans la bonne case de la structure instruction et puis appel execute
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

    //

    //Example writing right to left cause most significant is 31 all the way left
    // MOV r1, 2
    //1 : 00000010 => Immediate Value bits 0 to 7 reverse is 01000000 => 0x40
    //2 : 0001 =>  dest bytes 8 to 11 => 1 => 0001 =>   reverse 1000 => 0x8
    //3 : 0000 => ope2 12 to 15 => 0 => 0000 => reverse 0000 => 0x0
    //4 : 0001 => ope1 16 to 19 => 1 => 0001 => reverse 1000 => 0x8
    //5 : 1000 => opcode 20 to 23 => 8 => 1000 => reverse 0001 => 0x1
    //6 : 1 => immediate value flag + 000  bit 24 to 27 => reverse 0001 => 0x1
    //7 : 0000 => bcc bit 28 to 31 => 0000 => 0x0
    // Before reverse : left to right => bit 0 to 31 => 0000 0010 0000 0001 1000 1000 0000
    // After reverse : left to right => bit 31 to 0 => 0000 0001 0001 1000 0000 0100 0000
    // Expected Hex : 0x01180840 => correspond à after reverse :::
                // 0x00 => 0x0 : bcc, 0x1 : IV flag buffer[0]
                //0x18 => 0x1 : opcode, 0x8 : ope1 buffer[1]
                //0x48 => 0x4 : ope2, Ox8 : dest buffer[2]
                //0x00 => IV buffer[3]

    
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[0],buffer[0]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[1],buffer[1]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[2],buffer[2]);
    printf("Value of a: Hex: %X, Decimal: %d \n",buffer[3],buffer[3]);
    //buffer[0] = 0x80; // B offset with positive offset 
    
    if (buffer[0] == 0x00 || buffer[0] == 0x01) //no BCC , if à supprimer
    {
        printf("Buffer 0 => BCC|IV flag: %x|%x \n",buffer[0] & 0x10, buffer[0] & 0x01);
        info.BCC = 0;
        info.IV_Flag = buffer[0] & 0x01;
        printf("Buffer 11 => opcode|ope1: %x, in decimal \n", buffer[1] >> 4 & 0xF);
        printf("Buffer 1 => opcode|ope1: %x|%x \n",buffer[1] >>4 & 0xF, buffer[1] & 0x0F);
        info.opCode = buffer[1] >> 4 & 0xF;
        info.ope1 = buffer[1] & 0x0F;
        printf("OPcode = %x \n",buffer[1] >> 4 & 0xF);
        printf("Buffer 2 => ope2|dest: %x|%x \n",buffer[2] >> 4 & 0xF,buffer[2] & 0x0F);
        info.ope2 = buffer[2] >> 4;
        info.dest = buffer[2] & 0x0F;
        printf("Buffer 3 => IV: %x\n", buffer[3] & 0xFF);
        info.IV = buffer[3] & 0xFF;
        
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
    return info;
}

//checks if there's an overflow in case of ADD with carry
int overflowCheck(uint64_t a, uint64_t b, int error_Flag)  //error_Flag decide whether to throw an error or not
{
    uint8_t msb_1;
    uint8_t msb_2;
    uint16_t sum_msb;

    msb_1 = (a >> 56) & 0xff;
    msb_2 = (b >> 56) & 0xff;
    sum_msb = msb_1 + msb_2;

    if (sum_msb > 0xff) 
    {
        //printf("Ignored Carry on MSB");
        if (error_Flag)
        {
            printf("Error : Overflow Detected");
            exit(EXIT_FAILURE);

        }
        else
            return 1; //1 for overflow detected
    }
    else
        return 0; //no overflow
}

int IVCheck(struct instruction info) //compares IV with operand1 or 2 to know which is a register and which isn't
{

    if (info.IV_Flag)
    {
        if (info.ope1 == info.IV) //IV is first op position
            return 1;
        else
            return 2; //IV is second op position
    }
    else
        return 0;
}

void setFlag(int BEQ,int BNE,int BLE,int BGE,int BL,int BG)
    {
        FLAGS[0] = BEQ;
        FLAGS[1] = BNE;
        FLAGS[2] = BLE;
        FLAGS[3] = BGE;
        FLAGS[4] = BL;
        FLAGS[5] = BG;
        
    }

void checkFlag(uint64_t a, uint64_t b) //used when CMP
{   
    if (a == b)
        setFlag(1,0,1,1,0,0);
    if (a > b)
        setFlag(0,1,0,1,0,1);
    if (a < b)
        setFlag(0,1,1,0,1,0);
        
    
}

void execute(struct instruction info)
{
    int IVPos =  IVCheck(info);//var used to check which operand is the IV, has the same utility as info.immediate_Value_Flag if there's no immediate value



    switch(info.opCode) {
        case 0: //AND
            if (!IVPos)
                r[info.dest] = r[info.ope1] & r[info.ope2];
            else
                r[info.dest] = r[info.dest] & info.IV;
            break;
        case 1: //ORR
            if (!IVPos)
                r[info.dest] = r[info.ope1] | r[info.ope2];
            else
                r[info.dest] = r[info.dest] | info.IV;
            break;
        case 3: //ADD //vide parce que EOR et ADD sont pareil
        case 6: //SUB vide pour les même raisons
        case 2: //EOR 
            if (!IVPos)
                r[info.dest] = r[info.ope1] ^ r[info.ope2];
            else
                r[info.dest] = r[info.dest] ^ info.IV;
            break;
        case 4: //ADC => test with https://onlinehextools.com/add-hex-numbers
            if (!IVPos) {
                if (!overflowCheck(r[info.ope1], r[info.ope2], 1))
                    r[info.dest] = r[info.ope1] + r[info.ope2];
            } else {
                if (IVPos == 1) {
                    if (!overflowCheck(info.IV, r[info.ope2], 1))
                        r[info.dest] = info.IV + r[info.ope2];
                } else {
                    if (!overflowCheck(r[info.ope1], info.IV, 1))
                        r[info.dest] = r[info.ope1] + info.IV;
                }
            }
            break;
        case 5: //CMP
            if (!IVPos)
                checkFlag(r[info.ope1], r[info.ope2]);
            else
                checkFlag(r[info.dest], info.IV);
            break;


        case 8: //MOV
            if (info.IV_Flag)
                r[info.dest] = r[info.ope2];
            else
                r[info.dest] = info.IV;
        case 9: //LSH
            if (info.IV_Flag)
                //r[info.dest] = r[info.ope1] * pow(2, r[info.ope2]);
            break;
        case 10: //RSH
            if (info.IV_Flag);
               r[info.dest] = (int) floor(r[info.ope1] / pow(2, r[info.ope2]));
    }
}

char* fetch(int PC, FILE *ptr) //BCC géré dans fetch with PC, lecture du binaire et gestion du PC selon si l'instruction est un BCC ou pas => SI BCC => calcul du nouveau BC et lecture de la bonne ligne selon l'offset, SI pas de BCC, envoi de toute la ligne à decode
//Info à trouver/calculer dans fetch => BCC, offset, bloc d'instruction (sans decoder) et PC

{
    //TODO => handle multiple instructions
    char *buffer;
    //FILE *ptr;
    char bcc;
    struct instruction info;
    long filelen;
    //printf("ok\n");
    //ptr = fopen("binary.bin","rb");  // r for read, b for binary, open outside function
    //first call : I read 4 bytes, cursor at the end of the 4 bytes.
    fseek(ptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(ptr)/4;             // Get the current byte offset in the file
    printf("file length : %ld \n",filelen); //3 lines was to know the malloc size => //file len = number of bytes, 1 line of code = 4 bytes, line number = (filelen/PC)/4
    
    rewind(ptr); //called only at the beginning 
    buffer = (char *)malloc(4 * sizeof(char));
    while (fread(buffer, 4 , 1, ptr) == 1) //read until there's nothing left to read
    {
        printf("here");
        
        //fclose(ptr);
        for (int i = 0; i < 4; ++i)
            printf("\n%x %d \n", buffer[i], i);
        info = decode(buffer);
        
        bcc = buffer[0] & 0x10;
        printf("%x", bcc);
        if (bcc == 0x0) //no BCC
        {
            //CALL EXECUTE
            printf("NO BCC\n");
             //
            PC++;
        }
        else
        {
        //todo 1- get offset, 2- verify if offset = max length of file or > 0, 3- fseek to the correct offset before calling again
        //can't get offset since compiler doesn't add it to binary correctly, skip for now
        //skip step 1 and assume the value is set:
            if (info.offset + PC < 0 || info.offset + PC > filelen)
            {
                printf("Erreur de Segmentation");
                exit(EXIT_FAILURE);
            }
            else
            {
                fseek(ptr,info.offset,SEEK_CUR);
            }
        }
    }    
    return 0;
}




void main(int argc, char *argv[]) {
    char *buffer;
    FILE *ptr;
    int PC =1;
    int i;
    

    // for (i =0; i<16;i++)
    // {
    //     r[i] = 0x0;
    // }

    // printf("Usage exemple : BIN_NAME <CODE> <STATE> (VERBOSE) \n");
    // ptr = fopen("binary.bin","rb");
    // buffer = fetch(PC, ptr);
    // fclose(ptr);
    // free(buffer);

    r[0] = 0xF000000000000000;
    r[1] = 0x7fffffffffffffff;

    
    r[2] = r[0] + r[1];
    
    printf("sum = 0x%lx\n", r[2]);
    printf("%d",(int)floor(4.9));
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
