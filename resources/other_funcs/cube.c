#include "raylib.h"
#include "rlgl.h"
#include "cube.h"


void Follow_Trajectory(struct all_triangles_mesh_events* initial_state, struct all_triangles_mesh_events* current_state,  trajectory_function traj, float absolute_time){
    Vector3 new_pos = traj(absolute_time);

    for (int which_triangle=0; which_triangle<current_state->amount_of_triangles; ++which_triangle){
        for(int which_vertex=0; which_vertex<3; ++which_vertex){
            current_state->all_triangles[which_triangle].all_3_vertices[which_vertex].x = initial_state->all_triangles[which_triangle].all_3_vertices[which_vertex].x + new_pos.x;
            current_state->all_triangles[which_triangle].all_3_vertices[which_vertex].y = initial_state->all_triangles[which_triangle].all_3_vertices[which_vertex].y + new_pos.y;
            current_state->all_triangles[which_triangle].all_3_vertices[which_vertex].z = initial_state->all_triangles[which_triangle].all_3_vertices[which_vertex].z + new_pos.z;
        }
    }    


    return;
}




void ScaleSizeMeshEvents( struct all_triangles_mesh_events* atme, float scale_factor){
    for(int which_triangle=0; which_triangle < atme->amount_of_triangles; ++which_triangle)
    {
        for(int which_vertex=0; which_vertex<3; ++which_vertex){
            atme->all_triangles[which_triangle].all_3_vertices[which_vertex].x *= scale_factor;
            atme->all_triangles[which_triangle].all_3_vertices[which_vertex].y *= scale_factor;
            atme->all_triangles[which_triangle].all_3_vertices[which_vertex].z *= scale_factor;
        }
    }
    return;
}



void DrawTriangleMeshFromEvents(struct all_triangles_mesh_events* atme){
    // initialize x,y,z to be used for each triangle to extract from their Vector3s:
    float x_1=0.0f; float y_1=0.0f; float z_1 = 0.0f; float x_2=0.0f; float y_2=0.0f; float z_2 = 0.0f; 
    int next_vertex;

    // save the old state onto the stack, again, not necessary
    rlPushMatrix();

    // We are making lines
    rlBegin(RL_LINES);

        rlColor4ub(0, 0, 0, 255);

        for(int which_triangle=0; which_triangle < atme->amount_of_triangles; ++which_triangle){
            for(int which_vertex=0; which_vertex < 3; ++which_vertex){

                switch (which_vertex)
                {
                case 0:
                    next_vertex=1; break;
                case 1:
                    next_vertex=2; break;
                case 2:
                    next_vertex=0; break;
                }
                x_1 = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].x;   x_2 = atme->all_triangles[which_triangle].all_3_vertices[next_vertex].x;
                y_1 = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].y;   y_2 = atme->all_triangles[which_triangle].all_3_vertices[next_vertex].y;
                z_1 = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].z;   z_2 = atme->all_triangles[which_triangle].all_3_vertices[next_vertex].z;

                rlVertex3f(x_1,y_1,z_1);
                rlVertex3f(x_2,y_2,z_2);
            }
        }
    rlEnd();

    // oops, forgot this.
    rlPopMatrix();
}




void DrawFromAllTrianglesMeshEvents (struct all_triangles_mesh_events* atme){
    // initialize x,y,z to be used for each triangle to extract from their Vector3s:
    float x=0.0f; float y=0.0f; float z;

    // Until we fix it, let's do this
    rlDisableBackfaceCulling();

    // Save the old state onto the stack (unneccesary since we don't do transformations, but good to have)
    rlPushMatrix();

    // Tell rlgl that we will give it triangles (and not lines or 'quads', whatever that is).
    rlBegin(RL_TRIANGLES);

        rlColor4ub(0, 0, 255, 255);
        
        for(int which_triangle=0; which_triangle< atme->amount_of_triangles; ++which_triangle){
            for(int which_vertex=0; which_vertex<3; ++which_vertex){
                x = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].x;
                y = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].y;
                z = atme->all_triangles[which_triangle].all_3_vertices[which_vertex].z;
                
                // Send the vertex to rlgl
                rlVertex3f(x,y,z);
            }

        }

    // We are not giving more triangles
    rlEnd();

    // Now we pop the old state off the stack and back as the current transformation matrix state
    rlPopMatrix(); 

    return;
}





// NOTE: Cube position is the center position
void ModifiedDrawCube(float width, float height, float length, Color color)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    rlPushMatrix();
        // NOTE: dBe careful! Function order matters (rotate -> scale -> translate)

        // This is what causes the relative coordinates;
        // rlTranslatef(position.x, position.y, position.z);

        //rlScalef(2.0f, 2.0f, 2.0f);
        //rlRotatef(45, 0, 1, 0);

        rlBegin(RL_TRIANGLES);
            rlColor4ub(color.r, color.g, color.b, color.a);

            // Front Face -----------------------------------------------------
            rlVertex3f(x-width/2, y-height/2, z+length/2);  // Bottom Left
            rlVertex3f(x+width/2, y-height/2, z+length/2);  // Bottom Right
            rlVertex3f(x-width/2, y+height/2, z+length/2);  // Top Left

            rlVertex3f(x+width/2, y+height/2, z+length/2);  // Top Right
            rlVertex3f(x-width/2, y+height/2, z+length/2);  // Top Left
            rlVertex3f(x+width/2, y-height/2, z+length/2);  // Bottom Right

            // Back Face ------------------------------------------------------
            rlVertex3f(x-width/2, y-height/2, z-length/2);  // Bottom Left
            rlVertex3f(x-width/2, y+height/2, z-length/2);  // Top Left
            rlVertex3f(x+width/2, y-height/2, z-length/2);  // Bottom Right

            rlVertex3f(x+width/2, y+height/2, z-length/2);  // Top Right
            rlVertex3f(x+width/2, y-height/2, z-length/2);  // Bottom Right
            rlVertex3f(x-width/2, y+height/2, z-length/2);  // Top Left

            // Top Face -------------------------------------------------------
            rlVertex3f(x-width/2, y+height/2, z-length/2);  // Top Left
            rlVertex3f(x-width/2, y+height/2, z+length/2);  // Bottom Left
            rlVertex3f(x+width/2, y+height/2, z+length/2);  // Bottom Right

            rlVertex3f(x+width/2, y+height/2, z-length/2);  // Top Right
            rlVertex3f(x-width/2, y+height/2, z-length/2);  // Top Left
            rlVertex3f(x+width/2, y+height/2, z+length/2);  // Bottom Right

            // Bottom Face ----------------------------------------------------
            rlVertex3f(x-width/2, y-height/2, z-length/2);  // Top Left
            rlVertex3f(x+width/2, y-height/2, z+length/2);  // Bottom Right
            rlVertex3f(x-width/2, y-height/2, z+length/2);  // Bottom Left

            rlVertex3f(x+width/2, y-height/2, z-length/2);  // Top Right
            rlVertex3f(x+width/2, y-height/2, z+length/2);  // Bottom Right
            rlVertex3f(x-width/2, y-height/2, z-length/2);  // Top Left

            // Right face -----------------------------------------------------
            rlVertex3f(x+width/2, y-height/2, z-length/2);  // Bottom Right
            rlVertex3f(x+width/2, y+height/2, z-length/2);  // Top Right
            rlVertex3f(x+width/2, y+height/2, z+length/2);  // Top Left

            rlVertex3f(x+width/2, y-height/2, z+length/2);  // Bottom Left
            rlVertex3f(x+width/2, y-height/2, z-length/2);  // Bottom Right
            rlVertex3f(x+width/2, y+height/2, z+length/2);  // Top Left

            // Left Face ------------------------------------------------------
            rlVertex3f(x-width/2, y-height/2, z-length/2);  // Bottom Right
            rlVertex3f(x-width/2, y+height/2, z+length/2);  // Top Left
            rlVertex3f(x-width/2, y+height/2, z-length/2);  // Top Right

            rlVertex3f(x-width/2, y-height/2, z+length/2);  // Bottom Left
            rlVertex3f(x-width/2, y+height/2, z+length/2);  // Top Left
            rlVertex3f(x-width/2, y-height/2, z-length/2);  // Bottom Right
        rlEnd();
    rlPopMatrix();
}


