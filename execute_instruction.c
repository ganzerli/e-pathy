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
    const u32 INSTRUCTION = query[0];
    const u32 WHERE = query[1];                     // COUNT OF NODES TO REACH PATH
    const u32 WHAT = query[2];                      // COUNT OF ELEMENTS TO ADD
    const u32 OPTIONS = query[3];
    const u32 COUNT = query[4];
    // .. data[][][][][][]...
    
    const u32 DATA_BEGIN = 5;
    u32 data_where[WHERE] ,data_what[WHAT+1];

    // FILLING ARRAY FOR PATH WHERE
    for(u32 i = 0; i < WHERE; i++){
        data_where[i] = query[DATA_BEGIN+i];
        printf("\ndata_where[%u] %0x", i ,data_where[i]);
    }

    // FILLING ARRAY WHAT WITH DATA 
    for(u32 i = 0; i < WHAT; i++){
        data_what[i] = query[DATA_BEGIN + WHERE + i];
        printf("\ndata_what[%u] %0x", i ,data_what[i]);
    }

    printf("\nINSTRUCTION:%u \nOPTIONS: %u\ncount: %u", INSTRUCTION,OPTIONS,COUNT );
    
    switch(INSTRUCTION){
        
        case 0:// 0 = ADD NODE

            // just adding to root
            // add_to_path(WHERE , NODE_SKELETON);
            // init_node(WHERE, &data[WHERE] );
        
           
        break;
        
       
        case 1: // 1 = ADD TO PATH

        break;

        
        case 2:// 2 = GET PATH
            response_size = path(WHERE) * sizeof(u32);
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
