#include <stdio.h>
#include <string.h>
#include <unistd.h> // write(), read(), close()
#include "cssl.h"
#include "datex_lib.h"

static void process_buffer(int id, uint8_t *buf, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf(" %02X", buf[i]);
        CreateFrameListFromByte(buf[i]);
    }

    fflush(stdout);
}

void main()
{
    cssl_t *serial;
    cssl_start();
    serial = cssl_open("/dev/ttyUSB0",
                       process_buffer,
                       0,
                       9600,
                       8,
                       0,
                       1);

    if (!serial)
    {
        printf("%s\n", cssl_geterrormsg());
    }

    // unsigned char *prepared_msg;

    char test_buf[48] = {0x01, 0x30 , 0x30 , 0x30 , 0x34 , 0x36 , 0x02 , 0x53 , 0x70 , 0x61 , 0x63 , 0x65 , 0x65 , 0x43 , 0x6F , 0x6D , 0x39 , 0x2F , 0x31 , 0x2F , 0x31 
    ,0x3E , 0x31 ,0x39 ,0x34 ,0x2C ,0x35 ,0x30 ,0x31 ,0x30 ,0x30 ,0x2C ,0x47 ,0x4E ,0x45 ,0x45 ,0x52 ,0x52 ,0x2C ,0x36 ,0x38 ,0x03 ,0x30 ,0x30 ,0x30 ,0x37 ,0x30 ,0x04};

    process_buffer(1,test_buf,48);
    /*
    request_initialize_connection(serial);
    while(1){
        send_get_mem_request(serial);
        sleep(5);    
    }
    */
    cssl_close(serial);
    cssl_stop();
}