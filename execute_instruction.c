
// getting instruction from relacy, execute it and send result

unsigned int execute_instruction(char* reqbf , unsigned int size){

    unsigned int response_size = 0;

    reqbf[size-1] = '\0';

    printf( "%s" , reqbf );

    response_size = size;

    return response_size;
}
