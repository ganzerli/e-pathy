
/*
E-pathy saves every 32bit data in the given "path" or position of filebuffer, which is just an array.
So there is not protection in the core framework for which data shuld be there or would be much slower and not creative.
A good idea for relacy is to have all layers made only from nodes:
- root, check nodes, get in, check nodes and so on..
and only at the last layer insert data..

This relacy architecture will be like that:
[0][1][2][3]
    |
     ---->[data - path name 0 ][ node 1][ node 2][ node 3]
                                            |
                                             -----> [path name 0] [nd 1][nd 2][nd 3]

Only thing that e-pathy always does is: having a filebuffer[id] begin, run until [end]
So: -add to path, adds after. -> no sorting, here
    -init searches first [NODE-SKELETON] 11.and.all.0 , asks for memory and writes starting index of following path-branch after the 11 
    -delete brings the last element in the branch, to that index, and overwrites the last with [end] (organizing garbage collection together with ask for memory)
and basically nothing else..
*/

#define PATH_BUFFER_SIZE 256
#define ENDS_BUFFER_SIZE 256

u32 ends_buffer[ENDS_BUFFER_SIZE];   
u32 path_buffer[PATH_BUFFER_SIZE];
// needed to jump, to make some functions easier to develop..
u32 begin = 0;
u32 n_breaks = 0;

//ADD node o data to a path filebuffer[begin*]
// data will be almost always NODE_SKELETON
u32 add_to_path(u32 path_begin , u32 data){
    add_node_or_data_to( file_buffer, path_begin, data );
    save(FILENAME , file_buffer , int32count);
}

// search 1 NODE_SKELETON (uninitialized node)
// give a location and write array of data (or nodes) "in it"
// ! ! ! ! !    END_SKELETON  N E E D E D     at LAST INDEX of dataArray ! ! ! ! ! ! ! ! ! ! 
u32 init_node( u32 branchBegin , u32* dataArray ){
    n_breaks = find_ENDs( file_buffer , ends_buffer , branchBegin );                      // needed to know when to jump where
    init_node_in_path( file_buffer, branchBegin, ends_buffer, n_breaks, dataArray );
    // save in file back (in need of speed is also... but is good)
    save(FILENAME , file_buffer , int32count);
}

//          filebuffer[index]
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
// having 0 prints all the e-pathy format brances in the root branch
u32 raw_path_from(u32 selected_node){
    // from a filebuffer[begin] fill path buffer with nodes or data in it
    u32 count = get_path(file_buffer, path_buffer , selected_node );                            // print path from that --> *[begin]
    return count;
}

// A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N  ABSTRACTION
// ABSTRACTION  A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N
// A B S T R A C T I O N  ABSTRACTION  A B S T R A C T I O N  ABSTRACTION

// replaces raw_nodes in path_buffer with first data found in each one
u32 firsts_in_path(u32 count){

    printf("\n FIRSTS IN PATH \n");
    u32 raw_data = 0;
    u32 clean_data = 0;
    u32 path_buffer_i= 0;
    u32 node_name = 0;

    for(u32 i = 0; i< count;i++){
        // in this architecture every first data in a path rapresents a "name"
        raw_data = path_buffer[i];                              // for whom is coming from heavy job background, transitioning registers can be seen as fun.
        if( ! is_NODE(raw_data)  ) continue;
        clean_data = trim_first_2_bits(raw_data);
        // get data from pointed location
        raw_data = file_buffer[clean_data];
        clean_data = trim_first_2_bits(raw_data);               // get the "name" as was
        path_buffer[path_buffer_i] = clean_data;                // replace in path_buffer
        path_buffer_i++;
        printf("\npath buffer[%u] = %u", path_buffer_i ,clean_data);
    }
    return path_buffer_i;
}

// where is the id of a name in that path
// root/abc/ -> id of abc
//                             _-> first data in a path , part of relacy architecture
u32 name_in_id(u32 fromid, u32 name){
    printf("\n\n    NAME IN ID \n");
    u32 to_index = 0;
    // load raw nodes in path_buffer
    u32 count = raw_path_from(fromid);
    // check if name exists
    for(u32 i = 0; i < count; i++){
        // look in the node the node
        if( !is_NODE(path_buffer[i]) ){
            printf("\ndata in raw_node[%u] is not a node, is: %u" , i, path_buffer[i] );
            path_buffer[i] = 0;
            continue;
        }
        to_index = trim_first_2_bits (path_buffer[i]);
        if ( name == trim_first_2_bits(file_buffer[to_index]) ) {
            printf("\nnode named: %0x, is in filebuffer[%u]", name , to_index );
            return to_index;
        }

    }
    printf("\nnode named: %0x, not found in path filebuffer[%u]", name , fromid ); 
    return to_index;
}

// having path names find node after node to the last
u32 follow_path(u32*path , u32 count){
    printf("\n\n    FOLLOW PATH \n");
    u32 node_index = 0;
    // check if nodename in path exists in ROOT
    node_index = name_in_id( 0 , path[0]);
    if( node_index == 0 ) return 0;

    printf("\nname: %0x existing in ROOT , in filebuffer[%u]", path[0], node_index );

    // if path count is more than one..   [one] / [thwo] ...
    for(u32 i = 1; i < count; i++){
        node_index = name_in_id(node_index, path[i]);
        printf("\nname: %0x existing in %0x , in filebuffer[%u]", path[i] , path[i-1] , node_index );
    }
    
    return node_index;
}