#include <stdio.h>
#include <stdlib.h>

struct instruction{
    int r[16];
    int immediate_Value;
    int BCC;
    int opCode;
    int First_op;
    int Second_op;
    int destination_Register;
    int PC;
};


int fetch(){
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
        printf("%x", buffer[i]);
    }

    return 0;
}
int main(int argc, char *argv[]) {
    printf("Usage exemple : BIN_NAME <CODE> <STATE> (VERBOSE)");
    fetch();

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
