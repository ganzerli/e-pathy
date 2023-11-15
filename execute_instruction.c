/*      INSTRUCTION SET E-PATHY

0 = NEW NODE
1 = ADD TO PATH
2 = GET PATH

*/

// getting instruction from relacy, execute it and send result
unsigned int execute_instruction(char* buffer , unsigned int size){

    unsigned int response_size = 0;

    u32 *query = (u32*)buffer;
    u32 *resbf = (u32*)buffer;

    // see query table.. when it exists
    u32 instruction = query[0];
    u32 where = query[1];
    u32 what = query[2];
    u32 options = query[3];

    printf("\n%u\n%u\n%u\n%u", instruction, where, what, options);

    switch(instruction){
        // 0 = NEW NODE
        case 0:
            
        break;
        
        // 1 = ADD TO PATH
        case 1:
        break;

        // 2 = GET PATH
        case 2:
            response_size = path(where) * sizeof(u32);
            buffer = path_buffer;
            printf("response size:%u", response_size);
        break;

        default:
        //str_cpy( resbf, "DEFAULT!!");
        //response_size = str_len(resbf);
    }
    
    printf("\nno errors!");
    
    //buffer = resbf;

    for(u32 i = 0; i < response_size; i++){
        printf("\n that -> %0x", resbf[i] );
        printf("\n that bf -> %0x\n", buffer[i] );

    }




    return response_size;
}
