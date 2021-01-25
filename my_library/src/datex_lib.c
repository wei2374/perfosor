
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cssl.h"
#include "helper.h"
#include "constants.h"
#include "cvector.h"

bool m_storestart = false;
bool m_storeend = false;
cvector_vector_type(char) frameBuffer = NULL;


void CreateFrameListFromByte(unsigned char b){
    switch(b){
        case SOHCHAR:
            m_storestart = true;
            m_storeend = false;
            cvector_push_back(frameBuffer, b);
            break;
        case EOTCHAR:
            m_storestart = false;
            m_storeend = true;
            break;
        case ACKCHAR:
            break;
        case NAKCHAR:
            break;
        default:
            if(m_storestart == true && m_storeend == false)
                cvector_push_back(frameBuffer, b);
            break;
    }


    // character debuffer
    if(m_storestart == false && m_storeend == true){
        for(int i=0;i<cvector_size(frameBuffer);i++){
            unsigned char bchar = frameBuffer[i];
            switch(bchar){
                case ECHAR:
                    if(frameBuffer[i+1] == XCHAR){
                        cvector_erase(frameBuffer,i+1);
                        frameBuffer[i] == DCHAR;
                    }
                    else if(frameBuffer[i+1] == ECHAR){
                        cvector_erase(frameBuffer,i+1);
                        frameBuffer[i] == ECHAR;
                    }
                    break;
                
                case eCHAR:
                    if(frameBuffer[i+1] == xCHAR){
                        cvector_erase(frameBuffer,i+1);
                        frameBuffer[i] == dCHAR;
                    }
                    else if(frameBuffer[i+1] == eCHAR){
                        cvector_erase(frameBuffer,i+1);
                        frameBuffer[i] == eCHAR;
                    }
                    break;
            }
        }
    

        int framelen = cvector_size(frameBuffer);
        //print_array(frameBuffer,cvector_size(frameBuffer));
                
        if(framelen!=0){
            cvector_vector_type(char) frameBuffer2 = NULL;
            cvector_copy(frameBuffer, frameBuffer2);
            char checksum_buf[5];
            int checksum_rece=0;
            
            //get away the checksum
            for(int n=0;n<5;n++){
                checksum_buf[n] = frameBuffer2[framelen-1-n];
                cvector_pop_back(frameBuffer2);
            }

            for(int n=0;n<5;n++){
                if(checksum_buf[n]==0)
                    break;
                else
                    checksum_rece += ((int)checksum_buf[n]-48)*(myPow(10,n));
            }
            int checksum_computed = compute_checksum(frameBuffer2);
            
            printf("\nthe rece_checksum is %d", checksum_rece);
            printf("\nthe calc_checksum is %d", checksum_computed);
            
            if(checksum_rece == checksum_computed){
                printf("correct checksum");
            }
        }
    }
}
int myPow(int x,int n)
{
    int i; /* Variable used in loop counter */
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}

/*
    //if get a byte which indicates the start of a msg
    if(b==0x7e && m_fstart){
        m_fstart = false;
        m_storestart = true;
        //printf("start");
    }
    //encounter the end
    else if(b==0x7e && !m_fstart){
        m_fstart = true;
        m_storestart = false;
        m_storeend = true;
        //printf("end");
    }
   
    
    if(m_storestart){
        //encounter control byte
        if(b==0x7d){
            m_bitschiftnext = true;
        }
        //normal byte
        else{
            //the byte before is a control byte
            if(m_bitschiftnext){
                m_bitschiftnext = false;
                b |=0x7c;
                m_bList[counter1]=b;
                counter1++;
            }
            else if(b!=0x7e){
                m_bList[counter1]=b;
                counter1++;
            }
        }
    }

    //end of msg
    else if(m_storeend){
        if(counter1!=0){
            byte checksum=0x00;
            for(int i=0;i<counter1-1;i++){
                checksum+=m_bList[i];
            }
            if(checksum==m_bList[counter1-1]){
                printf("correct");
                copy_array(FrameList[array_counter], m_bList,counter1);
                //print_array(m_bList,50);
               
                //print_array(FrameList[array_counter],50);
                array_counter++;
            }

            m_storeend=false;
        }
        else{
            m_storestart = true;
            m_storeend = false;
            m_fstart = false;
        }
    }
    
}
/*
void CreateRecordList(byte** FrameList,int length){
    struct datex_record** record_array;
    record_array = malloc(length*sizeof(struct datex_struct*));

    for(int i=0;i<length;i++){
        struct datex_record datex_dtx;
        record_array[i] = (struct datex_struct*)FrameList[i];
    }
}


void Tx_buffer(unsigned char* payload,int length,cssl_t *serial){
    byte checksum=0;
    byte* temptxbuff = malloc(length*2);
    //printf("length is %d\n",length);   
    memset(temptxbuff,0x00,length*2);

    int len =  length;
    temptxbuff[0] = 0x7e;
    int counter1=1;
    
    for(int i=0;i<len;i++){
        switch(payload[i])
        {
            case(0x7e):
                temptxbuff[counter1] = 0x7d;
                temptxbuff[counter1+1] = 0x5e;
                counter1+=2;

                checksum+=0x7d;
                checksum+=0x5e;
                break;
                
            case(0x7d):
                temptxbuff[counter1] = 0x7d;
                temptxbuff[counter1+1] = 0x5d;
                counter1+=2;
                
                checksum+=0x7d;
                checksum+=0x5d;
                
                break;

            default:
                temptxbuff[counter1] = payload[i];
                counter1++;
                checksum+=payload[i];
                break;
        }
    }

    int buflen = counter1;
    byte* finaltxbuff = malloc(buflen+2);
    memset(finaltxbuff,0x00,buflen+2);

    for(int j=0;j<buflen;j++){
        finaltxbuff[j] = temptxbuff[j];
    }
    switch(checksum){
        case 0x7e:
            finaltxbuff[buflen] = checksum;
            finaltxbuff[buflen+1] = 0x5e;
            buflen++;
            break;
        case 0x7d:
            finaltxbuff[buflen] = checksum;
            finaltxbuff[buflen+1] = 0x5d;
            buflen++;
            break;
        default:
            finaltxbuff[buflen] = checksum;
    }
    
    //finaltxbuff[buflen] = checksum;
    finaltxbuff[buflen+1] = 0x7e;
    payload = finaltxbuff;
    length = buflen+2;
    
    print_array(finaltxbuff,length);
    //for(int i=0;i<length;i++){
    //    cssl_putchar(serial,finaltxbuff[i]);
    //}
    
    cssl_putdata(serial, finaltxbuff, length);
}
*/

void WriteRequestBuffer(char bedid[], char txbuf[], int len1, int len2, cssl_t *serial){

    cvector_vector_type(char) temptxbuff = NULL;
    cvector_push_back(temptxbuff, SOHCHAR);
    //deal with length
    int totalframelen = 15 + len1 + len2;
    int_save_to_buffer(totalframelen, temptxbuff);
    cvector_push_back(temptxbuff, STXCHAR);

    for(int i=0;i<len1;i++){
        cvector_push_back(temptxbuff, bedid[i]);
    } 
    cvector_push_back(temptxbuff,  '>');
    
    for(int i=0;i<len2;i++){
        cvector_push_back(temptxbuff, txbuf[i]);
    }
    cvector_push_back(temptxbuff, ETXCHAR);

    int checksum = compute_checksum(temptxbuff);
    int_save_to_buffer(checksum, temptxbuff);
    cvector_push_back(temptxbuff, EOTCHAR);
    
    // do character stuffing
    for (int i=0; i<cvector_size(temptxbuff); i++)
        {
            char bchar = temptxbuff[i];
            //printf("\n char %c",bchar);
            switch(bchar)
            {
                case DCHAR: //D->EX
                    temptxbuff[i] = ECHAR;
                    insert_element(temptxbuff, i+1, XCHAR);
                    i=i+1;
                    break;
                case ECHAR: //E->EE
                    insert_element(temptxbuff, i+1, ECHAR);
                    i=i+1;
                    break;
                case dCHAR: //d->ex
                    temptxbuff[i] = eCHAR;
                    insert_element(temptxbuff, i+1, xCHAR);
                    i=i+1;
                    break;
                case eCHAR: //e->ee
                    insert_element(temptxbuff, i+1, eCHAR);
                    i=i+1;
                    break;
                default:
                    break;
            }
        }

    printf("\n");
    for(int i=0;i<cvector_size(temptxbuff);i++){
        printf(" %d",temptxbuff[i]);
    }
    printf("\n");
    
    cssl_putdata(serial, temptxbuff, cvector_size(temptxbuff));
}

/**
 * with this request the host system initialize the connection to ComDevice
*/
void request_initialize_connection(cssl_t *serial){
    char bedid[] = "1/1/1";
    char command[] = "ADMIN:ALIVE";
    int len1 = 5;
    int len2 = 11;
    WriteRequestBuffer(bedid, command, len1, len2, serial);
}

/**
 * with this request the host asks for all actual parameter values of the ComDevice / pumps
 */
void send_get_mem_request(cssl_t *serial){
    char bedid[] = "SpaceCom9/1/1";
    char command[] = "MEM:GET";
    int len1 = 13;
    int len2 = 7;
    WriteRequestBuffer(bedid, command, len1, len2, serial);
}

/**
 * send ACK back
 */
void send_ack(cssl_t *serial)
{
    sleep(0.1);
    cvector_vector_type(char) temptxbuff = NULL;
    cvector_push_back(temptxbuff, ACKCHAR);
    cssl_putdata(serial, temptxbuff, cvector_size(temptxbuff));
}
