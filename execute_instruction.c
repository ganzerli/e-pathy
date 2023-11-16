/*      INSTRUCTION SET E-PATHY

0 = NEW NODE
1 = ADD TO PATH
2 = GET PATH

*/

// getting instruction from relacy, execute it and send result
unsigned int execute_instruction(char* buffer , unsigned int size){

    u32 response_size = 0;
    const u32 zise = size / sizeof(u32); 
    u32 *query = (u32*)buffer;
    char *resbf = buffer;

    // see query table.. when it exists
    u32 instruction = query[0];
    u32 where = query[1];
    u32 count = query[2];
    u32 options = query[3];
    // .. data[][][][][][]...

    //get starting point of all data
    

    printf("\ninstruction: %u\nwhere: %u\ncount: %u\noptions: %u", instruction, where, count, options);
    
    printf("\nDATA\n");

    for(u32 i = 4; i < zise; i++){
        printf("\ndata[%u] %0x", i ,query[i]);
    }


    switch(instruction){
        
        case 0:// 0 = NEW NODE
            add_to_path(where , NODE_SKELETON);
        break;
        
       
        case 1: // 1 = ADD TO PATH

        break;

        
        case 2:// 2 = GET PATH
            response_size = path(where) * sizeof(u32);
            buffer = (char*)path_buffer;
            printf("response size:%u", response_size);
        break;

        
        case 3:// 3 = DELETE
        break;

        default:
        buffer[0] = 'E';
        buffer[1] = 'R';
        buffer[2] = 'R';
        buffer[3] = 'O';
        buffer[4] = 'R';

        response_size = 5;
    }

    return response_size;
}
