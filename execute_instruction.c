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
        pure_to_format = query[DATA_BEGIN + WHERE_COUNT + i];
        // just for security reasons..
        pure_to_format = trim_first_2_bits(pure_to_format);

        ////////////////////////////////////
        pure_to_format += DATA_SKELETON;
        ////////////////////////////////////
        
        data_what[i] = pure_to_format;
        
        printf("\ndata_what[%u] %0x", i ,data_what[i]);
    }
    printf("\ndata_what[last] %0x",data_what[WHAT_COUNT]);

    printf("\nINSTRUCTION:%u \nOPTIONS: %u\ncount: %u", INSTRUCTION,OPTIONS,PACKAGE_COUNT );

    u32 count = 0;
    //  path_begin = index in filebuffer[ ]
    u32 path_begin = 0;    
    u32 const ROOT = 0;                         
    switch(INSTRUCTION){

        case 0: // display root
           
            // display
            count = raw_path_from(ROOT);
            firsts_in_path(count);
            // RESPONSE
            response_size = count * sizeof(u32);
            format_response(buffer , path_buffer , response_size);
        
        case 1:// 0 = ADD NODE
            // if path is 0 ROOT no need to search path and return 0 if not found
            if(data_where[0] == 0){
                add_to_path(ROOT , NODE_SKELETON);
                init_node(ROOT, data_what );
                // RESPONSE
                response_size = raw_path_from(ROOT) * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
                break;
            }// 1234 2222
            // if path is not ROOT

            //                    first in data_where is always 0 until now
            u32 path_begin = follow_path( data_where , WHERE_COUNT);
            add_to_path(path_begin , NODE_SKELETON);
            init_node(path_begin, data_what );
            count = raw_path_from(path_begin);
            // RESPONSE
            response_size = count * sizeof(u32);
            format_response(buffer , path_buffer , response_size);
           
            break;
        
       
        case 2: // 2 = SOMETHING

        break;

        
        case 3:// 3 = DISPLAY PATH

            // find index in filebuffer
            path_begin = follow_path(data_where,WHERE_COUNT);                 // data_where 0 is always set to 0 from relacy
            printf("\npath_begin found: %u", path_begin);

            // check name of nodes in path_begin
            count = raw_path_from(path_begin);
            path_begin = firsts_in_path(count);
            if(!path_begin){
                printf(" - > not any node in this path");
                char path_empty[] = "No nodes in this path";
                u32 * frm_path_empty = (u32*) path_empty;
                response_size = sizeof(path_empty);
                format_response(buffer , frm_path_empty , response_size);
            }else{
                response_size = count * sizeof(u32);
                format_response(buffer , path_buffer , response_size);
            }


            // response_size = path(0) * sizeof(u32);
            // buffer = (char*)path_buffer;
            // printf("response size:%u", response_size);
        break;

        
        case 4:// 3 = DELETE
        // get to a pathſ
        response_size = PACKAGE_COUNT * sizeof(u32);
        u32 path[2] = { 0x1234 , 0x3214};

        follow_path(path , 2);

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
