//
// Created by gram on 24/11/24.

//

#ifndef CUBE_H
#define CUBE_H

#endif //CUBE_H
#include "raylib.h"
#include "rlgl.h"



void print_vertex_events(struct all_triangles_mesh_events* atme);

struct all_triangles_mesh_events* get_events_from_stl (char *file_name, float time);


/* Sample Usage:

int main(){
    struct all_triangles_mesh_events* atme = stl_handling("../models/20mm_cube.stl", 0.0f);
    print_vertex_events(atme);

    return 1;
}

*/