#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gost_3411_2012_calc.h"
#include "gost_3411_2012_test.h"

#define FILE_BUFFER_SIZE 4096
#define DEFAULT_HASH_SIZE 512

TGOSTHashContext *CTX;

static void
HashPrint(TGOSTHashContext *CTX)
{
    printf("%d bit hash sum: \n", CTX->hash_size);
    int i;
    if (CTX->hash_size == 256)
        for(i = 32; i < 64; i++)
            printf("%02x", CTX->hash[i]);
    else
        for(i = 0; i < 64; i++)
            printf("%02x", CTX->hash[i]);
    printf("\n");
}

static void
GetHashTest()
{
    printf("GOST 34.11-2012 \"Stribog\"\nTest String: "
           "012345678901234567890123456789012345678901234567890123456789012\n");
    GOSTHashInit(CTX, 512);
    GOSTHashUpdate(CTX, test_string, sizeof test_string);
    GOSTHashFinal(CTX);
    HashPrint(CTX);
    GOSTHashInit(CTX, 256);
    GOSTHashUpdate(CTX, test_string, sizeof test_string);
    GOSTHashFinal(CTX);
    HashPrint(CTX);
}

static void
GetHashString(const char *str, int hash_size)
{
    uint8_t *buffer;
    buffer = malloc(strlen(str));
    memcpy(buffer, str, strlen(str));
    GOSTHashInit(CTX, hash_size);
    GOSTHashUpdate(CTX, buffer, strlen(str));
    GOSTHashFinal(CTX);
    printf("GOST 34.11-2012 \"Stribog\"\nString: %s\n", str);
    HashPrint(CTX);
}

static void
GetHashFile(const char *file_name, int hash_size)
{
    FILE *file;
    uint8_t *buffer;
    size_t len;
    GOSTHashInit(CTX, hash_size);
    if ((file = fopen(file_name, "rb")) != NULL)
    {
        buffer = malloc((size_t) FILE_BUFFER_SIZE);
        while ((len = fread(buffer, (size_t) 1, (size_t) FILE_BUFFER_SIZE, file)))
            GOSTHashUpdate(CTX, buffer, len);
        free(buffer);
        GOSTHashFinal(CTX);
        fclose(file);
        printf("GOST 34.11-2012 \"Stribog\"\nFile name: %s\n", file_name);
        HashPrint(CTX);
    }
    else
        printf("File error: %s\n", file_name);
}

int main(int argc, char *argv[])
{
    CTX = (TGOSTHashContext*)(malloc(sizeof(TGOSTHashContext)));

    int hash_size = DEFAULT_HASH_SIZE;
    int opt;
    while ((opt = getopt(argc, argv, "htf:s:d:")) != -1)
    {
        switch (opt)
        {
            case 't':
                GetHashTest();
            break;
            case 'd':
                if (strcmp(optarg, "256") == 0)
                    hash_size = 256;
                if (strcmp(optarg, "512") == 0)
                    hash_size = 512;
            break;
            case 'h':
                printf("GOST 34.11-2012 \"Stribog\"\nstribog.exe [-d <256 or 512>] [-s <string>] [-f <file>]\n");
            break;
            case 'f':
                GetHashFile(optarg, hash_size);
            break;
            case 's':
                GetHashString(optarg, hash_size);
            break;
        }
    }
    return 0;
}
