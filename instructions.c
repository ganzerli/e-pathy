


// SOME INPUT
    I4* new_data_or_nodes = malloc(sizeof(I4) * 128);
    new_data_or_nodes[0] = NODE_SKELETON;
    new_data_or_nodes[1] = 0b01010101010101010101010101010101;
    new_data_or_nodes[2] = 0b01010101010101010101010101010101;
    new_data_or_nodes[3] = 0b01010101010101010101010101010101;
    new_data_or_nodes[4] = END_SKELETON;
//  END SOME INPUT
    
    I4 begin = 0;
    I4 n_breaks = 0;

    // ADD node or data to a path [begin*]
    // if( 0 ){
    //    int32count = add_node_or_data_to( file_buffer, begin, NODE_SKELETON );
    //    save(FILENAME , file_buffer , int32count);
    // }

    // // INIT first uninitialized node [NODE_SKELETON] found in path from [begin*] and add array of new data or nodes
    // if( 0 ){
    //     n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed to know when to jump where
    //     init_node_in_path( file_buffer, begin, ends_buffer, n_breaks, new_data_or_nodes );
    //     save(FILENAME , file_buffer , int32count);
    // }
    
    // // PRINTING    
    // begin = 0; // whatever is the path to print
    // begin = get_node_begin(file_buffer , begin);                                        // whats filebuffer[begin] ->* points to 
    // get_path(file_buffer, path_buffer , begin );                                        // print path from that --> *[begin]

    // I4 todelete = 5;
    // // DELETE
    // if( 0 ){
    //     n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed for delete
    //     if ( delete(file_buffer , ends_buffer[n_breaks-1] ,  todelete ) ){
    //         garbage_sort();
    //         save(FILENAME , file_buffer , int32count);
    //     }
    // }

    // // PRINTING    
    // begin = 0; // whatever is the path to print
    // begin = get_node_begin(file_buffer , begin);                                        // whats filebuffer[begin] ->* points to 
    // get_path(file_buffer, path_buffer , begin );                                        // print path from that --> *[begin]