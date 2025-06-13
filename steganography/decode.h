#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 1

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *encoded_image;
    FILE *fptr_encoded_image;

    /* Secret File Info */
    int size;
    char txt_name[20];
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[100];
    int size_secret_file;
} DecodeInfo;

/* Decoding function prototypes */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status decode_open_files(DecodeInfo *decInfo);

/* Decode and Compare the Magic string with encode */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extn */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file extn size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secret file data size*/
Status decode_secret_file_data_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *image_buffer,int j,char *data);

/* Decode a size into LSB of image data array */
Status decode_size_to_lsb(int *size,char *image_buffer);

#endif 
