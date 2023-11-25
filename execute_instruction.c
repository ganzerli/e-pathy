/*      INSTRUCTION SET E-PATHY

0 = NEW NODE
1 = ADD TO PATH
2 = GET PATH

*/
void format_response(char*bf_out, u32* bf_in , u32 count){
    char* res_b_in = (char*)bf_in;
    for(u32 i = 0; i < count;i++){
        bf_out[i] = res_b_in[i];
    }
}

// getting instruction from relacy, execute it and send result
unsigned int execute_instruction(char* buffer , unsigned int size){

    u32 response_size = 0;
    const u32 zise = size / sizeof(u32); 
    u32 *query = (u32*)buffer;
    char *resbf = buffer;

    // see query table.. when it exists
    const u32 INSTRUCTION = query[0];
    const u32 WHERE_COUNT = query[1];                     // COUNT OF NODES TO REACH PATH
    const u32 WHAT_COUNT = query[2];                      // COUNT OF ELEMENTS TO ADD
    const u32 OPTIONS = query[3];
    const u32 PACKAGE_COUNT= query[4];
    // .. data[][][][][][]...
    
    const u32 DATA_BEGIN = 5;
    u32 data_where[WHERE_COUNT] ,data_what[WHAT_COUNT+1];

    // FILLING ARRAY FOR PATH WHERE_COUNT
    for(u32 i = 0; i < WHERE_COUNT; i++){
        data_where[i] = query[DATA_BEGIN+i];
        printf("\ndata_where[%u] %0x", i ,data_where[i]);
    }

    // FILLING ARRAY WHAT_DWITH DATA 
    // last as end skeleton
    data_what[WHAT_COUNT] = END_SKELETON; 
    for(u32 i = 0; i < WHAT_COUNT; i++){
        data_what[i] = query[DATA_BEGIN + WHERE_COUNT + i];
        printf("\ndata_what[%u] %0x", i ,data_what[i]);
    }
    printf("\ndata_what[last] %0x",data_what[WHAT_COUNT]);

    printf("\nINSTRUCTION:%u \nOPTIONS: %u\ncount: %u", INSTRUCTION,OPTIONS,PACKAGE_COUNT );


    
    switch(INSTRUCTION){
        
        case 0:// 0 = ADD NODE
            // path will be always 0 for now , root

            //add_to_path(0 , NODE_SKELETON);
            // init first NODE_SKELETON found at path filebuffer[0]
            init_node(0, data_what );
            response_size = raw_path_from(0) * sizeof(u32);
            format_response(buffer , path_buffer , response_size);
           
        break;
        
       
        case 1: // 1 = ADD TO PATH

        break;

        
        case 2:// 2 = GET PATH
            // find path names
            size = raw_path_from(0);
            first_in_nodes(size);
            response_size = size * sizeof(u32);
            format_response(buffer , path_buffer , response_size);

            // response_size = path(0) * sizeof(u32);
            // buffer = (char*)path_buffer;
            // printf("response size:%u", response_size);
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
