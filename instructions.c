
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

u32 begin = 0;
u32 n_breaks = 0;

//ADD node (or data -> deprecating) to a path [begin*]
// data will be almost always NODE_SKELETON
u32 add_to_path(u32 path_begin , u32 data){
    add_node_or_data_to( file_buffer, path_begin, data );
    save(FILENAME , file_buffer , int32count);
}

// // INIT first uninitialized node [NODE_SKELETON] found in path from [begin*] and add array of new data or nodes

// // SOME INPUT
//     u32 new_data_or_nodes[5];
//     new_data_or_nodes[0] = NODE_SKELETON;
//     new_data_or_nodes[1] = 1;
//     new_data_or_nodes[2] = 2;
//     new_data_or_nodes[3] = 3;
//     new_data_or_nodes[4] = END_SKELETON;
// //  END SOME INPUT

// having an unitialized node -NODE_SKELETON- in a path , 
// walk throught the path, find that kind of node, search memory, assing the location in that node
// and write the location of the first element in found NODE_SKELETON
// COUNT is NOT NEEDED, but NEEDED IS END_SKELETON at LAST INDEX of dataArray
u32 init_node( u32 branchBegin , u32* dataArray ){
    n_breaks = find_ENDs( file_buffer , ends_buffer , branchBegin );                      // needed to know when to jump where
    init_node_in_path( file_buffer, branchBegin, ends_buffer, n_breaks, dataArray );
    save(FILENAME , file_buffer , int32count);
}

// if( 0 ){
//     n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed to know when to jump where
//     init_node_in_path( file_buffer, begin, ends_buffer, n_breaks, new_data_or_nodes );
//     save(FILENAME , file_buffer , int32count);
// }

u32 del(u32 index){
    u32 todelete = index;
    u32 deleted = file_buffer[index];
    u32 n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed for delete
    if ( delete(file_buffer , ends_buffer[n_breaks-1] ,  todelete ) ){
        garbage_sort();
        save(FILENAME , file_buffer , int32count);
    }
    return trim_first_2_bits( deleted );
}

// lodaing path in path buffer
// having 0 prints the root branch
// example: running path() again for every data found (hope is all nodes) will be displayed the first layer of the graph
u32 path(u32 selected_node){
// PRINTING    
    u32 node_begin = 0;
    // having a selected node in filebuffer[node] , get filebuffer[begin]
    node_begin = get_node_begin(file_buffer , selected_node);                                // what filebuffer[begin] ->* points to 
    // from a filebuffer[begin] fill path buffer with nodes or data in it
    u32 count = get_path(file_buffer, path_buffer , node_begin );                            // print path from that --> *[begin]
    return count;
}


// first better using something getting names
void getToPath(u32*path , u32 count){
    u32 begin = 0;
    for(u32 i = 0; i < count; i++){
        begin = file_buffer[path[i]];
        begin = get_node_begin(file_buffer, begin);
        // print path buffers
    }
}