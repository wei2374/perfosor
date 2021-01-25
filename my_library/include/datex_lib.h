void prepare_phdb_request(cssl_t *serial);
void Tx_buffer(unsigned char* payload,int length,cssl_t *serial);
void CreateFrameListFromByte(unsigned char b);
void request_initialize_connection(cssl_t *serial);
void send_ack(cssl_t *serial);
void send_get_mem_request(cssl_t *serial);