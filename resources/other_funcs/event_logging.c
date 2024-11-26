#include "CA1d_io.h"
#include "cube.h"

struct CA1d* atme_to_CA1d_leak(struct all_triangles_mesh_events* atme){
    // First let's figure out how big this string will be. Our file format will be:
    /*
    *     TIME281.82122 TRIANGLE0 VERTEX0 X132.32001 Y12.031232 Z943.1239212 
    *     TIME281.82122 TRIANGLE0 VERTEX1 X132.32001 Y12.031232 Z943.1239212 
    *     TIME281.82122 TRIANGLE0 VERTEX2 X132.32001 Y12.031232 Z943.1239212 
    *     
    */
    
    struct CA1d* triangles_log = (struct CA1d*) calloc(1, sizeof(struct CA1d));
    
    // 3 lines per triangle
    int number_of_lines = 3 * atme->amount_of_triangles;

    // every line is at most (and also in the neighborhood) 80 characters long including the newline.
    triangles_log->char_array = (char*) malloc( number_of_lines * 80*sizeof(char));
    triangles_log->len_of_char_array = number_of_lines * 80;

    // set the whole thing to spaces
    memset(triangles_log->char_array, ' ',  number_of_lines*80*sizeof(char));

    char* current_pos_in_string = triangles_log->char_array; unsigned int nullchar_location; 

    for(int which_triangle=0; which_triangle<atme->amount_of_triangles; ++which_triangle){
        for(int which_vertex=0; which_vertex<3; ++which_vertex){
        nullchar_location = sprintf(current_pos_in_string, "TIME%.8g TRIANGLE%d VERTEX%d X%.8g Y%.8g Z%.8g", atme->time, which_triangle, which_vertex, atme->all_triangles[which_triangle].all_3_vertices[which_vertex].x, 
                                                                                     atme->all_triangles[which_triangle].all_3_vertices[which_vertex].y, atme->all_triangles[which_triangle].all_3_vertices[which_vertex].z);
        // set the nullchar to a space.
        current_pos_in_string[nullchar_location]=' ';

        // add a newline on the last spot of the row
        current_pos_in_string[79] = '\n';
        current_pos_in_string+=80;
        }
    }
    // add the null terminator
    triangles_log->char_array[ number_of_lines*80 -1] = '\0';

    return triangles_log;
}