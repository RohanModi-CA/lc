#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CA1d_io.h"


struct CA1d error_ca1d = {0,0};

struct CA1d* read_file_to_ca1d_leak(const char* filename) {
    /*
     * It is very important: this does *not* free the memory for the char_array. That must be done after use.
     */

    // Create a file pointer and open the file in
    // read mode.
    FILE* file_pointer = fopen(filename, "rb");

    if (file_pointer == NULL) {
        fprintf(stderr, "File could not be opened\n");
        return &error_ca1d;
    }

    // First, let's get the file size, so that we can allocate the right amount of memory for the string.
    // The fseek command moves a "cursor" of sorts through the file, and will stop at whence. For instance, the EOF.
    fseek(file_pointer, 0L, SEEK_END);
    // Now, we would like to know where this is, so let's ask where the cursor is. This basically tells us the Delta Memory.
    long int file_size = ftell(file_pointer);
    // now let's just go back to the start of the file // printf("%ld", file_size);
    rewind(file_pointer);

    // avoid memory leaks by closing the file
    // Now we want to dynamically allocate memory for a list of chars. Assuming the file is stored in ASCII,
    // the amount of chars is equal to the file size, in bytes, which is what ftell() gave us

    // ReSharper disable once CppDFAMemoryLeak
    char *file_contents = (char* ) calloc(file_size+1, sizeof(char)); // make sure to add room for the terminator

    // we already checked that the file was there, so no need to handle that right now.

    fread(file_contents, sizeof(char), file_size, file_pointer);
    file_contents[file_size]='\0';
    //printf("%s", file_contents);

    struct CA1d* file_contents_char_array = malloc(sizeof(struct CA1d));
    file_contents_char_array->char_array = file_contents;
    file_contents_char_array->len_of_char_array = file_size;


    fclose(file_pointer);
    // free(file_contents); // We need this to persist
    return file_contents_char_array;
}

int write_ca1d_to_file(const struct CA1d* ca1d_inp, const char* filename) {

   if (ca1d_inp->len_of_char_array != 0) {
        FILE *fp = fopen(filename, "wb");
        if (fp == NULL) {
            fprintf(stderr, "File could not be opened\n");
            return 1;
        }
        fprintf(fp, "%s", ca1d_inp->char_array);
   }
    return 0;
}

