// GARBAGE COLLECTION LIBRARY DOES:
// creating a bin file named thrash, and the fun stuff is that the file is a bin
// saving integers in this file.
// sorting the integers ascending
// having a quantity check if a series of those ints covers the need
// updates the scavage struct with quantity, starting index in garbage file, and starting index for other file
// updates the thrash file

// GLOBAL VARIABLES
u32 garbage32count = 0;

struct scavage{
    u32 index_data_file;
    u32 index_garbage_file;
    u32 count;
}scavaging;

#define GARBAGE "thrash"
#define THRASH_BUFFER_SIZE 256 * sizeof (u32)

////////////////////////////////////////////////////////////////////////////////        CHECK GARBAGE FILE       ////
// CHECKS IF FILE EXISTS, AND RETURNS NUMBER OF 32BITS INT IN THE FILE
u32 garbage_check(){
    printf("\n\n    Checking garbage file   ....\n");
    u32 ints_count = 0;                     
    u32 bytes_count = 0;
    FILE *fp;
    // check in file how many u32 there are
    fp=fopen(GARBAGE,"ab");                                            
    fseek(fp, 0, SEEK_END);
    bytes_count = ftell(fp);
    fclose(fp);
    
    ints_count = bytes_count / 4;

    printf("\nGARBAGE:    There are currently %u 4-Bytes INTEGERS in the file" , ints_count);
    printf("\nFilesize %u Bytes\n" , bytes_count);
    return ints_count;
}

////////////////////////////////////////////////////////////////////////////////        TAKE TRASH BIN AND TURN IT UPSIDE DOWN       ////
void garbage_turn_bin(){
    printf("\n\n  Turning the trhash bin...\n");
    const u32 zise = garbage32count;
    FILE *fp=fopen(GARBAGE,"rb");      
    u32 current_integer = 0;
    // print content of file
    for (u32 i = 0; i < zise; i++){
        fread(&current_integer, sizeof (u32), 1, fp);
        printf("%u ", current_integer);
    }
    printf("\n");
    fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////        DROP DATA IN THE THRASH      ////
u32 garbage_drop_in_trash(u32 index){
    printf("\n\n    DROPPING IN THRASH  \n");
    FILE *fp;
    u32 idnex = index;
    // write new index in garbage collection file
    fp=fopen(GARBAGE,"ab");
    if (fp == NULL) return 0;

    fwrite(&idnex, sizeof (u32), 1, fp); 
    fclose(fp);

    printf("\n written in garbage collector: %u" , idnex);
    garbage_turn_bin();
    return 1;
}

////////////////////////////////////////////////////////////////////////////////       GET SIZE AND POSITION OF DELETED ITEMS       ////
void garbage_get_compost( u32 size_needed ){

    printf("\n\n    GARBAGE CHECK FOR FREE SPACE\n");
    const u32 u32count = garbage32count;
    u32 size_found = 1;                                          // size needed will be at least 2 [u32][END], more is better memory and speed..
    u32 current_integer = 0;
    u32 last_integer = SOMETHING_WENT_WRONG - 1;
    // reset
    scavaging.index_data_file = 0;
    scavaging.index_garbage_file = 0;
    scavaging.count = 0;
    
    FILE *fp =fopen(GARBAGE,"rb");
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", GARBAGE, __FILE__, __LINE__ - 1);

    // having the file sorted..
    for (u32 i = 0; i < u32count; i++){
        // collecting a current_integer from file
        fread(&current_integer, sizeof (u32), 1, fp);
        // check if the last 2 integer are in serie ...[70][79]85] [98][99]..
        if(current_integer == last_integer +1){
            size_found++;
        }else{
            size_found = 1;
        }
        // remember current integer for next loop
        last_integer = current_integer;

        if(size_found == size_needed){
            // case last integers    [10] [11] [12] [13] 
            // size needed 4         [ 1] [ 2] [ 3] [ 4]
            // i = example           [ 0] [ 1] [ 2] [ 3]
            scavaging.index_data_file = current_integer - (size_needed-1);                          // i starts from 0
            scavaging.index_garbage_file = i - (size_needed-1);                                     // and size needed starts from 1
            scavaging.count = size_needed;

            i = u32count;
            break;
        }
    }
    fclose(fp);
    printf("\nindex for data: %u \nindex in garbage file: %u \ncount: %u \n" , scavaging.index_data_file , scavaging.index_garbage_file , scavaging.count);
}

////////////////////////////////////////////////////////////////////////////////       DELETE REUSED-ALLOCATED MEMORY FOR GARBAGE COLLECTION     ////
u32 garbage_memory_allocated(){

    u32 const counz = scavaging.count;                                 
    u32 const index = scavaging.index_garbage_file;
    printf("\n\n    UPDATE GARBAGE COLLECTON FILE\n");

    FILE *fp = fopen(GARBAGE,"rb");
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", GARBAGE, __FILE__, __LINE__ - 1);

    u32 *empty_trash_bin = malloc( THRASH_BUFFER_SIZE );
    u32 current_integer = 0;
    u32 i = 0;

    for ( i = 0; i < garbage32count; i++){
        fread(&current_integer , sizeof(u32) , 1 , fp);
        empty_trash_bin[i] = current_integer;
    }
    fclose(fp);

    printf("\nIntegers remaining in garbage collection file\n");
    fp = fopen(GARBAGE,"wb");
    // collect everything until [<-][<-][<-][<-] [index]
    for ( i = 0; i < index; i++ ){
        printf("(%u) " , empty_trash_bin[i]);
        fwrite(&empty_trash_bin[i], sizeof (u32), 1, fp); 
    }
    // collect everything after i+count [0] [1] [2] [3] [4] [5] [6] [7]
    //        i                         [i][+1][+2][+3]     
    //        count                     [1] [2] [3] [4]
    for ( i = index + counz; i < garbage32count; i++ ){
        printf("(%u)" , empty_trash_bin[i]);
        fwrite(&empty_trash_bin[i], sizeof (u32), 1, fp); 
    }
    fclose(fp);
    free(empty_trash_bin);

    garbage32count -= counz;
    printf("\nCount of u32 in garbage collection file: %u \n" , garbage32count);
    return garbage32count;
}

////////////////////////////////////////////////////////////////////////////////       SORT GARBAGE FILE       ////
u32 garbage_sort(){

    const u32 size = garbage32count;
    printf("\n\n    GARBAGE SORTING  -> %u elements \n" , size);
    if(size == 0)  return 0;                                                    

    u32 *empty_trash_bin = malloc( THRASH_BUFFER_SIZE );
    u32 current_integer = 0;
    FILE *fp = fopen(GARBAGE,"rb");
    if(fp == NULL) return 0;

    // LOAD FILE
    for (u32 i = 0; i < size; i++){
        fread(&current_integer, sizeof (u32), 1, fp);
        empty_trash_bin[i] = current_integer;
    }
    fclose(fp);

    // SORTING
    sort_int32(empty_trash_bin , size);

    // WRITE BACK
    fp = fopen(GARBAGE ,"wb");
    for (u32 i = 0; i < size; i++){
        current_integer = empty_trash_bin[i];
        fwrite(&current_integer, sizeof (u32), 1, fp);

        printf("\n sorted : %u" , current_integer);
    }
    fclose(fp);

    // returning higest number
    u32 max = empty_trash_bin[size];
    free(empty_trash_bin);
    return max;
}