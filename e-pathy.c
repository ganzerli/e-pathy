#include <stdio.h>
#include <stdlib.h>

 // 4 bytes integer
typedef u_int32_t  I4;    
#include "tools.c"

#define SOMETHING_WENT_WRONG    0b11111111111111111111111111111111
#include "garbage_collection.c"

#define FILENAME "file"
#define _1GB 1073741823

#define NODE_SKELETON           0b11000000000000000000000000000000
#define DATA_SKELETON           0b01000000000000000000000000000000
#define END_SKELETON            0b00000000000000000000000000000000

#define MAX_FILE_SIZE           ( SOMETHING_WENT_WRONG - 1024 )
//  E R R O R S
#define ERROR_1                 ( SOMETHING_WENT_WRONG - 1 )
#define ERROR_2                 ( SOMETHING_WENT_WRONG - 2 )
#define ERROR_3                 ( SOMETHING_WENT_WRONG - 3 )
#define ERROR_4                 ( SOMETHING_WENT_WRONG - 4 )

// global variables
I4 int32count = 0;

// I know, I know.. and i even like Javascript...
////////////////////////////////////////////////////////////////////////////////        IS END       ////
unsigned int is_END(I4 num){
   return  ( 0b00111111111111111111111111111111 | num ) == 0b00111111111111111111111111111111;
}
////////////////////////////////////////////////////////////////////////////////        IS NODE       ////
unsigned int is_NODE(I4 num){
   return  ( 0b00111111111111111111111111111111 | num ) == 0b11111111111111111111111111111111;
}
////////////////////////////////////////////////////////////////////////////////        IS DATA      ////
unsigned int is_DATA(I4 num){
   return  ( 0b00111111111111111111111111111111 | num ) == 0b01111111111111111111111111111111;
}

////////////////////////////////////////////////////////////////////////////////        TRIM FIRST 2 BITS       ////
I4 trim_first_2_bits(I4 data){
    I4 trimmed = 0;
    I4 mask = 0b00111111111111111111111111111111;
    trimmed = data & mask;
    return trimmed;
}

////////////////////////////////////////////////////////////////////////////////        REQUIRE MEMORY       ////
I4 require_memory(I4 amount, I4 limit){
    garbage_turn_bin();                                                             // Look in garbage
    garbage_get_compost( amount );                                                  // watch if somthing useful

    // when in garbage is nothing usable scavaging.count is set to 0
    if(scavaging.count != 0){
        garbage_memory_allocated();                                                 // if space update garbage file
        scavaging.count = 0;                                                        // reset global struct
        return scavaging.index_data_file;                                           // return the index for this file
    }
    // SOMETHING FOR SCALABILITY
    if(limit > MAX_FILE_SIZE){
        // CHECK FOR NEW FILE
    }
    return limit;                                                                   // if nothing in thrash
}

////////////////////////////////////////////////////////////////////////////////        CHECK FILE       ////
// CHECKS IF FILE EXISTS, AND RETURNS NUMBER OF 32BITS INT IN THE FILE
I4 epathy_check(){
    I4 ints_count = 0;                     
    I4 bytes_count = 0;
    I4 const end_skeleton = END_SKELETON;
    FILE *fp;

    fp=fopen(FILENAME,"ab");                                            
    // CHECK IF THERE IS AT LEAST 1 (wll be always 4 per fwrite) BYTE IN THE FILE
    // IF THE FILE DOES NOT HAVE ANY BYTE, INITIALIZE THE FILE WITH A 4 BYTE INTEGER VALUE 0
    fseek(fp, 0, SEEK_END);                                                         // moving pointer to end file
    bytes_count = ftell(fp);                                                        // get position of pointer
    fclose(fp);
    // if no bytes in the file write a [END BRANCH] as first 4 bytes integer
    if(bytes_count  == 0){
      
        fp=fopen(FILENAME,"ab");   
        fwrite(&end_skeleton, sizeof (I4), 1, fp);

        ints_count++;    
        fclose(fp);
    }else{
        // the file has only 4 bytes integer
        ints_count = bytes_count / 4;
    }

    // print some interesting info
    printf("There are currently %d 4-bytes INTEGERS in the file \n", ints_count );
    printf("Current filesize: %d bytes\n", bytes_count );

    return ints_count;
}

////////////////////////////////////////////////////////////////////////////////        SAVE IN FILE       ////
void save(char *filepath , I4* newbuffer , I4 count ){
    I4 curren_integer = 0;
    FILE *fp =fopen(filepath,"wb");
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", FILENAME, __FILE__, __LINE__ - 1);
    
    // write the file with new paths
    for (I4 i = 0; i < count; i++){
        curren_integer = newbuffer[i];
        fwrite(&curren_integer, sizeof (I4), 1, fp);
    }
    fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////        LOAD FILE       ////
void load_file(I4* buffer, char* filename , I4 count){
    I4 curren_integer = 0;
    FILE *fp =fopen(filename,"rb");
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", filename, __FILE__, __LINE__ - 1);

    // save every INT32 from the file in the buffer
    for (I4 i = 0; i < count; i++){
        fread(&curren_integer, sizeof (I4), 1, fp);
        buffer[i] = curren_integer;   
    }
    fclose(fp);
}
                                                                                
////////////////////////////////////////////////////////////////////////////////        FIND ENDS       ////
I4 find_ENDs( I4* filebuffer , I4* ENDsbuffer , I4 path_begin ){
    I4 ends = 1;
    I4 end = 0;
    I4 i = path_begin;
    unsigned char exit = 0;
    I4 END_points_to = 0;

    printf("\n   Finding ENDs from branch [%d]\n\n", path_begin);

    while (!exit == 0){
        printf("[%u] -> %u \n",i, filebuffer[i]);
        if( is_END( filebuffer[i] ) ){
            
            END_points_to = trim_first_2_bits(filebuffer[i]);
            printf("END at filebuffer[%u] brings to...: [%u] \n", i , END_points_to);

            if(END_points_to == END_SKELETON){
               // end = path_begin + i;
                ENDsbuffer[ends-1] = i;
                printf("ends buffer[%u] = %u\n",ends - 1, ENDsbuffer[ends]);
                exit = 1;
                break;
            }else{
                end = i;
                i = END_points_to;                                                  // set position of i at p* of [END p*]    
                ENDsbuffer[ends] = end;                                             // save [END] in ENDsbuffer[last]
                printf("ends buffer[%u] = %u\n",ends, ENDsbuffer[ends]);
                ends++;                                                             // point to next index in ENDsbuffer[next last]
                printf("[%u] -> %u \n",i, filebuffer[i]);
            }
        }
        i ++;
    }
    return ends;
}

////////////////////////////////////////////////////////////////////////////////        CHECK FOR SPACE RIGHT AFTER       ////
// BEEING AT POSITION is_END()
I4 if_space_after(I4 position , I4* filebuffer , I4 limit){
    I4 result = 0;
    //  if it is the END OF THE FILE
    if(position == limit -1) return limit-1;
    // BEEING AT POSITION is_END() if there is just place after it
    if(filebuffer[position] == END_SKELETON && filebuffer[position+1] == END_SKELETON) return position;
    return result;
}

////////////////////////////////////////////////////////////////////////////////        ADD NODE TO       ////
I4 add_node_or_data_to(I4* filebuffer , I4 path_begin , I4 new_node_or_data){

    I4 i = path_begin;
    unsigned char stop = 0;
    I4 check = 0;
    I4 available_memory_at = 0;
    I4 END_points_to = 0;
    I4 limit = int32count;                                                          // first keep global variable as is

    printf("\n   Adding Node in branch [%u]\n\n", path_begin );

    // find a spot and write an uninitialized node
    while (stop == 0){
        printf("[%u] -> %u \n",i, filebuffer[i]);

        if( is_END( filebuffer[i] ) ){
            if(filebuffer[i] == END_SKELETON){                                      // end of the branch
                // if is End Of File (it h-appen-d often)
                // or there is somehow [END_SKELETON][END_SKELETON] --> could be an end of branch, and one element was deleted.
                // is no need to rewrite the end as a pointer
                // the end is just replaced 1 position furder and a new data is written in this position
                if( (limit-1)  == if_space_after(i, filebuffer , limit) ){          // there is space right after [END]->[END]...

                    filebuffer[i] = new_node_or_data;                               // good place          [write here]  [END]...
                    printf("[new node] %u , is written at filebuffer[%u]\n", new_node_or_data , i);

                    filebuffer[i+1] = END_SKELETON;                                 // anyway [END]      [written here]  [END]...
                    printf("[END_SKELETON] placed at filebuffer[%u]\n", i+1);

                    limit ++;                                                       // update int32count
                    stop = 1;                                                       // exit loop
                    break;
                }else{                                                              // no space right after
                    available_memory_at = require_memory(2,limit);
                    printf(" memory available at [%u]\n", available_memory_at );

                    filebuffer[i] = END_SKELETON + available_memory_at;             // rewrite [END] as [END p*]
                    printf(" filebuffer[%u] END now pointing to--> [%u]\n", i, available_memory_at );
                    printf("[%u] now is -> %u\n",i, filebuffer[i] );
                    // set position of new node in this [END]  ...[END p*]
                    filebuffer[available_memory_at] = new_node_or_data;             // [END *p]---..''-->[p*] = new_node or data 
                    limit ++;                                                       // update int32count
                    printf(" new_node_or_data[%u] written in filebuffer[%u]\n", new_node_or_data, (available_memory_at) );
                    // add END_SKELETON right after that
                    filebuffer[available_memory_at +1] = END_SKELETON;              // [END *p]---..''-->[p*][END]
                    limit ++;                                                       // update int32count
                    printf(" [END_SKELETON] written in filebuffer[%u]\n", (available_memory_at+1) );
                    stop = 1;                                                       // exit the loop
                    break;
                }
            }else{
                // END is a pointer [ 00 101010101...] ---> *[101010101...]
                END_points_to = trim_first_2_bits(filebuffer[i]);
                printf("end found at filebuffer[%u] containing [%u]\n", i , END_points_to);

                if (END_points_to > END_SKELETON){                                  // type of [END] is [00] in this architecture , everything is more than 0 
                    i = END_points_to;                                              // JUMP and continue reading brunch [END *p]---..''-->[data] [node][data][data][node][node]
                    printf("[%u] -> %u \n",i, filebuffer[i]);
                }                                                                   // NO NEED OF i-- [END *p]---..''--> [i now][next in loop, could be END]
            }
        }
        i ++;
    }
    int32count = limit;
    return available_memory_at;
}

/////// !! array NEW DATA OR NODES NEEDS TO END WITH [END_SKELETON] !! /////////        INIT NODE IN PATH       ////
I4 init_node_in_path(I4* filebuffer, I4 path_begin,  I4 *ENDs, I4 ends, I4 *new_data_nodes){
    
    printf("\n\n    init_node_in_path:\n\n");
    I4 found_and_initialized = SOMETHING_WENT_WRONG;                                // flag to exit if nothing found
    I4 i = path_begin;                                                              // set i at filebuffer[path_begin]
    I4 branch_break = 0;                                                            // set brunch_break at first [END] of ENDs buffer
    I4 filebuffer_count = int32count;                                               // keep first int32count global variable there 

    while(i < ENDs[ends-1]){                                                        // [END] of the branch
        printf("filebuffer[%u] = %u\n", i , filebuffer[i]);

        if (filebuffer[i] == NODE_SKELETON){                                        // uninitialized node found
            printf("filebuffer[%u] = %u  == %u\n" , i, filebuffer[i] , NODE_SKELETON);
            found_and_initialized = i;                                              // save the index
            i = SOMETHING_WENT_WRONG;                                               // exit loop
            break;
        }
        if(i == ENDs[branch_break] ){                                               // position is on a break [END p*]
            printf("filebuffer[%u] ,  Ends[branch_break] == %u \n" , i , ENDs[branch_break]  );
            //data_variable = trim_first_2_bits(filebuffer[i]);                     // END TYPE IS ALREADY [00]..101010...
            branch_break ++;                                                        // set break to next of ENDs buffer
            i = filebuffer[i];                                 
        }else{                                                                      // if not a bresk [END p*]
            i++;                                                                    // continue               
        } 
    }
    // RETURN IF NOTHING FOUND
    if(found_and_initialized == SOMETHING_WENT_WRONG){
        printf("\nNOT ANY uninitialized node found: NODE_SKELETON in path: /%u not foud, you need to add_node_or_data_to -> %u ", path_begin , path_begin);
        return filebuffer_count;
    }

    // UNINITIALIZED NODE [NODE_SKELETON] WAS FOUND
    printf("\nNODE_SKELETON at position[%u] , initializing...\n",found_and_initialized);
    printf("in fielbuffer[%u] the data is: [%u]\n", found_and_initialized, filebuffer[found_and_initialized]);
    printf("inititalizing node.....\n\n");
    // find space
    I4 count = 0;
    I4 memory_position = filebuffer_count;

    while(new_data_nodes[count] != END_SKELETON){
        count ++;
    }
    // LOOP JUMPS AT END_SKELETON count needs +1
    count ++;

    printf(" %u of memory are required \n", count);
    memory_position = require_memory(count , filebuffer_count);                         // get position where to write
    printf(" memory available at : [%u]\n", memory_position );

    for ( i = 0; i < count; i++){
        filebuffer[memory_position + i] = new_data_nodes[i];                            // write new set from that position
        printf("new data written in filebuffer[%u]  is --> %u\n" , (memory_position + i) , filebuffer[memory_position + i]);
    }

    filebuffer[found_and_initialized] = NODE_SKELETON + memory_position;                // replace (fill) NODE_SKELETON with p* of beginnin
    filebuffer_count += count; 
    int32count = filebuffer_count;
    printf("\nfond and initialized at position[%u], now is containing the value[*%u]\n",found_and_initialized, filebuffer[found_and_initialized] - NODE_SKELETON );
    return memory_position;
}

////////////////////////////////////////////////////////////////////////////////        GET BEGIN FROM NODE       ////
I4 get_node_begin( I4* filebuffer, I4 node){
    printf("\n\n     Node begin\n\n");
    I4 node_begin = 0;
    node_begin = filebuffer[node];                                                      // look in the node
    node_begin = trim_first_2_bits(node_begin);                                         // get only the data (trim type)

    printf("node_begin in filebuffer[%u] points to --> [%u]\n", node, node_begin);
    return node_begin;
}

////////////////////////////////////////////////////////////////////////////////        GET PATH FROM BEGIN       ////
I4 get_path( I4* filebuffer, I4* path_buffer, I4 node_begin){

    printf("\n\n     Get path\n\n");
    printf("node begin : %u  , filebuffer[node_begin] = %u\n", node_begin, filebuffer[node_begin]);

    I4 i = node_begin;
    I4 count = 0;

    while( filebuffer[i] != END_SKELETON){
        if(is_END(filebuffer[i])){                                                      // other [END] or brake [p*]
            // JUMP
            printf("filebuffer[%u] = %u , IS END \n", i ,filebuffer[i]);
            i = trim_first_2_bits(filebuffer[i]);
            printf("following path: filebuffer[%u] = %u\n", i ,filebuffer[i]);
        }else{                                                                          // data or node
            // GET
            path_buffer[count] = filebuffer[i];
            printf( "path buffer %u/[%u] = %u  , in filebuffer[%u]\n" , node_begin , count , path_buffer[count] , i );
            count ++;
            i++;
        }
    }
    return count;
}

////////////////////////////////////////////////////////////////////////////////        DELETE DATA       ////
I4 delete(I4* file_buffer , I4 i_end , I4 i_ndex ){

    I4 result = is_DATA(file_buffer[i_ndex]);
    if(!result){
        printf("\n\n     DELETING \nfilebuffer[%u] IS NOT TYPE DATA\n", i_ndex);
        return result;
    } 

    I4 data = trim_first_2_bits(file_buffer[i_ndex]);                                   // gat only the data (trim type)
    // writing the last element of the branch in the element to delete
    file_buffer[ i_ndex ] = file_buffer[ i_end - 1 ];
    // set an END_SKELETON to that last element, that is now in the place of the deleted element
    file_buffer[ i_end - 1 ] = END_SKELETON;

///////////////// INTEGRATING GARBAGE COLLECTION LIBRARY /////////////////////////
    // Write free space in garbage collection                                   //
    garbage_drop_in_trash(i_end);                                               //
    // update count for thrash                                                  //
    garbage32count++;                                                           //
//////////////////////////////////////////////////////////////////////////////////
    printf("deleted: %u ; ( data + type %u ) - type %u = data %u " , data , file_buffer[i_ndex] , DATA_SKELETON , data);
    result = 1;                                                                         // obviously garbage collection has no bugs
    return result;
}


////////////////////////////////////////////////////////////////////////////////        MAIN       ////
int main() {

    printf("\n\n    WELCOME TO E-PATHY\n\n");
    printf("    e-pathy is saving integers creating paths, of integers..\n");
    printf("    so if you like to have fun you can have /0/50/0/1 containing 1 , 2 , 3 , 4 , 5 or other integers.\n");
    printf("    Only some people don't have a limit in the definition of what they are doing in the world, they are also many.\n");
    printf("    Anyway, e-pathy does have a limit, e-pathy cannot store integers bigger than: \n");
    printf("    1 Billion \n");
    printf("    073 Millions \n");
    printf("    741 Tousend \n");
    printf("    8 Hundered \n");
    printf("    2 times 10 \n");
    printf("    and 3 Units \n");
    printf("\n\n    That has a reason, and to know it you need to contact the creator of all this and offer a relatable amount of money\n\n\n");

    int32count = epathy_check();
    garbage32count = garbage_check();

    //print_whole_file(FILENAME ,int32count);
    I4* file_buffer = malloc( _1GB ); 
    I4* ends_buffer = malloc( sizeof (I4) * 256 );     
    I4* path_buffer = malloc( sizeof (I4) * 256 );

    load_file(file_buffer,FILENAME,int32count);

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
    if( 0 ){
       int32count = add_node_or_data_to( file_buffer, begin, NODE_SKELETON );
       save(FILENAME , file_buffer , int32count);
    }

    // INIT first uninitialized node [NODE_SKELETON] found in path from [begin*] and add array of new data or nodes
    if( 0 ){
        n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed to know when to jump where
        init_node_in_path( file_buffer, begin, ends_buffer, n_breaks, new_data_or_nodes );
        save(FILENAME , file_buffer , int32count);
    }
    
    // PRINTING    
    begin = 0; // whatever is the path to print
    begin = get_node_begin(file_buffer , begin);                                        // whats filebuffer[begin] ->* points to 
    get_path(file_buffer, path_buffer , begin );                                        // print path from that --> *[begin]

    I4 todelete = 5;
    // DELETE
    if( 0 ){
        n_breaks = find_ENDs( file_buffer , ends_buffer , begin );                      // needed for delete
        if ( delete(file_buffer , ends_buffer[n_breaks-1] ,  todelete ) ){
            garbage_sort();
            save(FILENAME , file_buffer , int32count);
        }
    }

    // PRINTING    
    begin = 0; // whatever is the path to print
    begin = get_node_begin(file_buffer , begin);                                        // whats filebuffer[begin] ->* points to 
    get_path(file_buffer, path_buffer , begin );                                        // print path from that --> *[begin]


    // #free_heap
    free(file_buffer);
    free(ends_buffer);
    free(path_buffer);
    free(new_data_or_nodes);

   return EXIT_SUCCESS;
}