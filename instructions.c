
/*
E-pathy saves every 32bit data in the given "path" of filebuffer.
So there is not real protection for which data shuld be there or would be much slower and not creative.
A good idea is to have all layers made only from nodes:
- getting the path, look for path begin, getting the path, and so on..
and only at the last layer insert data..

This relacy architecture will be like that:
[0][1][2][3]
    |
     ---->[data - path name 0 ][ node 1][ node 2][ node 3]
                                            |
                                             -----> [path name 0] [nd 1][nd 2][nd 3]

Only thing that e-pathy always does is: having a filebuffer[id] begin, run until [end]
So: -add to path, adds after. -> no sorting, here
    -init runs to find first [NODE-SKELETON] 11.and.all.0 , ask for memory and writing starting index after the 11 
    -delete brings the last element, to that index, and overwrites the last with [end] (organizing garbage collection together with ask for memory)
and basically nothing else..
*/


// those coulb be useful somewhere else...
u32 ends_buffer[256];   
u32 path_buffer[256];


// SOME INPUT
    u32 new_data_or_nodes[5];
    new_data_or_nodes[0] = NODE_SKELETON;
    new_data_or_nodes[1] = 0b01010101010101010101010101010101;
    new_data_or_nodes[2] = 0b01010101010101010101010101010101;
    new_data_or_nodes[3] = 0b01010101010101010101010101010101;
    new_data_or_nodes[4] = END_SKELETON;
//  END SOME INPUT
    
u32 begin = 0;
u32 n_breaks = 0;

u32 add_to_path(u32 path_begin , u32 data){
// ADD node or data to a path [begin*]
// if( 0 ){
//    int32count = add_node_or_data_to( file_buffer, begin, NODE_SKELETON );
//    save(FILENAME , file_buffer , int32count);
// }
}

// // INIT first uninitialized node [NODE_SKELETON] found in path from [begin*] and add array of new data or nodes
// if( 0 ){
//     n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed to know when to jump where
//     init_node_in_path( file_buffer, begin, ends_buffer, n_breaks, new_data_or_nodes );
//     save(FILENAME , file_buffer , int32count);
// }


u32 delete(u32 index){
// u32 todelete = 5;
// // DELETE
// if( 0 ){
//     n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed for delete
//     if ( delete(file_buffer , ends_buffer[n_breaks-1] ,  todelete ) ){
//         garbage_sort();
//         save(FILENAME , file_buffer , int32count);
//     }
// }
    return trim_first_2_bits( file_buffer[index] )
}

// lodaing path in path buffer
// having 0 prints the root branch
// example: running path() again for every data found (hope is all nodes) will be displayed the first layer of the graph
void path(u32 path_begin){
// // PRINTING    
// begin = 0; // whatever is the path to print
// begin = get_node_begin(file_buffer , begin);                                        // whats filebuffer[begin] ->* points to 
// get_path(file_buffer, path_buffer , begin );                                        // print path from that --> *[begin]
}