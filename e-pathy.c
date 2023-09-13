#include <stdio.h>
#include <stdlib.h>

#define FILENAME "file"
#define _1GB 1073741823

#define SOMETHING_WENT_WRONG    0b11111111111111111111111111111111
#define NODE_SKELETON           0b11000000000000000000000000000000
#define DATA_SKELETON           0b01000000000000000000000000000000
#define END_SKELETON            0b00000000000000000000000000000000

 // 4 bytes integer
typedef u_int32_t  I4;                                                                             

////////////////////////////////////////////////////////////////////////////////        IS END       ////
I4 is_END(I4 num){
   return  ( 0b00111111111111111111111111111111 | num ) == 0b00111111111111111111111111111111;
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

// void print_whole_file(char* filepath , I4 count){
//     I4 curren_integer = 0;
//     FILE *fp =fopen(FILENAME,"rb");
//     // check if everything good to create or open the file
//     if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", FILENAME, __FILE__, __LINE__ - 1);
//     // print everything
//     for (size_t i = 0; i < (size_t)count; i++){
//         fread(&curren_integer, sizeof (I4), 1, fp);
//         ////////////////////////////////
//         printf("%u ", curren_integer);//
//         ////////////////////////////////
//     }
//     fclose(fp);
//     printf("\n");
// }


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////     USEFUL FUNCTIONS FOR AN OUTER LIBRARY       ////////////////////////////////////////////////////

I4 *sort_int32( I4 *data , size_t size ){

    unsigned int count = (unsigned int) size - 1;

    register I4 max = 0;
    register I4 index = 0;

    while (count > 0){

        for(I4 i = 0; i < count; i++){
        
            if (data[i] > max){
                max = data[i];
                index = i;
            }
        }
        
        data[index] = data[count];
        index = 0;
        data[count] = max;
        max = 0;

        count --;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
I4 find_ENDs(I4* endsbuffer , I4 path_begin , I4* filebuffer){
    I4 ends = 0;
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
                end = path_begin + i;
                endsbuffer[ends] = end;
                printf("ends buffer[%u] = %u\n",ends, endsbuffer[ends]);
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
I4 add_node_to(I4* filebuffer , I4 limit, I4 path_begin , I4 new_node){

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

                    //in check is the free position
                    filebuffer[i] = new_node;
                    printf("[new node] %u , is written at filebuffer[%u]\n", new_node , i);

                    // if position is current end of file pad the end, in other case is anyway END_SKELETON.. until now is better then a condition(poor registers!!)
                    filebuffer[i+1] = END_SKELETON;
                    printf("[END_SKELETON] placed at filebuffer[%u]\n", i+1);

                    limit ++;
                    stop = 1;
                    break;
                }else{
                    // require memory
                    available_memory_at = require_memory(1,limit);
                    printf(" memory available at [%u]\n", available_memory_at );

                    // give to the end the pointer for that position
                    filebuffer[i] = END_SKELETON + available_memory_at;
                    printf(" filebuffer[%u] END now pointing to--> [%u]\n", i, available_memory_at );
                    printf("[%u] now is -> %u\n",i, filebuffer[i] );

                    // set new node at available memory
                    filebuffer[available_memory_at] = new_node;
                    limit ++;
                    printf(" new_node[%u] written in filebuffer[%u]\n", new_node, (available_memory_at) );

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

                ////////////////////////////////////////////////////////// to review
                if (END_points_to > END_SKELETON){
                    i = END_points_to;
                    printf("[%u] -> %u \n",i, filebuffer[i]);
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
    printf("ENDs[ends] = %u \n" , ENDs[ends]);
    while(i < ENDs[ends]){

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
            printf("\nNOT ANY uninitialized node found: NODE_SKELETON in path: /%u not foud, you need to add_node_to -> %u ", path_begin , path_begin);
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

    printf(" requiring %u of memory \n", count);
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

    I4 newBranch = NODE_SKELETON ;
//  SOME INPUT
    new_data_or_nodes[0] = newBranch;
    new_data_or_nodes[1] = newBranch;
    new_data_or_nodes[2] = newBranch;
    new_data_or_nodes[3] = END_SKELETON;
    I4 new_data_or_nodes_size = 4;
//  END SOME INPUT

    I4 begin = 0;

    // needed to know when to jump where
    I4 n_breaks = find_ENDs(ends_buffer , begin , file_buffer);

    // sometimes you cannot be safe
    if(int32count > 0){

        // add node to path: begin
        if( 1 ){
           int32count = add_node_to( file_buffer,
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

    // #free_heap
    free(file_buffer);
    free(ends_buffer);
    free(path_buffer);
    free(new_data_or_nodes);

    
   // remove
   
   //   if is node message you need to delete single data
   //   get begin of node
   //   find [data to delete]
   //   run to end of branch
   //   copy the last data or node in [data to delete]
   //   cover with an END_SKELETON
   //   write the last END_SKELETON in garbage collector file
   //   exiting the program order garbage collection file

   // adding a node or data again in that branch will just take the place back with if_space_after
   // if there are more then 2 free places after one other the garbage collector can assign memory there.. in the case


    I4 sorting [] = {12,3,4,3,5,4,24,9,8,7,6,5,4,3,2,1,0,14,6,66,8,79,8,81,6,5,7,3,6,8,3,2,2,4,6,8,4,2,2,5,7,8,9,9,4,2,5,5,7,6,8,4,
    212,98,8457,58789,5,6,8793,5176562,5745,7,9,46,24557,58,9456,26,5,74585,6,6565,6,385,8,56,3748,4562345,249,58078468,6256,5756846,
    51,4526,48,4747,245647,6545,627,458,56,4,5152,5,45,6,7,5,45,4526,7,457,68,45,61,2,52,462,45,74,475,2,9847,5,2874,594,8,6720641,7,
    4524,579,684,5,79865513,586,50,702360,65,9702,36,5230,516,298749,87,6,2451,98,4756,498,72,8246,207117,4607,4962,4651,265,9,82641865,
    14,6529,87,6965194,659,26,4641962,98,65,298,652,98652,46,29,486,52,86,526,59,84,65,9,865165,92,86,598,65,62,984,7519,874,70,23652,
    315,162,987,4987,6,245,198,475,649,8728,24,620,7017,460,7496,24,65026,598,264189,84752,984,75,28,745,9486,720,641,74,524,5796,845,
    798,655135,86517123,606,5965,146,52,98,7696,509,465,926,464,962,986,529,8652,98,652,462,94,865,28,65,2,65,984,659,86516,59,286,59,
    865,62,98475,9,874,98,47,5,29,8475,2874,594,867,2064,17,45,24,57,968,45,798,65,503,58,650,702,365,9264,640,962,986,529,8652,986,52,
    462,948,652,86,5265,984,659,8651,6592,86,59,865,6298,47,5098,746165,971236,52,305,162,987,498,762,45,19,847,56,498,7282,46,207117,
    460,74,96,24,6502,65,98,26,408,651,46529,876,96519,4,98,4,56,4,52,7,698,0,456,62,4646,469,6,2,6,457,79,680,8,67,541,524,7459,7858};


    size_t zise = sizeof sorting / sizeof (I4);
    sort_int32(sorting , zise);
    
    for( size_t i = 0; i < zise; i++ ){
        printf("%u," , sorting[i]);
    }


   return EXIT_SUCCESS;
}