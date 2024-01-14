//      INSTRUCTION SET E-PATHY

#define DISPLAY_ROOT 0
#define ADD_NODE 1
#define ADD_TO_EXISTING_PATH 2
#define DIAPLAY_PATH 3
#define DELETE 4
#define ROOT 0


void format_response(char*bf_out, void* bf_in , u32 count){
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
    const u32 PACKAGE_COUNT = query[4];
    // .. data[][][][][][]...
    
    const u32 DATA_BEGIN = 5;
    u32 data_where[WHERE_COUNT] ,data_what[WHAT_COUNT+1];
    // DATA WHERE O IS ALWAYS ROOT
    // ADDING A 0 TO data_where[1] fo further checks


    // FILLING ARRAY FOR PATH WHERE_COUNT
    for(u32 i = 0; i < WHERE_COUNT; i++){
        data_where[i] = query[DATA_BEGIN+i];
        printf("\ndata_where[%u] %0x", i ,data_where[i]);
    }
    // FILLING ARRAY WHAT_DWITH DATA 
    // last as END_SKELETON
    data_what[WHAT_COUNT] = END_SKELETON; 
    u32 pure_to_format = 0;
    for(u32 i = 0; i < WHAT_COUNT; i++){
        data_what[i] = query[DATA_BEGIN + WHERE_COUNT + i]; 
        printf("\ndata_what[%u] %0x", i ,data_what[i]);
    }
    printf("\ndata_what[last] %0x",data_what[WHAT_COUNT]);
    printf("\nINSTRUCTION:%u \nOPTIONS: %u\ncount: %u", INSTRUCTION,OPTIONS,PACKAGE_COUNT );

    u32 count = 0;
    //  path_begin = index in filebuffer[ ]
    u32 path_begin = 0;    

    switch(INSTRUCTION){

        case DISPLAY_ROOT: 
            count = raw_path_from(ROOT);
            firsts_in_path(count);
            // RESPONSE
            response_size = count * sizeof(u32);
            format_response(buffer , path_buffer , response_size);
            break;

        case ADD_NODE:
            // adding node is expected only one value in data_what
            // the only value in data_what should be a -DATA- , <--> NOT a NODE
            if(!epathy_format(&data_what[0], DATA_SKELETON)){
            //  F O R M A T    E R R O R
                printf("BAD FORMAT: formatting data");
                char BAD_FORMAT[] = "BAD FORMAT: formatting data";
                response_size = sizeof(BAD_FORMAT);
                format_response(buffer , BAD_FORMAT , response_size);
                break;
            }
            if(data_where[0] == ROOT){
                for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path(ROOT , NODE_SKELETON);// add empty node
                for(u32 i = 0; i< WHAT_COUNT ; i++) init_node(ROOT, &data_what[i]);// initializing new empty node                             
                // RESPONSE
                response_size = raw_path_from(ROOT) * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
                break;
            }
            // data_where = PATH from ROOT + 1 layer, ROOT MUST NOT be INCLUDED
            u32 path_begin = follow_path( data_where , WHERE_COUNT);
            for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path(path_begin , NODE_SKELETON);
            for(u32 i = 0; i< WHAT_COUNT ; i++) init_node(path_begin, &data_what[i]);

            count = raw_path_from(path_begin);
            // RESPONSE
            response_size = count * sizeof(u32);
            format_response(buffer , path_buffer , response_size);
            break;
        
        case ADD_TO_EXISTING_PATH: 
        // if node or data
        // 
        break;
        
        case DIAPLAY_PATH:
            // data_where = PATH from ROOT, ROOT MUST NOT be INCLUDED
            printf("\n display path..");
            path_begin = follow_path(data_where,WHERE_COUNT);                 // data_where 0 is always set to 0 from relacy
            printf("\npath_begin found: %u", path_begin);

            // check name of nodes in path_begin
            count = raw_path_from(path_begin);
            path_begin = firsts_in_path(count);
            if(!path_begin){
                printf(" - > not any node in this path");
                char path_empty[] = "No nodes in this path";
                response_size = sizeof(path_empty);
                format_response(buffer , path_empty , response_size);
            }else{
                response_size = count * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
            }
        break;

        
        case DELETE:
        break;

        default:
            printf("\nREQUEST ERROR: OPTION %u NOT AVAILABLE" , INSTRUCTION);
            char OPT_NOT_EXISTING[] = "nREQUEST ERROR: OPTION NOT AVAILABLE";
            response_size = sizeof(OPT_NOT_EXISTING);
            format_response(buffer , OPT_NOT_EXISTING , response_size);
        break;
    }

    return response_size;
}
