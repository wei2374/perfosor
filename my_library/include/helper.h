void print_array(unsigned char* array, int length);
void string2ByteArray(char* input, unsigned int* output);
void copy_array(unsigned char* dest, unsigned char* src,int length);
int compute_checksum(unsigned char* bytes);
void int_save_to_buffer(int integer,unsigned char* bytes);
void insert_element(unsigned char* bytes, int index, char element);