#include "CA1d_io.h"
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "cube.h"



bool is_number_char(char c){
    char numbers[14] = {'0','1','2','3','4','5','6','7','8','9','.','-','+', 'e'};
    for(int i=0; i<14;++i){
        if(c==numbers[i]){
            return true;
        }
    }
    return false;
}

void print_vertex_events(struct all_triangles_mesh_events* atme){
    if(!atme){
        printf("%s", "Pointer given to print is NULL.\n");
        return;
    }


    printf("%s", "\n The time is: "); printf("%f", atme->time);
    for(int i=0; i < atme->amount_of_triangles; i++)
    {
        printf("%s", "\n\n === Triangle "); printf("%d", i); printf("%s", " === \n");
        for(int j=0; j<3; j++){
            printf("%s", "\n Vertex "); printf("%d", j);
            for(int k=0; k<3; k++){
                printf("%s", " "); 
                switch (k)
                {
                case 0:
                    printf("%f", atme->all_triangles[i].all_3_vertices[j].x);
                    break;
                case 1:
                    printf("%f", atme->all_triangles[i].all_3_vertices[j].y);
                    break;
                case 2:
                    printf("%f", atme->all_triangles[i].all_3_vertices[j].z);
                    break;
                }
            }
        }
    }
    printf("%s", "\n");
    return;


}



char* get_substring_leak(char* input_char_array, int index_start, int index_end){
    //this does not check for out of bounds
    char* substring = (char*) calloc(index_end - index_start +2, sizeof(char));
    int j = 0;
    for(int i=index_start;i<index_end+1; ++i){
        //printf("%c", input_char_array[i]);
        substring[j] = input_char_array[i];
        j++;
    }
    substring[j] = '\0';
    return substring;
}

int count_occurences_in_CA1d(struct CA1d* ca1d, const char* string_to_compare_to) {
    int occurrences_so_far = 0;
    const char* current_position = ca1d->char_array;
    const char* end_of_array = ca1d->char_array + ca1d->len_of_char_array;
    size_t len_to_compare = strlen(string_to_compare_to);

    while ((current_position = strstr(current_position, string_to_compare_to)) != NULL && current_position < end_of_array) {
        occurrences_so_far++;
        current_position += len_to_compare; // Move to the next character after the match
    }

    return occurrences_so_far;
}


struct all_triangles_mesh_events* get_events_from_stl (char *file_name, float time){
// you pass the file name and we give the vertices in some sort of struct
// how do we handle the not knowing how long? well:
// first we just count the faces, and then we allocate memory and loop again
// is that efficient? I'm not sure. However this does not run continously so it's fine.

    struct CA1d* stl_CA1d = read_file_to_ca1d_leak(file_name);
    //printf("%s", stl_CA1d->char_array);

    // First let's count how many outer loop and endloop. (They should be identical, otherwise the file is broken).
    int outer_loops = count_occurences_in_CA1d(stl_CA1d, "outer loop");
    int endloops = count_occurences_in_CA1d(stl_CA1d, "endloop");
    if ( (outer_loops != endloops) || (outer_loops < 4) ){
        free(stl_CA1d->char_array); free(stl_CA1d);
        return NULL;
    }

    //  now that we know how many faces we are dealing with, we can initialize our vertices struct. 

    // allocate memory for the each triangle
    struct one_triangle_vertices* all_vertices = (struct one_triangle_vertices*) calloc(3*outer_loops, sizeof(struct one_triangle_vertices));
    // now let's create the struct containing all of our triangles
    struct all_triangles_mesh_events* our_mesh_events = (struct all_triangles_mesh_events*) calloc(1, sizeof(struct all_triangles_mesh_events));
    our_mesh_events->all_triangles = all_vertices;
    our_mesh_events->amount_of_triangles = outer_loops;
    our_mesh_events->time = time;

    //Now, we need to loop through and actually populate the all_vertices array, which is an array of arrays[3] of Vector3s.
    // each outer_loop is one face, which is made of nine coordinates.
    bool in_face = false; bool is_number = false;
    char* string_remaining = stl_CA1d->char_array;
    char* endptr; // for the conversion, we don't care abt this.


    char current_num[30]; // assume all will be less than 30 decimals... otherwise come on!
    
    for(int which_triangle=0; which_triangle<outer_loops; ++which_triangle){

        string_remaining = strstr(string_remaining, "outer loop") + 10;

        // Now that we are there let's extract the numbers:
        for(int which_vertex=0; which_vertex<3; ++which_vertex){
            // now, find the vertex:
            string_remaining = strstr(string_remaining, "vertex") + 6;
            
            // now, we need to loop through to get each which_xyz:
            for(int which_xyz=0; which_xyz<3; ++which_xyz){
                while (true){
                // will this be an infinite loop? I don't think so, assuming a proper STL. 
                    if (is_number_char(*string_remaining)){
                        break;
                    }
                    string_remaining++;
                }
                // now we need to make a float from this. 

                memset(current_num, '\0', 30); // fill it with the end of line character
                for(int i=0; i<30; i++){
                    if(!is_number_char(*string_remaining)){ // so, stop when you hit a space
                        break;
                    }
                    current_num[i] = *string_remaining; 
                    string_remaining++;
                }
                switch (which_xyz) // assign to Vector3.x,y,z
                {
                case 0:
                    our_mesh_events->all_triangles[which_triangle].all_3_vertices[which_vertex].x = strtof(current_num, &endptr);
                    break;
                case 1:
                    our_mesh_events->all_triangles[which_triangle].all_3_vertices[which_vertex].y = strtof(current_num, &endptr);
                    break;
                case 2:
                    our_mesh_events->all_triangles[which_triangle].all_3_vertices[which_vertex].z = strtof(current_num, &endptr);
                    break;
                default:
                    break;
                }
            }


        }

    }
    
    free(stl_CA1d->char_array); free(stl_CA1d);
    return our_mesh_events;

}

