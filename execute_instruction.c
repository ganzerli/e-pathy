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

        case ADD_NODE:// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
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
                    // [data or node] [END_SKELETON]-_-'-_~"--> [branch begin n]
                    // [data or node] [END_SKELETON]-_-'-_~"--> [other branch - begin or even not]
                    u32 new_name_for_new_branch[2] = { data_what[i] , END_SKELETON };
                    init_node(ROOT,new_name_for_new_branch);     // initializing new empty node    

                }     
                // RESPONSE
                response_size = raw_path_from(ROOT) * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
                break;
            }

            //  IF TO ADD NOT IN ROOT , FOLLOW PATH FUNCTION WORKS
            //  or needed is always to specify the path from root already from the other program sending TCP
            //  much prettyer for hypster and people with university payed from the parents not adding always root..
            //  the intellectual guy can tell to girls that he is sending data to : 
            //  client/items_bought/book_123/page_1/possible_secret_references/client/cart 
            //  instead of: ROOT/client/items_bought/book_123/page_1/possible_secret_references/client/cart
            //  exactly.. e-pathy can have loops.. not jet but is easy.. you already know what im thinking.. but, and hippy what can also have???
            u32 path_begin = follow_path( data_where , WHERE_COUNT);

            for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path(path_begin , NODE_SKELETON);
            // giving a -name- to the new path, and --> ADD end_skeleton <--
            for(u32 i = 0; i< WHAT_COUNT ; i++) {
                u32 new_name_for_new_branch[2] = { data_what[i] , END_SKELETON };
                init_node( path_begin , new_name_for_new_branch );     // initializing new empty node    
            }  
            // RESPONSE
            count = raw_path_from(path_begin);
            format_response(buffer , path_buffer , count * sizeof(u32));
            break;
        
        case DIAPLAY_PATH:// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
            if(data_where[0] == ROOT){
                count = raw_path_from(ROOT);
                // check path_buffer [*][*][*]_find_first_32bit_data->[begin]<-[][][][][end] of every node in this layer
                firsts_in_path(count);
                // FOMRAT DATA AND RESPONSE
                response_size = count*sizeof(u32);
                format_response(buffer,path_buffer,response_size);
                break;
            }

            path_begin = follow_path( data_where , WHERE_COUNT );
            count = raw_path_from(path_begin);
            count = firsts_in_path(count);
            // response
            response_size = count*sizeof(u32);                  // needed for network stuff, see return statement
            format_response(buffer,path_buffer,response_size);

        break;

        
        case DELETE:
        break;

        default:
            printf("\nREQUEST ERROR: OPTION %u NOT AVAILABLE" , INSTRUCTION);
            char OPT_NOT_EXISTING[] = "nREQUEST ERROR: OPTION NOT AVAILABLE";
            format_response(buffer , OPT_NOT_EXISTING , sizeof(OPT_NOT_EXISTING));
        break;
    }

    return response_size;
}
