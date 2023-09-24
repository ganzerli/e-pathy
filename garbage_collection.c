// GLOBAL VARIABLE FOR GARBAGE COLLECTION LIBRARY
I4 garbage32count = 0;
#define GARBAGE "thrash"

struct scavage{
    I4 index;
    I4 count;
}scavaging;

////////////////////////////////////////////////////////////////////////////////        CHECK GARBAGE FILE       ////
// CHECKS IF FILE EXISTS, AND RETURNS NUMBER OF 32BITS INT IN THE FILE
I4 garbage_check(){

    printf("\n\n    Checking garbage file   ....");

    I4 ints_count = 0;                     
    I4 bytes_count = 0;
    FILE *fp;

    // check in file how much I4 there are
    fp=fopen(GARBAGE,"ab");                                            
    fseek(fp, 0, SEEK_END);
    bytes_count = ftell(fp);
    fclose(fp);
    
    ints_count = bytes_count / 4;

    printf("\nGARBAGE:    There are currently %u 4-Bytes INTEGERS in the file" , ints_count);
    printf("\nFilesize %u Bytes" , bytes_count);

    return ints_count;
}

////////////////////////////////////////////////////////////////////////////////        DROP DATA IN THE THRASH      ////
I4 garbage_drop_in_trash(I4 index){

    printf("\n\n    DROPPING IN THRASH  ");
    FILE *fp;
    I4 idnex = index;

    // write new index in garbage collection file
    fp=fopen(GARBAGE,"ab");
    if (fp == NULL) return 0;

    fwrite(&idnex, sizeof (I4), 1, fp); 
    printf("\n written in garbage collector: %u" , idnex);
    fclose(fp);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////        TAKE TRASH BIN AND TURN IT UPSIDE DOWN       ////
void garbage_turn_bin(I4 zise){

    printf("\n\n  Turning the trhash bin...\n");

    FILE *fp=fopen(GARBAGE,"rb");      
    I4 current_integer = 0;

    for (I4 i = 0; i < zise; i++){
        fread(&current_integer, sizeof (I4), 1, fp);
        printf("%u ", current_integer);
    }
    printf("\n");
    fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////       GET SIZE AND POSITION OF DELETED ITEMS       ////
void garbage_get_compost(struct scavage * scavaging , I4 size_needed){

    // !!!!!!!!!!!! size needed is without [END]! in scavaging MUST be coun > 0 only if FOUND is COUNT+1 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("\n\n    GARBAGE CHECK FOR FREE SPACE\n");

    const I4 I4count = garbage32count;

    I4 size_found = 1;                                          // size needed will be at least 2 [I4][END], more is better memory and speed..
    I4 current_integer = 0;
    I4 last_integer = SOMETHING_WENT_WRONG - 1;

    scavaging->index = 0;
    scavaging->count = 0;
    
    FILE *fp =fopen(GARBAGE,"rb");
    
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", GARBAGE, __FILE__, __LINE__ - 1);

    // having the file sorted..
    for (I4 i = 0; i < I4count; i++){

        // collecting a current_integer from file
        fread(&current_integer, sizeof (I4), 1, fp);
        
        // check if the last 2 integer are in serie ...[70][79]85] [98][99]..
        if(current_integer == last_integer +1){
            size_found++;
        }else{
            size_found = 1;
        }

        // remember current integer for next loop
        last_integer = current_integer;

        if(size_found > size_needed){
            // case ... [10][11][12][13] > [14]
            // size needed 4;
            //          [1] [2] [3] [4] [+1 for END] (thats why > and not ==)
            // i =      [ 0][ 1][ 2][ 3][ 4]
            scavaging->index = current_integer - size_needed;
            scavaging->count = size_needed;

            i = I4count;
            break;
        }
    }
    fclose(fp);
    printf("\nindex: %u \ncount: %u \n" , scavaging->index , scavaging->count);
}

////////////////////////////////////////////////////////////////////////////////       DELETE REUSED-ALLOCATED MEMORY FOR GARBAGE COLLECTION     ////
I4 garbage_memory_allocated(I4 index , I4 count){
    // in garbage get compost is allocated count only if count + 1 found
    // the program will ask for amount of memory only the functions will keep count of the [END] needed allocating memory
    // count need to be incremented considerating count and also [END]! [c][o][u][n][t][END]
    I4 counz = count + 1;                                  // always remember the need of [END]

    printf("\n\n    UPDATE GARBAGE COLLECTON FILE\n");

    FILE *fp =fopen(GARBAGE,"rb");
    if(fp == NULL)  fprintf(stderr, "error opening file: %s #RUNNING %s at line # %d\n", GARBAGE, __FILE__, __LINE__ - 1);

    I4 *empty_trash_bin = malloc(256 * sizeof (I4));
    I4 current_integer = 0;

    for (I4 i = 0; i < garbage32count; i++){
        fread(&current_integer , sizeof(I4) , 1 , fp);
        empty_trash_bin[i] = current_integer;
    }

    I4 i = 0;
    printf("\nIntegers remaining in garbage collection file\n");
    // collect everything until [<-][<-][<-][<-] [index]
    for ( i = 0; i < index; i++ ){
        printf("(%u) " , empty_trash_bin[i]);
    }
    // collect everything after i+count [0] [1] [2] [3] [4] [5] [6] [7]
    //        i                         [i][+1][+2][+3]     
    //        count                     [1] [2] [3] [4]
    for ( i = index + counz; i < garbage32count; i++ ){
        printf("(%u)" , empty_trash_bin[i]);
    }


    free(empty_trash_bin);
    garbage32count -= counz;

    printf("\nCount of I4 in garbage collection file: %u \n" , garbage32count);
    return garbage32count;
}

////////////////////////////////////////////////////////////////////////////////       SORT GARBAGE FILE       ////
I4 garbage_sort(I4 size){

    printf("\n\n    GARBAGE SORTING  -> %u elements \n" , size);
    if(size == 0)  return 0;                                                    

    I4 *empty_trash_bin = malloc(256 * sizeof (I4));
    I4 current_integer = 0;
    FILE *fp = fopen(GARBAGE,"rb");
    
    if(fp == NULL) return 0;

    // LOAD FILE
    for (I4 i = 0; i < size; i++){
        fread(&current_integer, sizeof (I4), 1, fp);
        empty_trash_bin[i] = current_integer;
    }
    fclose(fp);

    // SORTING
    sort_int32(empty_trash_bin , size);

    // WRITE BACK
    fp = fopen(GARBAGE ,"wb");
    for (I4 i = 0; i < size; i++){
        current_integer = empty_trash_bin[i];
        fwrite(&current_integer, sizeof (I4), 1, fp);

        printf("\n sorted : %u" , current_integer);
    }
    fclose(fp);

    I4 max = empty_trash_bin[size];
    free(empty_trash_bin);

    return max;
}
