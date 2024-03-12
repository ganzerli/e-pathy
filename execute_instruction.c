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
    u32 data_where[WHERE_COUNT] ,data_what[WHAT_COUNT];
    // DATA WHERE O IS ALWAYS ROOT
    // ADDING A 0 TO data_where[1] fo further checks


    // FILLING ARRAY FOR PATH WHERE_COUNT
    for(u32 i = 0; i < WHERE_COUNT; i++){
        data_where[i] = query[DATA_BEGIN+i];
        printf("\ndata_where[%u] %0x", i ,data_where[i]);
    }
    // FILLING ARRAY WHAT_DWITH DATA 
    // last as END_SKELETON
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
            // to initialize a node, in this architecture, [][][*]
            //                                                  |
            //                                                  ---> [DATA-(branch_name)][node][node][node]
            // the first node in the pointed location, will be a DATA
            for(u32 i = 0; i < WHAT_COUNT; i++){
                epathy_format(&data_what[i], DATA_SKELETON);
            }
            // !! FOLLOW_PATH() DOES START FROM 0 , FOLLOWS FROM THERE, 0 WILL ALWAYS BE, 0 = ROOT
            if(data_where[0] == ROOT){
                for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path(ROOT , NODE_SKELETON);// add empty node
                
                for(u32 i = 0; i< WHAT_COUNT ; i++) {
                    // needed in this case, is: creatin an array every time, and give it WITH an added END_SKELETON,
                    // in this case , is added just 1 data, to init a brunc, thic can be done more than 1 time, exactly in this case is: WHAT_COUNT times
                    //
                    // [data or node] [END_SKELETON]-_-'-_~"--> [branch begin n]
                    // [data or node] [END_SKELETON]-_-'-_~"--> [other branch - begin or even not]
                    //                                    
                    //
                    u32 new_name_for_new_branch[2] = { data_what[i] , END_SKELETON };
                    init_node(ROOT,new_name_for_new_branch);     // initializing new empty node    

                }     

                // RESPONSE
                response_size = raw_path_from(ROOT) * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
                break;
            }

            u32 path_begin = follow_path( data_where , WHERE_COUNT);
            for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path(path_begin , NODE_SKELETON);
            for(u32 i = 0; i< WHAT_COUNT ; i++) init_node(path_begin, &data_what[i]);

            count = raw_path_from(path_begin);
            // RESPONSE
            response_size = count * sizeof(u32);
            //
            format_response(buffer , path_buffer , response_size);
            break;
        
        case ADD_TO_EXISTING_PATH: 
        // if node or data
        // 
        break;
        
        case DIAPLAY_PATH:
            // // data_where = PATH from ROOT, ROOT MUST NOT be INCLUDED
            // printf("\n display path..");
            // path_begin = follow_path(data_where,WHERE_COUNT);                 // data_where 0 is always set to 0 from relacy
            // printf("\npath_begin found: %u", path_begin);

            // // check name of nodes in path_begin
            // count = raw_path_from(path_begin);
            // path_begin = firsts_in_path(count);
            // if(!path_begin){
            //     printf(" - > not any node in this path");
            //     char path_empty[] = "No nodes in this path";
            //     response_size = sizeof(path_empty);
            //     format_response(buffer , path_empty , response_size);
            // }else{
            //     response_size = count * sizeof(u32);
            //     format_response(buffer , path_buffer , response_size);
            // }

            // if display root
            if(data_where[0] == ROOT){
                // load all [begin][][][] [end] in path_buffer
                count = raw_path_from(ROOT);
                // check path_buffer [*][*][*]_find_first_32bit_data->[begin]<-[][][][][end] of every node in this layer
                firsts_in_path(count);
                // LOG
                printf("DISPLAY ROOT\npath buffer");
                for(u32 i =0 ; i < count ; i++)printf("\npath_buffer[%u] -> %u" , i , path_buffer[i]);
                // FOMRAT DATA AND RESPONSE
                response_size = count*sizeof(u32);
                format_response(buffer,path_buffer,response_size);
                break;
            }
            printf("following path");
            path_begin = follow_path( data_where , WHERE_COUNT );
            count = raw_path_from(path_begin);
            firsts_in_path(count);

            //LOG
            printf("DISPLAY ROOT\npath buffer");
            for(u32 i =0 ; i < count ; i++)printf("\npath_buffer[%u] -> %u" , i , path_buffer[i]);
            // FORMAT DATA AND RESPONSE
            response_size = count*sizeof(u32);
            format_response(buffer,path_buffer,count*sizeof(u32));
     

            //path_begin = follow_path(data_where,WHERE_COUNT);


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
