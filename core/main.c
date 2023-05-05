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
int OVERFLOW = 0;
int OVERFLOWLSH = 0;

struct instruction initInfo()
{
    struct instruction info;
    info.BCC = 0;
    info.dest = 0;
    info.IV = 0;
    info.IV_Flag = 0;
    info.offset = 0;
    info.opCode = 0;
    info.ope1 = 0;
    info.ope2 = 0;
    return info;
}
struct instruction decode(uint8_t *buffer) //Prend une instruction non PCC, découpe l'instruction et met chaque demi octet dans la bonne case de la structure instruction et puis appel execute
{
    struct instruction info = initInfo();
    uint32_t tempOffset = 0;
    int i;
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

    
    printf("Value of a: Hex: 0x%X, Decimal: %d \n",buffer[0],buffer[0]);
    printf("Value of a: Hex: 0x%X, Decimal: %d \n",buffer[1],buffer[1]);
    printf("Value of a: Hex: 0x%X, Decimal: %d \n",buffer[2],buffer[2]);
    printf("Value of a: Hex: 0x%X, Decimal: %d \n",buffer[3],buffer[3]);
    //buffer[0] = 0x80; // B offset with positive offset 
    //MOV r1, 2 => buffer[0] = 0x08
    if (buffer[0] == 0x00 || buffer[0] == 0x08) //no BCC , if à supprimer
    {
        printf("Buffer 0 => BCC|IV flag: %x|%x \n",buffer[0] & 0x10, buffer[0] & 0x08);
        info.BCC = 0; //redundant
        info.IV_Flag = buffer[0] & 0x08;
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
        printf("Buffer0? %x\n", buffer[0]);
        info.BCC = buffer[0] >> 4 & 0xF;
        printf("buffer[0] = 0x%x\n", buffer[0]);
        printf("BCC is %d\n",info.BCC);
        info.offset = 1;
        if ((buffer[0] & 0xF) == 8)
        {
            info.offset = -1;
            printf("negative offset here\n");
        }
            
            
        tempOffset |= buffer[0] & 0b00000111;
        for (i = 1; i < 4; i++ )
        {
            tempOffset <<=8;
            tempOffset |= buffer[i];
        }
        printf("offset without sign is %d \n", tempOffset);
        info.offset = tempOffset * info.offset;
        printf("offset is %d\n",info.offset);
        info.offset *= 4; //pour avoir la valeur en nombre d'instructions
        printf("offset is in bytes : %d\n",info.offset);
        
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
void overflowCheck(uint64_t a, uint64_t b)  //error_Flag decide whether to throw an error or not
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
        OVERFLOW = 1; //1 for overflow detected
        
    }
    else
    {
         OVERFLOW = 0; //no overflow
         
    }   
}

void overflowlshcheck(uint64_t a, uint64_t b)
{
    int i = 0;
    uint64_t c = (a << b);
    uint64_t d = (c >> b);
   if ( d != a )
   {
       while(a != 0)
       {
           a = a >> 1;
           i+=1 ;
       }

   }
   OVERFLOWLSH = i;
   //printf("%" PRIu64 "\n", c);
   //printf("%" PRIu64 "\n", d);
   //printf("%d\n", OVERFLOWLSH);
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

    if (a > b)
        setFlag(0,1,0,1,0,1);
    if (a < b)
        setFlag(0,1,1,0,1,0);
    if (a == b)
        setFlag(1,0,1,1,0,0);        
}

int checkBCC(struct instruction info)
{
    switch(info.BCC)
    {
        case 8: //B
            return 1;
            break;
        default:
            return FLAGS[info.BCC-8];
    }
}

void execute(struct instruction info)
{
    int IVPos =  IVCheck(info);//var used to check which operand is the IV, has the same utility as info.immediate_Value_Flag if there's no immediate value


    printf("info.opcode == %d\n", info.opCode);
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
            if (!IVPos)
            {
                if (OVERFLOW)
                {
                    if (r[info.ope1] >= r[info.ope2])
                        r[info.ope2] += 1;
                    else
                        r[info.ope1] += 1;
                }  
                overflowCheck(r[info.ope1],r[info.ope2]);
                r[info.dest] = r[info.ope1] + r[info.ope2];
            }
            else
            {
                if (IVPos == 1)
                {
                    if (OVERFLOW)
                        info.IV = info.IV +1;
                    overflowCheck(info.IV,r[info.ope2]);
                    r[info.dest] = info.IV + r[info.ope2];
                }
                else
                {
                    if (OVERFLOW)
                        info.IV += 1;
                    overflowCheck(r[info.ope1],info.IV);
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
            printf("MOV instruction = 0x%lx\n",r[info.dest]);
            break;
        case 9: //LSH
            if (OVERFLOWLSH == 0)
            {
                if (info.IV_Flag)
                {
                    r[info.dest] = r[info.ope1] * pow(2, r[info.ope2]);
                }

                else
                {
                    r[info.dest] = r[info.ope1] * pow(2, r[info.IV]);
                }

            }
            else
            {

                if (info.IV_Flag)
                {
                    int over = 63-r[info.ope2];
                    r[15] = r[info.ope1] >> over;
                    r[info.dest] = r[info.ope1] * pow(2, r[info.ope2]);
                }

                else
                {
                    int over = 63-r[info.IV];
                    r[15] = r[info.ope1] >> over;
                    r[info.dest] = r[info.ope1] * pow(2, r[info.IV]);
                }

            }


            break;
        case 10: //RSH
             if (info.IV_Flag)
                r[info.dest] = (int) floor(r[info.ope1] / pow(2, r[info.ope2]));
             else
                 r[info.dest] = (int) floor(r[info.ope1] / pow(2, r[info.IV]));

            break;
    }
}

uint8_t* fetch(FILE *ptr) //BCC géré dans fetch with PC, lecture du binaire et gestion du PC selon si l'instruction est un BCC ou pas => SI BCC => calcul du nouveau BC et lecture de la bonne ligne selon l'offset, SI pas de BCC, envoi de toute la ligne à decode
//Info à trouver/calculer dans fetch => BCC, offset, bloc d'instruction (sans decoder) et PC

{
    size_t buffer_size = 4;
    uint8_t *buffer = malloc(buffer_size);
    size_t numBytes = 0;
    int PC = 0;
    char bcc;
    struct instruction info;
    long filelen;
    //first call : I read 4 bytes, cursor at the end of the 4 bytes.
    fseek(ptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(ptr);             // Get the current byte offset in the file
    printf("file length : %ld \n",filelen); //3 lines was to know the malloc size => //file len = number of bytes, 1 line of code = 4 bytes, line number = (filelen/PC)/4
    
    rewind(ptr); //called only at the beginning 
    buffer = malloc(buffer_size);
    while ((numBytes = fread(buffer, sizeof(uint8_t), buffer_size, ptr)) == buffer_size)
    {
        printf("PC after reading but before increment by = %d\n\n", PC);
        printf("FILE LEN == %ld \n\n",filelen);
        for (int i = 0; i < 4; ++i)
            printf("%x %d \n", buffer[i], i);
        info = decode(buffer);
        PC += 4;
        // execute(info);
        // bcc = buffer[0] & 0x10;
        // printf("%x", bcc);
        if (info.BCC == 0) //no BCC
        {
            printf("NO BCC\n");
            printf("File len == %ld\n", filelen);
            printf("PC after execute at the end of the current line  %d\n\n",PC);
            execute(info);
        }
        else
        {
        //todo 1- get offset, 2- verify if offset = max length of file or > 0, 3- fseek to the correct offset before calling again
        //can't get offset since compiler doesn't add it to binary correctly, skip for now
        //skip step 1 and assume the value is set:
            printf("BCC\n");
            if (checkBCC(info))
            {
                if (info.offset + PC - 4 < 0 || info.offset + PC > filelen)
                {

                //PC = 8 => ligne one is read and ligne 2 BCC, if offset = - 1 => -4 , fseek(PC+offset - 4)
                //File len = 16, PC = 12 offset = 1 => 4
                
                    printf("File len == %ld\n", filelen);
                    printf("offset is %d\n", info.offset);
                    printf("PC is in bytes %d\n",PC);
                    printf("Erreur de Segmentation\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    printf("File len == %ld\n", filelen);
                    printf("offset is %d\n", info.offset);
                    printf("PC is in bytes %d\n",PC);
                    fseek(ptr,info.offset - 4,SEEK_CUR);
                    PC += info.offset - 4;
                }
            }
            
        }

    }    
    printf("PC outside = %d\n", PC);
    return buffer;
}




void main(int argc, char *argv[]) {
    uint8_t *buffer;
    FILE *ptr;
    int i;
    struct instruction info;
    

    // for (i =0; i<16;i++)
    // {
    //     r[i] = 0x0;
    // }
    uint64_t a = pow(2,50);
    printf("%" PRIu64 "\n", a);
    uint64_t b = 14;

    overflowlshcheck(a,b);
    printf("Usage exemple : BIN_NAME <CODE> <STATE> (VERBOSE) \n");
    ptr = fopen("binary.bin","rb");
    buffer = fetch(ptr);
    fclose(ptr);
    free(buffer);

    // r[0] = 0x24152dfb45da45df;
    // r[1] = 0xa521147fde45f45a;
    // r[2] = 0x45dcea451f2d45a4;
    // r[3] = 0xf5554ed4f4522365;

    
    
    // printf("sum = 0x%lx\n", r[2]);
    // printf("%d",(int)floor(4.9));
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

    ///// EXO2 TEST 

                // if (OVERFLOW)
                // {
                //     if (r[1] >= r[3])
                //         r[3] += 1;
                //     else
                //         r[1] += 1;
                // }  
                // overflowCheck(r[1],r[3]);
                // r[4] = r[1] + r[3];

                //    if (OVERFLOW)
                // {
                //     if (r[0] >= r[2])
                //         r[2] += 1;
                //     else
                //         r[0] += 1;
                // }  
                // overflowCheck(r[0],r[2]);
                // r[5] = r[0] + r[2];
                 printf("sum = 0x%lx%lx\n", r[5],r[4]);

                


}
