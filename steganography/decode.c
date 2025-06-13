#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "color.h"



Status do_decoding(DecodeInfo *decInfo)
{
    if(decode_open_files(decInfo) == e_success){
        printf("INFO: Done.Opened all recquired files\n");
        if(decode_magic_string(decInfo) == e_success){
            printf("INFO: Decoding Magic String Signature\n");
            printf("INFO: Done\n");
            if(decode_secret_file_extn_size(decInfo) == e_success){
                printf("INFO: Decoding Output File Extenstion Size\n");
                printf("INFO: Done\n");
                if(decode_secret_file_extn(decInfo) == e_success){
                    printf("INFO: Decoding Output File Extenstion\n");
                    printf("INFO: Done\n");
                    if(decode_secret_file_data_size(decInfo) == e_success){
                        printf("INFO: Decoding file size\n");
                        printf("INFO: Done\n");
                        if(decode_secret_file_data(decInfo) == e_success){
                            printf("INFO: Decoding file data\n");
                            printf("INFO: Done\n");

                            return e_success;
                        }
                        else{
                            return e_failure;
                        }
                    }
                    else{

                    }
                }
                else{

                }
            }
            else{

            }
        }
        else{
            printf(RED"Error : Magic String is not matched !!!"RESET"\n");
        }
    }
    else{

    }

    //return e_success;
}

Status decode_open_files(DecodeInfo *decInfo)
{
    //source image file
    decInfo->fptr_encoded_image = fopen(decInfo->encoded_image,"r");

    //Error Handling
    if(decInfo->fptr_encoded_image == NULL)
    {
        perror("fopen");
        printf("ERROR: Unable to open file %s\n",decInfo->encoded_image);

        return e_failure;
    }

    return e_success;
}

/*the below function used to decode the magic string from the encoded image*/
Status decode_magic_string(DecodeInfo *decInfo){
    char str[20];
    printf(CYAN"Enter the Magic String:"RESET);
    scanf("%s",str); 
    getchar();                                              //reading the magic string from the user
    
    char buf[strlen(MAGIC_STRING)];
    
    char img_buf[8];
    fseek(decInfo->fptr_encoded_image,54,SEEK_SET);                 //changing the file pointer from 0th index to 54 because magic string is present in 54th byte
    for(int i=0;i<strlen(MAGIC_STRING);i++){
        fread(img_buf,8,1,decInfo->fptr_encoded_image);               //reading 8 byte from encoded image
        decode_byte_to_lsb(buf,i,img_buf);                          //function call to decode lsb from each byte and store it in a char buffer
    }
    
    buf[strlen(MAGIC_STRING)]='\0';    // adding null character to the magic string       
    printf("%s\n",buf);
    if(strcmp(buf,str)==0) 
         return e_success;                        // validating whether the user's i/p and magic string are matching or not
    else return e_failure;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer,32,1,decInfo->fptr_encoded_image);
    decode_size_to_lsb(&decInfo->size,buffer);
    printf("--->%d\n",decInfo->size);
    //fwrite(buffer,32,1,decInfo->fptr_secret);

    return e_success;
} 

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char extn[8];
    //printf("--->%d\n",decInfo->size);
    for(int i=0; i<decInfo->size;i++)
    {
        fread(extn,8,1,decInfo->fptr_encoded_image);
        decode_byte_to_lsb(decInfo->extn_secret_file,i,extn);
    }
    
    strcat(decInfo->txt_name,decInfo->extn_secret_file);
    // FILE *txt_fptr = fopen(decInfo->txt_name,"w");
    // printf("%s--\n",decInfo->txt_name);
    return e_success;
}

Status decode_secret_file_data_size(DecodeInfo *decInfo)
{
   char size_buff[32];
   fread(size_buff,32,1,decInfo->fptr_encoded_image);
   decode_size_to_lsb(&decInfo->size_secret_file,size_buff);
   //printf("%d\n",decInfo->size_secret_file);
   return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char file_buff[8];    
    for(int i=0;i<decInfo->size_secret_file;i++)
    {
        fread(file_buff,1,8,decInfo->fptr_encoded_image);
        decode_byte_to_lsb(decInfo->secret_data,i,file_buff);
    }
    //printf("%s\n",decInfo->secret_data);
    FILE *data_fptr = fopen(decInfo->txt_name,"w");
    fprintf(data_fptr,"%s",decInfo->secret_data);
    fclose(data_fptr);

    return e_success;
}

/*function to decode lsb into byte(char)*/
Status decode_byte_to_lsb(char *data,int j,char *image_buffer)
{
    data[j] = 0;
    for(int i=0;i<8;i++)
    {
        data[j] |= (image_buffer[i] & 1)<<i;
    }
}

Status decode_size_to_lsb(int *size, char *image_buffer)
{
    int i;
    *size = 0;
    for(i=0;i<32;i++)
    {
        *size |= (image_buffer[i] & 1)<<i;
    }
  
}

