
void sort_int32( I4 *data , I4 size ){

    unsigned int count = (unsigned int) size -1;
    register I4 max = 0;
    register I4 index = 0;

    while (count > 0){
        for(I4 i = 0; i < count; i++){
            if (data[i] >= max){
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