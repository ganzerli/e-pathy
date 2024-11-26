//      INSTRUCTION SET E-PATHY

#define DISPLAY_ROOT 0
#define ADD_NODE 1
#define ADD_TO_EXISTING_PATH 2
#define DIAPLAY_PATH 3
#define DELETE 4
#define ROOT 0

// format 32 bits into 8 bit int array
void format_response( char*bf_out, void* bf_in , u32 count ){
    char* res_b_in = (char*) bf_in;                         // cast and reassign the input buffer
    for( u32 i = 0; i < count; i++; ){
        bf_out[i] = res_b_in[i];                            // copy every byte in a response buffer
    }
}

// getting instruction from relacy, execute it and send result
unsigned int execute_instruction( char* buffer , unsigned int size ){

    u32 response_size = 0;                                  // used for response
    u32 *query = (u32*)buffer;                              // i have fancy names, but not a girlfriend

    const u32 INSTRUCTION = query[0];                       // CODE AS 32 BIT INTEGER REPRESENTING A COMMAND
    const u32 WHERE_COUNT = query[1];                       // COUNT OF NODES TO REACH PATH
    const u32 WHAT_COUNT = query[2];                        // COUNT OF ELEMENTS TO ADD
    const u32 OPTIONS = query[3];                           // IF AND WHICH OPTION TO USE (just in case, never needed for now)
    const u32 PACKAGE_COUNT = query[4];                     // SIZE OF THE WHOLE REQUEST IN 32 UINT
    
    // .. data[][][][][][]...
    const u32 DATA_BEGIN = 5;                               // other 0 to 4 is the "request header"
    u32 data_where[WHERE_COUNT] ,data_what[WHAT_COUNT];     // create arrays to contain the data received

    //  ! ! ! ALREADY FORGOT MORE THAN ONCE , MYSELF.
    //  DATA WHERE, DOES NOT BEGIN WITH 0, ROOT IS NOT CONTAINED IN THE PATH
    //  THE PATH WILL NOT BE [0][256][8086]... but [256][8086] , BECAUSE THE FUNCITON FOLLOW PATH() IS EASYER LIKE THAT (and easyer also the query itself, less to write, always)
    //  THERE WILL BE A [0] or in other name [ROOT] , only if the destination of the command is ROOT, first-starting branch.

    // FILLING ARRAY FOR PATH WHERE_COUNT
    for(u32 i = 0; i < WHERE_COUNT; i++){
        data_where[i] = query[DATA_BEGIN+i];                
        printf("\ndata_where[%u] %0x", i ,data_where[i]);
    }

    // FILLING ARRAY WHAT with DATA 
    for(u32 i = 0; i < WHAT_COUNT; i++){
        data_what[i] = query[DATA_BEGIN + WHERE_COUNT + i]; 
        printf("\ndata_what[%u] %0x", i ,data_what[i]);
    }
    printf("\ndata_what[last] %0x",data_what[WHAT_COUNT]);
    printf("\nINSTRUCTION:%u \nOPTIONS: %u\ncount: %u", INSTRUCTION,OPTIONS,PACKAGE_COUNT );

    u32 count = 0 , path_begin = 0;                             //  path_begin = index in filebuffer[ ]    

    switch(INSTRUCTION){
        // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
        case ADD_NODE: // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
            // to initialize a node, in this architecture, [][][*]
            //                                                  |
            //                                                  ---> [DATA-(branch_name)][node][node][node]
            // the first node in the pointed location, will be a DATA, in that way can be a "name" for the entire branch
            for(u32 i = 0; i < WHAT_COUNT; i++){
                epathy_format(&data_what[i], DATA_SKELETON);    //  GIVING THE "TYPE" TO THE DATA TO INSERT 
            }
            // !! FOLLOW_PATH() DOES START FROM 0 , ITSELF , JUST A STRUCTURAL DECISOION MADE FROM THE CORPORATION , send me an email for better info
            if(data_where[0] == ROOT){
                for( u32 i = 0; i < WHAT_COUNT ; i++ ) add_to_path(ROOT , NODE_SKELETON);   // first just add empty nodes in ROOT
                for( u32 i = 0; i < WHAT_COUNT ; i++ ) {                                    // to be initialized...
                    // needed in this case, is: creating an array every time, and give it WITH an added END_SKELETON (see function in instructions but also in core)
                    // in this case , is just 1 data added (being a loop, 1 per time), to init a brunc, thic can be done more than 1 time, WHAT_COUNT times to be exact.
                    // [data or node] [END_SKELETON]-_-'-_~"--> [branch begin n]            // these are representations of packed data, with end_skeleton
                    // [data or node] [END_SKELETON]-_-'-_~"--> [other branch - begin]      // being written in the destination branch, if it was visualizable
                    u32 new_name_for_new_branch[2] = { data_what[i] , END_SKELETON };
                    init_node(ROOT,new_name_for_new_branch);     // initializing new empty node    

                }     
                // RESPONSE
                response_size = raw_path_from(ROOT) * sizeof(u32);                          // NEEDED for tcp response, network stuff, see return statement
                format_response( buffer , path_buffer , response_size );                    // just in case copy 4 bytes data as line of 1 bytes
                break;
            }

            //  IF TO ADD NOT IN ROOT , FOLLOW PATH FUNCTION WORKS
            //  or needed is always to specify the path from root already from the other program sending TCP
            //  much prettyer for hypster and people with university payed from the parents not adding always root..
            //  the intellectual guy can tell to girls that he is sending data to : 
            //  client/items_bought/book_123/page_1/possible_secret_references/client/cart 
            //  instead of: ROOT/client/items_bought/book_123/page_1/possible_secret_references/client/cart
            //  exactly.. e-pathy can have loops.. not jet but is easy.. you already know what im thinking.. but, and hippy what can also have???
            u32 path_begin = follow_path( data_where , WHERE_COUNT );

            for(u32 i = 0; i< WHAT_COUNT ; i++) add_to_path( path_begin , NODE_SKELETON );
            // giving a -name- to the new path, and --> ADD end_skeleton <-- , same as the procedure of ROOT
            for(u32 i = 0; i< WHAT_COUNT ; i++) {
                u32 new_name_for_new_branch[2] = { data_what[i] , END_SKELETON };           // is needed every time also an end skeleton
                init_node( path_begin , new_name_for_new_branch );                           
            }  
            // RESPONSE
            count = raw_path_from(path_begin);                                              // just get the count of elements in path, 
            response_size = count * sizeof(u32);                                            // NEEDED for tcp response, network stuff, see return statement
            format_response(buffer , path_buffer , response_size);                          // practically unuseful path_buffer is not been updated
            break;
        // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
        case DIAPLAY_PATH: // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
            if( data_where[0] == ROOT ){
                count = raw_path_from( ROOT );                              // fill path_buffer with raw path ( type with location )
                // check path_buffer [*][*][*]_find_first_32bit_data->[begin]<-[][][][][end] of every node in this layer
                firsts_in_path( count );                                    // as already visualized, check what is the first data,the "name" for every node in the path
                // RESPONSE
                response_size = count * sizeof( u32 );                      // NEEDED for tcp response, network stuff, see return statement
                format_response( buffer , path_buffer , response_size );    // firsts_in_path(count) fills path_buffer
                break;
            }

            path_begin = follow_path( data_where , WHERE_COUNT );           
            count = raw_path_from( path_begin );
            count = firsts_in_path( count );                                // collect all "names" of all nodes in path
            // response
            response_size = count*sizeof(u32);                              // needed for network stuff, see return statement
            format_response( buffer , path_buffer , response_size );

        break;

        
        case DELETE:
            // it will
            // until now is only possible to delete only one -data-
            // it can become brutal to delete a branch, because can contain other branches, and this is e-pathy...
            
            // neded a bit of pseudo code
            
            /*
            get the branch
            path_begin = follow_path(data_where , WHERE_COUNT);
            count = raw_path_from( path_begin );
            
            now path_buffer has all the nodes in the path
            [][][]...
            
            can be copied to have it doing the next steps
            base_buffer[][][]
            
            istinct is to go in the first and see what there is -> raw path - path buffer
            --> [1][1][1][1]
            and also in the second -> append 
            --> [1][1][1][1][2][2][2] 
            and third also because is the last -> append
            --> [1][1][1][1][2][2][2][3][3]
      
      	    and keep the lengths
            len = 4,3,2
            
            now path buffer can stay in memory...
            
            then we open all and append everything, there are lengths
            [1][1][1][1][2][2][2][3][3] [1-1][1-1] [1-2][1-2][1-2] [2-1][2-1] [2-2] [2-3][2-3] [3-1][3-1] [3-2][3-2]
            len = 4,3,2, 2,3 2,1,2 2,2
            
            now the first layer is unpackaged.. 
            then we need a stack for layers
            layers = [ 4+3+2 ] [2+3+2+1+2+2+2]
	
	   we keep sum of len as the other begin for now
	   the_middle_begin = layers length - 1 [] 
	   
	    ____________________________________________________
	   
	    [1][1][1][1][2][2][2][3][3] [1-1][1-1] [1-2][1-2][1-2] [2-1][2-1] [2-2] [2-3][2-3] [3-1][3-1] [3-2][3-2]
            len = 4,3,2, 2,3 2,1,2 2,2
            ____________________________________________________
            
            len can become new..
            
            [1][1][1][1][2][2][2][3][3] [1-1][1-1] [1-2][1-2][1-2] [2-1][2-1] [2-2] [2-3][2-3] [3-1][3-1] [3-2][3-2]
            
            len_index = 0
            len = [0]
            layers = [ 4+3+2 ] [2+3+2+1+2+2+2]
            
            [1][1][1][1][2][2][2][3][3] --- [1-1][1-1] [1-2][1-2][1-2] [2-1][2-1] [2-2] [2-3][2-3] [3-1][3-1] [3-2][3-2] --- [1-1-1][1-1-1] [1-1-2][1-1-2] [1-2-1][1-2-1] [1-2-2][1-2-2] 
	    
	    len = 2,2,2,2
	   
	    now len can become an integer , a 32bit integer len = is if was 8
	   
	    layers = [ 4+3+2 ] [2+3+2+1+2+2+2] [8 or more]
	
	    if in the second [1-1-1] there is a data and not a node, will be just deleted, length does not increase and the content is not appended,
	    
	    even layers can be a constant lenght and have only last layer [begin][end]
            
            */
        break;

        default:
            path_buffer[0] = [0];
            format_response( buffer , path_buffer , 4 );        // first word in the relacy file as response, error is not implemented jet
        break;
    }

    return response_size;
}
