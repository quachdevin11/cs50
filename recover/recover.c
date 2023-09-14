#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE \n");
        return 1;
    }

    char *in_filename = argv[1];
    FILE *in_file = fopen(in_filename, "r");

    if (in_file == NULL)
    {
        printf("Could not open file.\n");
        return 2;
    }

    int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE]; // One bLock of 512 BYTES buffer arry to store copied values
    int img_counter = 0;
    FILE *out_file = NULL;
    char out_filename[8];

    while (fread(&buffer, BLOCK_SIZE, 1, in_file) == 1)
    {
        // if beginning of JPEG File, open new file, write, then close
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xf0 && (buffer[3] & 0xf0) == 0xe0)
        {

            if (!(img_counter == 0))
            {
                fclose(out_file);
            }

            sprintf(out_filename, "%03i.jpg", img_counter);
            out_file = fopen(out_filename, "w");
            img_counter++;
        }

        if (!(img_counter == 0))
        {
            fwrite(&buffer, BLOCK_SIZE, 1, out_file);
        }

    }

    fclose(out_file);
    fclose(in_file);

    return 0;
}