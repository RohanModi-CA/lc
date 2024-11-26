//
// Created by gram on 23/11/24.
//

#ifndef CUBE_H
#define CUBE_H

#endif //CUBE_H
#include "raylib.h"
#include "rlgl.h"

struct one_triangle_vertices {
    // This contains three vertices
    Vector3 all_3_vertices[3];
};

struct all_triangles_mesh_events {
    struct one_triangle_vertices* all_triangles;
    int amount_of_triangles;
    float time;
};

typedef Vector3 (*trajectory_function)(float);


void Follow_Trajectory(struct all_triangles_mesh_events* initial_state, struct all_triangles_mesh_events* current_state,  trajectory_function traj, float absolute_time);


void ScaleSizeMeshEvents( struct all_triangles_mesh_events* atme, float scale_factor);

void DrawFromAllTrianglesMeshEvents (struct all_triangles_mesh_events* atme);

void DrawTriangleMeshFromEvents(struct all_triangles_mesh_events* atme);

void ModifiedDrawCube(float width, float height, float length, Color color);


