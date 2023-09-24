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
    // SEARCH THROUGHT GARBAGE COLLECTOR..


    garbage_turn_bin();


    garbage_get_compost( amount );

    if(scavaging.count != 0){
        garbage_memory_allocated();
        scavaging.count = 0;
        return scavaging.index_data_file;
    }

    if(limit > MAX_FILE_SIZE){
        // CHECK FOR NEW FILE
    }
    return limit;
}

////////////////////////////////////////////////////////////////////////////////        CHECK FILE       ////
// CHECKS IF FILE EXISTS, AND RETURNS NUMBER OF 32BITS INT IN THE FILE
I4 epathy_check(){
    I4 ints_count = 0;                     
    I4 bytes_count = 0;
    I4 const end_skeleton = END_SKELETON;
    FILE *fp;

    // check is always at the beginning.. so we need to append or the file will be continuosly overwritten and could never exist anything...
    fp=fopen(FILENAME,"ab");                                            
    // CHECK IF THERE IS AT LEAST 1 (wll be always 4 per fwrite) BYTE IN THE FILE
    // IF THE FILE DOES NOT HAVE ANY BYTE, INITIALIZE THE FILE WITH A 4 BYTE INTEGER VALUE 0
    // moving pointer to end file
    fseek(fp, 0, SEEK_END);
    // get position of pointer
    bytes_count = ftell(fp);
    fclose(fp);
    // now last int has the position of the LAST BYTE of the file (not last integer, nor one of those used here)
    if(bytes_count  == 0){
        // if no bytes in the file write a [END BRANCH] as first 4 bytes integer
        fp=fopen(FILENAME,"ab");   

        fwrite(&end_skeleton, sizeof (I4), 1, fp);
        ints_count++;    

        fclose(fp);
    }else{
        // the file has only 4 bytes integer
        ints_count = bytes_count / 4;
    }
    return ints_count;
}

////////////////////////////////////////////////////////////////////////////////        SAVE IN FILE       ////
void save(char *filepath , I4* newbuffer , I4 count ){

    I4 curren_integer = 0;
    FILE *fp =fopen(filepath,"wb");

    // check if everything good to create or open the file
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", FILENAME, __FILE__, __LINE__ - 1);

    // print everything
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

    // check if everything good to create or open the file
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", filename, __FILE__, __LINE__ - 1);

    // print everything
    for (I4 i = 0; i < count; i++){
        fread(&curren_integer, sizeof (I4), 1, fp);
        ////////////////////////////////
        buffer[i] = curren_integer;   //
        ////////////////////////////////
    }
    fclose(fp);
}
                                                                                
////////////////////////////////////////////////////////////////////////////////        FIND ENDS       ////
I4 find_ENDs( I4* filebuffer , I4* endsbuffer , I4 path_begin ){
    I4 ends = 1;
    I4 end = 0;

    I4 i = path_begin;
    unsigned char stop = 0;
    I4 check = 0;

    I4 data_variable = 0;
    printf("\n   Finding ENDs from branch [%d]\n\n", path_begin);

    while (stop == 0){
        printf("[%u] -> %u \n",i, filebuffer[i]);
        if( is_END( filebuffer[i] ) ){
            
            data_variable = trim_first_2_bits(filebuffer[i]);
            printf("END at filebuffer[%u] brings to...: [%u] \n", i , data_variable);

            if(data_variable == END_SKELETON){
               // end = path_begin + i;
                endsbuffer[ends-1] = i;
                printf("ends buffer[%u] = %u\n",ends - 1, endsbuffer[ends]);
                stop = 1;
                break;
            }else{
                end = i;
                i = data_variable;
                endsbuffer[ends] = end;
                printf("ends buffer[%u] = %u\n",ends, endsbuffer[ends]);
                ends++;
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
I4 add_node_or_data_to(I4* filebuffer , I4 limit, I4 path_begin , I4 new_node_or_data){

    I4 i = path_begin;
    unsigned char stop = 0;
    I4 check = 0;
    I4 available_memory_at = 0;
    I4 END_points_to = 0;

    printf("\n   Adding Node in branch [%u]\n\n", path_begin );

    // find a spot and write an uninitialized node
    while (stop == 0){
        printf("[%u] -> %u \n",i, filebuffer[i]);

        if( is_END( filebuffer[i] ) ){
            if(filebuffer[i] == END_SKELETON){       
                // if is End Of File (it h-appen-d often) right now:
                // or there is somehow [END_SKELETON][END_SKELETON] --> could be an end of branch, and one element was deleted.
                // is no need to rewrite the end as a pointer
                // the end is just replaced 1 position furder and a new data is written in this position
                if( (limit-1)  == if_space_after(i, filebuffer , limit) ){

                    filebuffer[i] = new_node_or_data;
                    printf("[new node] %u , is written at filebuffer[%u]\n", new_node_or_data , i);

                    // if position is current end of file pad the end, in other case is anyway END_SKELETON.. until now is better then a condition(poor registers!!)
                    filebuffer[i+1] = END_SKELETON;
                    printf("[END_SKELETON] placed at filebuffer[%u]\n", i+1);

                    limit ++;
                    stop = 1;
                    break;
                }else{
                    // require memory , one for the new node and one for [END]
                    available_memory_at = require_memory(2,limit);
                    printf(" memory available at [%u]\n", available_memory_at );

                    // give to the end the pointer for that position
                    filebuffer[i] = END_SKELETON + available_memory_at;
                    printf(" filebuffer[%u] END now pointing to--> [%u]\n", i, available_memory_at );
                    printf("[%u] now is -> %u\n",i, filebuffer[i] );

                    // set new node at available memory
                    filebuffer[available_memory_at] = new_node_or_data;
                    limit ++;
                    printf(" new_node_or_data[%u] written in filebuffer[%u]\n", new_node_or_data, (available_memory_at) );

                    // add END_SKELETON right after that
                    filebuffer[available_memory_at +1] = END_SKELETON;
                    limit ++;
                    printf(" [END_SKELETON] written in filebuffer[%u]\n", (available_memory_at+1) );

                    stop = 1;
                    break;
                }
            }else{
                // END is a pointer [ 00 101010101...] ---> *[101010101...]
                END_points_to = trim_first_2_bits(filebuffer[i]);
                printf("end found at filebuffer[%u] containing [%u]\n", i , END_points_to);

                if (END_points_to > END_SKELETON){
                    i = END_points_to;
                    printf("[%u] -> %u \n",i, filebuffer[i]);
                    // NO NEED OF i--, HERE IS JUST FINDING FOR END TO WRITE THE NEXT DATA OR NODE IN THIS BRANCH
                }
            }
        }// if is_END(filebuffer[i]);
        i ++;
    }
    return limit;
}

////////////////////////////////////////////////////////////////////////////////        INIT NODE IN PATH       ////
I4 init_node_in_path(I4* filebuffer, I4 path_begin, I4 filebuffer_count, I4 *ENDs, I4 ends, I4 *new_data_nodes){
    
    // run throught filebuffer from path begin
    // find uninitialized node NODE_SKELETON

    printf("\n\n    init_node_in_path:\n\n");
    I4 found_and_initialized = 0;
    I4 i = path_begin;
    I4 branch_break = 0;

    // if a brake of branch found get continuation from ENDs array until last end
    printf("ENDs[ends] = %u \n" , ENDs[ends-1]);
    while(i < ENDs[ends-1]){

        printf("filebuffer[%u] = %u\n", i , filebuffer[i]);
        // when uninitialized node found
        if (filebuffer[i] == NODE_SKELETON){
            printf("filebuffer[%u] = %u  == %u\n" , i, filebuffer[i] , NODE_SKELETON);
            found_and_initialized = i;
            //i = SOMETHING_WENT_WRONG;
            break;
        }
        // if pointer found jump and continue searching from that position
        if(i == ENDs[branch_break] ){
            printf("filebuffer[%u] ,  Ends[branch_break] == %u \n" , i , ENDs[branch_break]  );
            // !!!! END_SKELETON BEGINS ALREADY WITH [00]..101010... SO THE POINTER IS ALREADY CLEAN
            // else the first 2 bits need to be trimmed
            //data_variable = trim_first_2_bits(filebuffer[i]);
            branch_break ++;
            // if filebuffer[i] is a break it contains the pointer to cotinue reading the branch
            i = filebuffer[i];                                 
        }else{
            i++;
        } 
    }
    
    if(found_and_initialized == 0){
        if(i != 0){
            printf("\nNOT ANY uninitialized node found: NODE_SKELETON in path: /%u not foud, you need to add_node_or_data_to -> %u ", path_begin , path_begin);
            return filebuffer_count;
        }
    }
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
    memory_position = require_memory(count , filebuffer_count);
    printf(" memory available at : [%u]\n", memory_position );

    for ( i = 0; i < count; i++){
        filebuffer[memory_position + i] = new_data_nodes[i];
        printf("new data written in filebuffer[%u]  is --> %u\n" , (memory_position + i) , filebuffer[memory_position + i]);
    }

    filebuffer[found_and_initialized] = NODE_SKELETON + memory_position;
    filebuffer_count += count; 
    printf("\nfond and initialized at position[%u], now is containing the value[*%u]\n",found_and_initialized, filebuffer[found_and_initialized] - NODE_SKELETON );
    return filebuffer_count;
}

////////////////////////////////////////////////////////////////////////////////        GET BEGIN FROM NODE       ////
I4 get_node_begin( I4* filebuffer, I4 node){
    printf("\n\n     Node begin\n\n");

    I4 node_begin = 0;
    node_begin = filebuffer[node];
    node_begin = trim_first_2_bits(node_begin);
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
        // printf( "path buffer %u/[%u] = %u  , in filebuffer[%u]\n" , node_begin , count , path_buffer[count] , i );
        if(is_END(filebuffer[i])){
            // JUMP
            printf("filebuffer[%u] = %u , IS END \n", i ,filebuffer[i]);
            i = trim_first_2_bits(filebuffer[i]);
            printf("following path: filebuffer[%u] = %u\n", i ,filebuffer[i]);
        }else{
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
        printf("\n\n     DELETING \nfilebuffer[%u] IS NOT A DATA, IS A NODE -->", i_ndex);
        return result;
    } 

    I4 data = trim_first_2_bits(file_buffer[i_ndex]);
    printf("deleted: %u ; ( data + type %u ) - type %u = data %u " , data , file_buffer[i_ndex] , DATA_SKELETON , data);

    // writing the last element of the branch in the element to delete
    file_buffer[ i_ndex ] = file_buffer[ i_end - 1 ];
    // set an END_SKELETON to that last element, that is now in the place of the deleted element
    file_buffer[ i_end - 1 ] = END_SKELETON;

    // Write free space in garbage collection
    garbage_drop_in_trash(i_end);
    // update count for thrash
    garbage32count++;

    printf("\n\nfree space should be: %u" , i_end);
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

    // when no file or empty format for use
    I4 tempI4 = epathy_check();
    I4 int32count = tempI4;
    I4 filesize = (tempI4 * 4);

    // PRINT USEFUL INFO
    printf("There are currently %d 4-bytes INTEGERS in the file \n", int32count );
    printf("Current filesize: %d bytes\n", filesize );

    //print_whole_file(FILENAME ,int32count);

    I4* file_buffer = malloc( _1GB ); 
    I4* ends_buffer = malloc( sizeof (I4) * 128 );     
    I4* path_buffer = malloc( sizeof (I4) * 128 ); 
    I4* new_data_or_nodes = malloc(sizeof(I4) * 128);
    
    load_file(file_buffer,FILENAME,int32count);

    I4 newBranch =  NODE_SKELETON ;
    I4 newData =    0b01010101010101010101010101010101;

// SOME INPUT
    new_data_or_nodes[0] = newBranch;
    new_data_or_nodes[1] = newBranch;
    new_data_or_nodes[2] = newBranch;
    new_data_or_nodes[3] = newData;
    new_data_or_nodes[4] = newData;
    new_data_or_nodes[5] = newData;
    new_data_or_nodes[6] = newData;
    new_data_or_nodes[7] = newData;
    new_data_or_nodes[8] = newData;
    new_data_or_nodes[9] = newData;
    new_data_or_nodes[10] = END_SKELETON;

// SOME INPUT TO COVER BACK THE DATA DELETED
    // new_data_or_nodes[0] = newData;
    // new_data_or_nodes[1] = newData;
    // new_data_or_nodes[2] = newData;
    // new_data_or_nodes[3] = newData;
    // new_data_or_nodes[4] = newData;
    // new_data_or_nodes[5] = END_SKELETON;

        // when no file or empty format for use
    garbage32count = garbage_check();
    filesize = (garbage32count * 4);


//  END SOME INPUT

    I4 begin = 0;
    // needed to know when to jump where
    I4 n_breaks = find_ENDs( file_buffer , ends_buffer , begin );

    // sometimes you cannot be safe
    if(int32count > 0){

        // add node to path: begin
        if( 0 ){
           int32count = add_node_or_data_to( file_buffer,
                                     int32count, 
                                     begin, 
                                     newBranch );

           save(FILENAME , file_buffer , int32count);
        }

        // find and init first uninitialized node [NODE_SKELETON] found in path: begin
        if( 0 ){
            int32count = init_node_in_path( file_buffer, 
                                            begin, 
                                            int32count,
                                            ends_buffer, 
                                            n_breaks, 
                                            new_data_or_nodes );

            save(FILENAME , file_buffer , int32count);
        }
        // begin is the index in filebuffer , filebuffer[begin]
        // get from initialized node in filebuffer, the pointed address
        begin = get_node_begin(file_buffer , begin);
        // now is possible to search something in the path beginneng from filebuffer[begin]
    
        // get content of entire path beginning from: begin
        get_path(file_buffer, path_buffer , begin );
    }
    
    // // // // // // // // // // // // // // // // // // // // // // // // // R E M O V E

    // assuming branch at 0 is initialized
    // search wht is in node position 0
    begin = 0;
    begin = get_node_begin(file_buffer , begin);

    // get ends starting from the NEW branch begin
    n_breaks = find_ENDs(file_buffer , ends_buffer , begin);

    // printing anyways
    get_path(file_buffer, path_buffer , begin );

    // tempI4 = 0;

    // if( 0 ){

    //     tempI4 = delete(file_buffer , ends_buffer[n_breaks-1] ,  7 );
    //     if( tempI4 ){
    //         garbage_sort();
    //         save(FILENAME , file_buffer , int32count);
    //     }

    // }

    // JUST FPR TESTING PORPOSES
    // tempI4 = 0;
    // //when something deleted garbage collection is to increase
    
    // if( tempI4 ){
    //     ///////////////////////////////////////////////////////
    //     garbage_get_compost( 5 );
    //     ///////////////////////////////////////////////////////
    //     garbage_turn_bin();    
    //     // !! to use if and onyly if the memory is allocated correctly!!
    //     tempI4 = garbage_memory_allocated();
    // } 


    // #free_heap
    free(file_buffer);
    free(ends_buffer);
    free(path_buffer);
    free(new_data_or_nodes);

   return EXIT_SUCCESS;
}



