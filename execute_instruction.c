/*      INSTRUCTION SET E-PATHY

0 = NEW NODE
1 = ADD TO PATH
2 = GET PATH

*/

// getting instruction from relacy, execute it and send result
unsigned int execute_instruction(char* buffer , unsigned int size){

    unsigned int response_size = 0;
    u32 *query = (u32*)buffer;
    char *resbf = buffer;

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
            resbf = (char*)path_buffer;
            printf("response size:%u", response_size);
        break;

        default:
        //str_cpy( resbf, "DEFAULT!!");
        //response_size = str_len(resbf);
    }

    for(u32 i = 0; i < response_size; i++){
        buffer[i] = resbf[i];
        printf("that -> %0x", buffer[i]);
    }

    return response_size;
}
