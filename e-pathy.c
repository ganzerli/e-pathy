#include <stdio.h>
#include <stdlib.h>

#define SOMETHING_WENT_WRONG    0xFFFFFFFF  //11111111111111111111111111111111

#define FILENAME "file"

#define NODE_SKELETON           0xC0000000  //11000000000000000000000000000000
#define DATA_SKELETON           0x40000000  //01000000000000000000000000000000
#define END_SKELETON            0x00000000  //00000000000000000000000000000000
#define RAW_DATA                0x80000000  //10000000000000000000000000000000

//  E R R O R S
#define ERROR_1                 ( SOMETHING_WENT_WRONG - 1 )
#define ERROR_2                 ( SOMETHING_WENT_WRONG - 2 )
#define ERROR_3                 ( SOMETHING_WENT_WRONG - 3 )
#define ERROR_4                 ( SOMETHING_WENT_WRONG - 4 )

#define _1GB 1073741823
#define FILESIZE _1GB / 5       // 200 MB

 // 4 bytes integer
typedef u_int32_t  u32;  

// global variables
u32 int32count = 0;
u32 *file_buffer;
 
#include "tools.c"
#include "garbage_collection.c"

// PARTS OF THE PROGRAM
#include "core.c"
#include "instructions.c"
#include "server.c"


////////////////////////////////////////////////////////////////////////////////        MAIN       ////
int main() {

    printf("\n\n    WELCOME TO E-PATHY\n\n");
    printf("    e-pathy is saving integers creating paths, of integers..\n");
    printf("    so if you like to have fun you can have /0/50/0/1 containing 1 , 2 , 3 , 4 , 5 or other integers.\n");
    printf("    Only some people don't have a limit in the definition of what they are doing in the world, they are also many.\n");
    printf("    Anyway, e-pathy does have a limit, e-pathy cannot store integers bigger than: \n");
    printf("    1 Billion \n");
    printf("    073 Millions \n");
    printf("    741 Tousend \n");
    printf("    8 Hundered \n");
    printf("    2 times 10 \n");
    printf("    and 3 Units \n");
    printf("\n\n    That has a reason, and to know it you need to contact the creator of all this and offer a relatable amount of money\n\n\n");

    int32count = epathy_check();
    garbage32count = garbage_check();

    file_buffer = malloc(FILESIZE);

    load_file(file_buffer,FILENAME,int32count);

   //  get parse execute commands, send data
   //  ...

    add_to_path(0 , NODE_SKELETON);
    free(file_buffer);
   
   return EXIT_SUCCESS;
}
