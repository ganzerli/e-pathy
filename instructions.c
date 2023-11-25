
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

// search 1 NODE_SKELETON (uninitialized node)
// give a location and write array of data (or nodes) in
// END_SKELETON NEEDED at LAST INDEX of dataArray
u32 init_node( u32 branchBegin , u32* dataArray ){
    n_breaks = find_ENDs( file_buffer , ends_buffer , branchBegin );                      // needed to know when to jump where
    init_node_in_path( file_buffer, branchBegin, ends_buffer, n_breaks, dataArray );
    // save in file back (in need of speed is also... but is good)
    save(FILENAME , file_buffer , int32count);
}

// index of file buffers
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
u32 raw_path_from(u32 selected_node){
    // from a filebuffer[begin] fill path buffer with nodes or data in it
    u32 count = get_path(file_buffer, path_buffer , selected_node );                            // print path from that --> *[begin]
    return count;
}

// A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N  ABSTRACTION
// ABSTRACTION  A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N
// A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N  ABSTRACTION

// replaces raw_nodes with first data found in each one
void first_in_nodes(u32 count){
    u32 to_index = 0;
    for(u32 i = 0; i< count;i++){
        // in this architecture every first data in a path rapresents a "name"
        if( !is_NODE(path_buffer[i]) ){
            printf("\ndata in raw_node[%u] is not a node, is: %u" , i, path_buffer[i] );
            path_buffer[i] = 0;
            continue;
        }
        to_index = trim_first_2_bits (path_buffer[i]);
        path_buffer[i] = trim_first_2_bits(file_buffer[to_index]); 
    }
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