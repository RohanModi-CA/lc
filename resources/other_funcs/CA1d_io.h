#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* SAMPLE CODE
    struct CA1d* file_contents_char_array = read_file_to_ca1d_leak( "/home/gram/Téléchargements/hello123.stl" );
    //printf("%s\n", file_contents_char_array->char_array);
    write_ca1d_to_file(file_contents_char_array, "hello.txt");

    free(file_contents_char_array->char_array); // now we stop the leak
    free(file_contents_char_array);
*/

struct CA1d {
    char* char_array;
    size_t len_of_char_array;
};

struct FA1d {
    float* array;
    size_t len_of_array;
};

extern struct CA1d error_ca1d;


struct CA1d* read_file_to_ca1d_leak(const char* filename);

int write_ca1d_to_file(const struct CA1d* ca1d_inp, const char* filename);

