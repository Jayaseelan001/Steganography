#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "color.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
 uint get_file_size(FILE *fptr)
 {
    uint size;
    fseek(fptr,0,SEEK_END);
    size = ftell(fptr);
    return size;
    rewind(fptr);

 }
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO: Done\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf(YELLOW"INFO : ## Encoding Procedure Started ##"RESET"\n");
            printf("INFO: Checking for secret.txt size\n");
            printf("INFO: Done\n");
            if (copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
            {
                printf("INFO: Copying Image Header\n");
                printf("INFO: Done\n");
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("INFO: Encoding Magic String Signature\n");
                    printf("INFO: Done\n");
                    if(encode_secret_file_extn_size(sizeof(int),encInfo) == e_success)
                    {
                        printf("INFO: Encoding secret.txt File Extension Size\n");
                        printf("INFO: Done\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo) == e_success)
                        {
                            printf("INFO: Encoding secret.txt File Extension\n");
                            printf("INFO: Done\n");
                            if(encode_secret_file_data_size(encInfo->size_secret_file,encInfo) == e_success)
                            {
                                printf("INFO: Encoding secret.txt File Size\n");
                                printf("INFO: Done\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("INFO: Encoding secret.txt File Data\n");
                                    printf("INFO: Done\n");
                                    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("INFO: Copying Left Over Data\n");
                                        printf("INFO: Done\n");
                                    }
                                    else
                                    {
                                        printf("Error : Encoding remaining data is failed !!!\n");
                                    }
                                }
                                else{
                                    printf("Error : Encoding of secret file data is failed !!!\n");
                                }
                            }
                            else
                            {
                                printf("Error : Encoded of file size failed!!!\n");
                            }
                        }
                        else
                        {
                            printf("Error : Encoding extension failed!!!\n");
                        }
                    }
                    else{
                        printf("Error : Secret file data encoding failed!!!\n");
                    }
                }
                else
                {
                    printf("Error : Magic string  encoding failed!!!\n");
                }
                
            }
            else
            {
                printf("Error : Header files doesn't copied ...\n");
            }
        }
        else
        {
            printf("Error : Capacity doesn't sufficient...\n");
        }
    }
    else
    {
        printf("Error : Opening files is unsuccessfull...\n");
    }

    return e_success;
}
Status check_capacity(EncodeInfo *encInfo)
{
    uint input_bmp = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    uint output_bmp = 54 + (8*((strlen(MAGIC_STRING)) + 4 + strlen(strstr(encInfo->secret_fname,".")) + 4 + encInfo->size_secret_file));
    // printf("The size of whole output image size is %u\n%u\n",output_bmp,input_bmp);
    if(input_bmp >= output_bmp)
    {
         return e_success;
    }
    else{
        printf("Capacity checked unsuccessfully...\n");
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    rewind(fptr_src_image);//input
    rewind(fptr_dest_image);//output
    char arr[54];
    fread(arr , 54 , 1 ,fptr_src_image);
    fwrite(arr , 54 , 1 ,fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];

    for(int i=0 ; i < strlen(MAGIC_STRING) ; i++)
    {
        fread(buffer , 8 ,1 ,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],buffer);
        fwrite(buffer , 8 , 1 ,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_secret_file_extn_size(long size,EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
   // printf("%ld---!!\n",strlen(encInfo->extn_secret_file));
    encode_size_to_lsb(strlen(encInfo->extn_secret_file),buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];
    //printf("--->%ld\n",strlen(file_extn));
    
    for(int i =0 ;i < strlen(file_extn) ; i++)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],buffer);
        fwrite(buffer,8,1,encInfo->fptr_stego_image);
        //printf("---%c\n",file_extn[i]);
    }

    return e_success;
}

Status encode_secret_file_data_size(long data_size , EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(data_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);

    //printf("--%ld--\n",data_size);

    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch[(encInfo->size_secret_file)];
    //printf("%ld\n",encInfo->size_secret_file);
    rewind(encInfo->fptr_secret);
    fread(ch,sizeof(ch),1,encInfo->fptr_secret);

    char buffer[8];
    //printf("file size -> %ld\n",encInfo->size_secret_file);
    for(int i =0 ; i < encInfo->size_secret_file ; i++)
    {
        fread(buffer,8,1,encInfo->fptr_src_image);
        encode_byte_to_lsb(ch[i],buffer);
        // printf("%c--",ch[i]);
        fwrite(buffer,8,1,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,sizeof(char),1,fptr_src) > 0)
    {
        fwrite(&ch,sizeof(char),1,fptr_dest);
    }

    return e_success;
}

Status encode_byte_to_lsb(char data,char *image_buffer)
{
    char ch;
    char get;
    for(int i=0 ; i < 8 ; i++)
    {
        ch = image_buffer[i] & (~1);
        get = (data >> i) & 1;
        image_buffer[i] = ch | get;
    }
}

Status encode_size_to_lsb(long size,char *image_buffer)
{
    char ch;
    char get;

    for(int i =0 ; i < 32 ; i++)
    {
        ch = image_buffer[i] & (~1);
        get = (size >> i) & 1;
        //printf("%d --\n",get);
        image_buffer[i] = ch | get;
    }
}