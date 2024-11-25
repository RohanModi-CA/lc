
/*******************************************************************************************
*
*   raylib [core] example - Initialize 3d camera free
*
*   Example originally created with raylib 1.3, last time updated with raylib 1.3
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2015-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <stdlib.h>
#include "resources/other_funcs/cube.h"
#include "resources/raylib/raylib.h"
#include "resources/world_funcs/camera_setup.h"
#include "resources/other_funcs/CA1d_io.h"
#include "resources/other_funcs/stl_handling.h"
#include <math.h>


Vector3 trajectory(float t){
    // I don't think this needs to be on the heap.
    Vector3 traj;
    traj.x = sin(t);
    traj.y = 0;
    traj.z = cos(t);
    return traj;
}




int main(void)
{

    struct all_triangles_mesh_events* cube_events = get_events_from_stl("resources/models/20mm_cube.stl", 0.0f);
    ScaleSizeMeshEvents( cube_events, 0.1  );

    struct one_triangle_vertices* vertices = (struct one_triangle_vertices*) calloc(cube_events->amount_of_triangles, sizeof(struct one_triangle_vertices)); 
    memcpy( vertices, cube_events->all_triangles, cube_events->amount_of_triangles * sizeof(struct one_triangle_vertices));

    struct all_triangles_mesh_events* the_fox = (struct all_triangles_mesh_events*) calloc(1, sizeof(struct all_triangles_mesh_events*));
    the_fox->all_triangles=vertices; the_fox->amount_of_triangles=cube_events->amount_of_triangles; the_fox->time=cube_events->time;

    //print_vertex_events(cube_events);


    // Initialization
    const int screenWidth = 1400;
    const int screenHeight = 950;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");
    SetTraceLogLevel(LOG_ERROR);

    Camera3D* camera = initialize_camera();

    DisableCursor();                    // Limit cursor to relative movement inside the window
    SetTargetFPS(30);                   // Set our game to run at 60 frames-per-second

    
    printf("%d", cube_events->amount_of_triangles);

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(camera, CAMERA_FREE);
        if (IsKeyPressed('Z')) camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };

        BeginDrawing();

            ClearBackground(RAYWHITE);
            BeginMode3D(*camera);

                Follow_Trajectory(cube_events, the_fox, trajectory, GetTime());

                //TranslateMeshEvents(cube_events, (Vector3) { sin(GetTime()) ,0,cos(GetTime())}, 0.6);

                DrawFromAllTrianglesMeshEvents(the_fox);

                DrawGrid(150, 1.0f);

            EndMode3D();


        EndDrawing();
    }



    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    free(cube_events->all_triangles); free(cube_events);
    free(the_fox->all_triangles); free(the_fox);
    free(camera);
    return 0;
}
