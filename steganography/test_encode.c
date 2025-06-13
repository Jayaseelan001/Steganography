#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "color.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;
    DecodeInfo decInfo;

    if(argc == 1)
    {
        printf(BHRED"ERROR: Correct Format: ./a.out -e <.bmp.file> <.txt.file> [optional output file]"RESET"\n");
        return 0;
    }

    // Check the operation type
    OperationType OprType = check_operation_type(argv);
    
    if (OprType == e_encode)
    {
         if(argc == 2)
    {
        printf(BHRED"ERROR: Pass .bmp file"RESET"\n");
        printf(BHRED"ERROR: Correct Format: ./a.out -e <.bmp.file> <.txt.file> [optional output file]"RESET"\n");
        return 0;
    }

    if(argc == 3)
    {
        printf(BHRED"ERROR: Pass .txt secret file"RESET"\n");
        printf(BHRED"ERROR: Correct Format: ./a.out -e <.bmp.file> <.txt.file> [optional output file]"RESET"\n");
        return 0;
    }
        if (argc == 4 || argc == 5 )
        {
            if (read_and_validate_encode_args(argv, &encInfo) == e_success)
            {
                printf("INFO: Opening required files\n");
                if (do_encoding(&encInfo) == e_success)
                {
                    printf(BHCYN"INFO: ## Encoding Done Successfully ##"RESET"\n");
                }
            }
            else
            {
                printf("Error : Validation is unsuccessful...\n");
            }
        }
        
    }
    else if (OprType == e_decode)
    {
        if(argc == 2)
        {
            printf(BHRED"ERROR: Pass .bmp encoded output file"RESET"\n");
        }
        if (argc == 3 || argc == 4)
        {
            printf(YELLOW"INFO : ## Decoding Procedure Started ##"RESET"\n");
            if (read_and_validate_decode_args(argv, &decInfo) == e_success)
            {
                printf("INFO: Opening required files\n");
                if (do_decoding(&decInfo) == e_success)
                {
                    printf(BHCYN"INFO : ## Decoding Done Successfully ##"RESET"\n");
                }
                else
                {
                    printf(BHRED"Error : Decoding Unsuccessful !!!"RESET"\n");
                }
            }
            else{
                return 0;
            }
        }
        else
        {
            printf(BHRED"Usage: ./a.out -d <encoded_image.bmp> [output file]"RESET"\n");
        }
    }
    else
    {
        printf("Error: Unsupported operation\n");
    }

    return 0;
}

OperationType check_operation_type(char *argv[])
{
    // Check argv[1] for encode or decode
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // Check argv[2] is .bmp file or not
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
        printf("INFO : Opened %s\n", argv[2]);
    }
    else
    {
        printf("Error : .bmp file doesn't exist...\n");
        return e_failure;
        return 0;
    }

    // Check argv[3] is .txt file or not
     if(strstr(argv[3],".txt"))                       // checking whether given input file argv[3] is .txt file or not
    {
        encInfo->secret_fname = argv[3];             // storing argv[3] in secret fname
        strcpy(encInfo->extn_secret_file,strstr(argv[3],".txt"));
    }
    else
    {
        printf("Error : .txt file doesn't exist...\n");
        return e_failure;
    }

    // Check argv[4] is empty or not
    if (argv[4] == NULL)
    {
        encInfo->stego_image_fname = "output.bmp";
        printf("INFO : Opened %s\n", encInfo->stego_image_fname);
    }
    else if (strstr(argv[4], ".bmp"))
    {
        encInfo->stego_image_fname = argv[4];
    }
    else
    {
        printf("Error : .bmp file doesn't exist in output file...\n");
        return e_failure;
    }

    return e_success;
}

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if(strcmp(strchr(argv[2],'.'),".bmp") == 0)
    {
        decInfo->encoded_image = argv[2];
        printf("INFO: Opening recqiured files\n");
        printf("INFO: Opened %s\n", argv[2]);
    }
    else if (strchr(argv[2], '.') == NULL)
    {
       printf("Error : pass encoded file with extension\n");
    }
    else
    {
        printf("Error : File extension is wrong\n");
        return e_failure;
    }

    if (argv[3] != NULL)
    {
        strcpy(decInfo->txt_name,argv[3]);
    }
    else
    {
        strcpy(decInfo->txt_name,"create");
    }

    return e_success;
}
