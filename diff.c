#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int read_BMP(char* filename1, char* filename2, unsigned char *info, unsigned char **dataR, unsigned char **dataG, unsigned char **dataB, int *size, int *width, int *height, int *offset, int *row_padded)
{
    int i = 0, j, k, read_bytes, h, w, o, p;
    unsigned char *data;

    FILE* f = fopen(filename1, "rb");

    if(f == NULL)
    {
        printf ("Invalid filename: %s\n", filename1);
        return -1;
    }


    read_bytes = fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    if (read_bytes != 54)
    {
        printf ("Error at read: %d instead of 54 bytes", read_bytes);
        return -1;
    }


    // extract image data from header
    *width = *(int*)&info[18];
    *height = *(int*)&info[22];
    *size = *(int*)&info[2];
    *offset = *(int*)&info[10];
    *row_padded = (*width*3 + 3) & (~3);

    
    //printf ("Filename: %s, Width: %d, Row_padded: %d, Height: %d, Size:  %d, Offset: %d\n", filename, *width, *row_padded, *height, *size, *offset);
    w = *width;
    p = *row_padded;
    h = *height;
    o = *offset;
    
    data = (unsigned char*) malloc (p * h);
    *dataR = (unsigned char*) malloc (w * h);
    *dataG = (unsigned char*) malloc (w * h);
    *dataB = (unsigned char*) malloc (w * h);

    fseek(f, sizeof(unsigned char) * o, SEEK_SET);
    read_bytes = fread(data, sizeof(unsigned char), p * h, f);
    if (read_bytes != p * h)
    {
        printf ("Error at read: %d\n", read_bytes);
        free (data);
        return -1;
    }
    for (k = 0; k < h; k++)
    {
        i = k * p;
        for (j = 0; j < w; j++)
        {
            (*dataB)[k*w + j] = data[i];
            (*dataG)[k*w + j] = data[i + 1];
            (*dataR)[k*w + j] = data[i + 2];

            //printf ("BGR %d %d i= %d: %d %d %d\n", k, j, i, data[i], data[i+1], data[i+2]);
            i+= 3;
        }
    }

    fclose(f);
    f = fopen(filename2, "rb");
    
    if(f == NULL)
    {
        printf ("Invalid filename: %s\n", filename2);
        return -1;
    }


    read_bytes = fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    if (read_bytes != 54)
    {
        printf ("Error at read: %d instead of 54 bytes", read_bytes);
        return -1;
    }
    
    // extract image data from header
    *width = *(int*)&info[18];
    *height = *(int*)&info[22];
    *size = *(int*)&info[2];
    *offset = *(int*)&info[10];
    *row_padded = (*width*3 + 3) & (~3);

    
    //printf ("Filename: %s, Width: %d, Row_padded: %d, Height: %d, Size:  %d, Offset: %d\n", filename, *width, *row_padded, *height, *size, *offset);
    w = *width;
    p = *row_padded;
    h = *height;
    o = *offset;
    
    free (data);
    data = (unsigned char*) malloc (p * h);

    fseek(f, sizeof(unsigned char) * o, SEEK_SET);
    read_bytes = fread(data, sizeof(unsigned char), p * h, f);
    if (read_bytes != p * h)
    {
        printf ("Error at read: %d\n", read_bytes);
        free (data);
        return -1;
    }
    for (k = 0; k < h; k++)
    {
        i = k * p;
        for (j = 0; j < w; j++)
        {
            if ((*dataB)[k*w + j] != data[i] ) { printf("wrong B\n");}
            if ((*dataG)[k*w + j] != data[i + 1] ) { printf("wrong G\n"); return -1;}
            if ((*dataR)[k*w + j] != data[i + 2] ) { printf("wrong R\n"); return -1;}

            //printf ("BGR %d %d i= %d: %d %d %d\n", k, j, i, data[i], data[i+1], data[i+2]);
            i+= 3;
        }
    }

    free (data);
    fclose(f);
    return 0;
}

int main(int argc, char ** argv)
{
    unsigned char info[54], *dataR = NULL, *dataG = NULL, *dataB = NULL;
    int blur_size, ret_code = 0, size, width, height, offset, row_padded;
    char *filename1, *filename2;
    float* dstB, *dstR, *dstG, sigma;
    double start, end;

    if (argc != 3)
    {
        printf ("Usage: %s <filename.bmp> <sigma> <blur_size> <output_filename.bmp>", argv[0]);
        return -1;
    }
    filename1 = argv[1];
    filename2 = argv[2];
    ret_code = read_BMP(filename1,filename2, info, &dataR, &dataG, &dataB, &size, &width, &height, &offset, &row_padded);
    if (ret_code < 0)
    {
        free (dataB);
        free (dataR);
        free (dataG);
        return -1;
    }


    return ret_code;
}