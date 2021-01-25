#include <stdio.h>
#include <cvector.h>

void print_array(unsigned char* array, int length){
    printf("\n");
    
    for(int i=0;i<length;i++){
        printf(" %02X",array[i]);

        if((i+1)%16==0){
            printf("\n");
        }
    }
    printf("\n");
}

void copy_array(unsigned char* dest, unsigned char* src,int length){
    for(int i=0;i<length;i++){
        dest[i]=src[i];
    }
}

void string2ByteArray(char* input, unsigned int* output)
{
    int loop;
    int i;
    
    loop = 0;
    i = 0;
    
    while(input[loop] != '\0')
    {
        output[i++] = input[loop++];
    }
}

int compute_checksum(unsigned char* bytes)
{
    int sum = 0;
    int crc = 0;

    for (int i = 0; i < cvector_size(bytes); i++)
    {
        sum += bytes[i];
    }
    printf("sum is %d \n",sum);

    crc = (unsigned char)(sum % 256);
    return crc;
}

void insert_element(unsigned char* bytes, int index, char element){
    int end_index = 0;
    for(;bytes[end_index]!='\0';end_index++)
    {}
     
    cvector_push_back(bytes, bytes[end_index-1]);  
    for(int i=end_index-1; i>index; i--){
        bytes[i] = bytes[i-1];     
    }
    bytes[index] = element;
}

void int_save_to_buffer(int integer, unsigned char* bytes){
    char length_buffer[5] = "0";
    sprintf(length_buffer,"%d",integer);
    int end_index = 0;
    for(end_index=0;length_buffer[end_index]!='\0';end_index++)
    {}
    for(int i=0;i<5-end_index;i++){
        cvector_push_back(bytes, '0');
    }
    for(int i=5;i>5-end_index;i--){
        cvector_push_back(bytes, length_buffer[5-i]);
    }
}