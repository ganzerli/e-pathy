// ALL WHAT HAS TO DO WITH ALPHABET

struct index {
    unsigned int size;
    unsigned int index;
};

unsigned int strgs_counter = 0;

void set_strgs_counter(unsigned int n){
    strgs_counter = n;
}

unsigned int get_strgs_counter(){
    return strgs_counter;
}


void str_unset(char* str, unsigned int size){
    for (unsigned int i = 0; i < size; i++){
        str[i] = '\0';
    }
}

// rewrites in the same buffer the allowed charachters and adds 0 at the end
unsigned short str_format(char* word){
    u32 count = 0;
    u32 check = 0;
    unsigned int i = 0;
    char c = word[i];

    while(c != '\0'){
        if( (u32) c > ' ') check++;
        if( (u32) c < 127) check++;
        if(check == 2){
            word[count] = word[i];
            //printf("%c", word[count]);
            count++;
        }
        check = 0;
        i++;
        c = word[i];
    }
    word[count] = '\0';
    return count;
}

// dreaming a world where no libraries already exist, no univerities, no predefied quantities, just a list, of where to get what... where the poor is really the poor, and the other, is in one of those places.
unsigned int str_cat( char* result, char* s1 , char* s2){
    unsigned int zise = 0;
    unsigned int i = 0;

    while (s1[i] != '\0'){
        result[i] = s1[i];
        i++;
    }
    zise = i;
    i = 0;
    while (s2[i] != '\0'){
        result[zise] = s2[i];
        i++;
        zise++;
    }
    result[zise] = '\0';
    return zise;
}
// //  // //  // //  // //  // //  // //  // //  // //  // //  // // LENGT OF STRING // //  // //  // //  // //  
unsigned int str_len(char* str){
    unsigned int size = 0;
    unsigned int i = 0;
        while(str[i] != '\0'){
            size++;
            i++;
        }
    return size;
}
// //  // //  // //  // //  // //  // //  // //  // //  // //  // // COMPARE 2 STRINGS // //  // //  // //  // //  
unsigned char str_cmp(unsigned int size, char* _1 , char* _2){
    unsigned char result = 1;
    for(unsigned int i = 0; i < size; i++){
        if (_1[i] != _2[i]) return 0;
    }
    return result;
}

// //  // //  // //  // //  // //  // //  // //  // //  // //  // // PREPROCESS STRING , LOAD STRUCTS WITH INDEX AND SIZE OF SUBSTRINGS // //  // //  // //  // //  
struct index* strpreprocess(char* splitter , char* str){
    // splitter , str : size
    unsigned int spl_sz = str_len(splitter);
    unsigned int str_sz = str_len(str);
    // get first char 
    register char frc = splitter[0];    
    struct index iiddxx;
    // worst case scenario
    struct index *idx_s = malloc(sizeof iiddxx * (str_sz / 2) );
    // set o as first index
    unsigned int indextosave = 0;
    // iterators
    unsigned int i = 0;
    unsigned int structs_counter = 0;
    while ( str[i] != '\0' ){
        // chaeck if first char of splitter is found
        if(str[i] == frc){
            if( str_cmp(spl_sz , splitter , &str[i]) ) {
                iiddxx.index = indextosave;
                iiddxx.size = i - indextosave;                                                                      // size of that part is indextosave = 0; i = 99; splitter found = i 
                // set iterators
                i+= spl_sz;                                                                                         //  set i after splitter   ab[c]<,>[i]bcdef
                indextosave = i;                                                                                    // set indx after splitter ab c <,>[a]bcdef
                // load struct in array of structs
                idx_s[structs_counter++] = iiddxx;
            }
        }
        i++;
    }
    // collecting the last from last struct
    iiddxx.index = iiddxx.index + iiddxx.size + spl_sz;
    iiddxx.size = i - iiddxx.index ;
    idx_s[structs_counter++] = iiddxx;

    // memory
    idx_s = realloc(idx_s , sizeof(iiddxx)* structs_counter);
    set_strgs_counter(structs_counter);
    return idx_s;
}

// # # # # # # # # # # # # # # # # # # # # # # # # # # #     K E E P   T R A C K   O F   S T R I N G S   I N   T H E   H E A P     # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
void * strings_in_heap[4096];
unsigned int strings_in_heap_count = 0;

void track_string( void *p ){
    strings_in_heap[strings_in_heap_count] = p;
    strings_in_heap_count++;   
}

void str_free_all(){
    for (size_t i = 0; i < strings_in_heap_count; i++){
        free(strings_in_heap[i]);
    }
    strings_in_heap_count = 0;
}
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


// //  // //  // //  // //  // //  // //  // //  // //  // //  // // GET ARRAY OF POINTERS FOR STRINGS // //  // //  // //  // //  
char ** str_split(char* splitter , char* str){
    // getting struct array of data abou splitting
    struct index* idx_s;
    idx_s = strpreprocess(splitter , str);
    // knowing size of array of pointers
    unsigned int count = get_strgs_counter();
    char ** splitted_strings = malloc( sizeof (char*) * count );
    // get substring from to following index structure
    char* new_string_from(char* str , unsigned int index , unsigned int size){
        char* newstr = malloc( (sizeof (char) * size) + 1 );
        // keep count of strings on the heap
        track_string(newstr);
        for (unsigned int i = 0; i < size; i++){
            newstr[i] = str[index + i];
        }
        newstr[size] = '\0';
        return newstr;
    };
    // making a new string substringing str and saving the pointer in splitted_strings
    char *newstr;
    for (unsigned int i = 0; i < count; i++){
        newstr = new_string_from(str , idx_s[i].index, idx_s[i].size ) ;
        splitted_strings[i] = newstr;
    }
    // is already count, but just in this unix time
    set_strgs_counter(count);
    free(idx_s);
    track_string(splitted_strings);
    return splitted_strings;
}


// //  // //  // //  // //  // //  // //  // //  // //  // //  // // GPRINTING MANY STRINGS // //  // //  // //  // //  
void print_strings(char ** strings){
    printf("\n");
    unsigned int count = get_strgs_counter();
    for (unsigned int i = 0; i < count; i++){
        printf("%s\n" , strings[i] );
    }
    set_strgs_counter(0);
}

void str_cpy(char* result, char* str){
    int count = str_len(str);
    for (unsigned int i = 0; i < count; i++){
        result[i] = str[i];
    }
    result[count] = '\0';
}

void sub_str(char*bf_substr, char* bf_input , unsigned int first, unsigned int last ){
    unsigned int itr = 0;
    for (unsigned int i = first; i <= last; i++){
        bf_substr[itr] = bf_input[i];
        itr++;
    }
    bf_substr[itr] = '\0';
}
