#include <SDL.h> 
#include <math.h> 
#include <vector> 
#include <Primitive.h>

#include "projection.h"

global bool Running = true; 

internal void MultiplyMatrixVector(Vec3f &i, Vec3f &o, Matrix_4x4 &m)
{
    o.x = i.x * m.element[0][0] + i.y * m.element[1][0] + i.z * m.element[2][0] + m.element[3][0];
    o.y = i.x * m.element[0][1] + i.y * m.element[1][1] + i.z * m.element[2][1] + m.element[3][1];
    o.z = i.x * m.element[0][2] + i.y * m.element[1][2] + i.z * m.element[2][2] + m.element[3][2];
    float w = i.x * m.element[0][3] + i.y * m.element[1][3] + i.z * m.element[2][3] + m.element[3][3];

    if (w != 0.0f)
    {
        o.x /= w; o.y /= w; o.z /= w;
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("3D Projection",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer)
        {
            Mesh cube_mesh;
            Matrix_4x4 projection_matrix; 
            float theta; 

            cube_mesh.triangles = {
                // SOUTH
                { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

                // EAST                                                      
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

                // NORTH                                                     
                { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                // WEST                                                      
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

                // TOP                                                       
                { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                // BOTTOM                                                    
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
            };

            // Projection Matrix
            float near = 0.1f;
            float far = 1000.0f;
            float fov = 180.0f;
            float aspect_ratio = (float)WINDOW_HEIGHT / (float)WINDOW_HEIGHT;
            float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

            projection_matrix.element[0][0] = aspect_ratio * fov_rad;
            projection_matrix.element[1][1] = fov_rad;
            projection_matrix.element[2][2] = far / (far - near);
            projection_matrix.element[3][2] = (-far * near) / (far - near);
            projection_matrix.element[2][3] = 1.0f;
            projection_matrix.element[3][3] = 0.0f;

            uint32_t last_tick_time = 0;
            uint32_t delta = 0;

            while(Running)
            {
                uint32_t tick_time = SDL_GetTicks();
                delta = (float)(tick_time - last_tick_time);
                last_tick_time = tick_time;

                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                    if (event.type == SDL_QUIT)
                    {
                        Running = false; 
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                Matrix_4x4 matRotZ, matRotX;
                theta = 1.0f * delta / 2;

                // Rotation Z
                matRotZ.element[0][0] = cosf(theta);
                matRotZ.element[0][1] = sinf(theta);
                matRotZ.element[1][0] = -sinf(theta);
                matRotZ.element[1][1] = cosf(theta);
                matRotZ.element[2][2] = 1;
                matRotZ.element[3][3] = 1;

                // Rotation X
                matRotX.element[0][0] = 1;
                matRotX.element[1][1] = cosf(theta * 0.5f);
                matRotX.element[1][2] = sinf(theta * 0.5f);
                matRotX.element[2][1] = -sinf(theta * 0.5f);
                matRotX.element[2][2] = cosf(theta * 0.5f);
                matRotX.element[3][3] = 1;
                
                Triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

                // Draw Triangles
                for (auto tri : cube_mesh.triangles)
                {
                    // Rotate in Z-Axis
                    MultiplyMatrixVector(tri.point[0], triRotatedZ.point[0], matRotZ);
                    MultiplyMatrixVector(tri.point[1], triRotatedZ.point[1], matRotZ);
                    MultiplyMatrixVector(tri.point[2], triRotatedZ.point[2], matRotZ);

                    // Rotate in X-Axis
                    MultiplyMatrixVector(triRotatedZ.point[0], triRotatedZX.point[0], matRotX);
                    MultiplyMatrixVector(triRotatedZ.point[1], triRotatedZX.point[1], matRotX);
                    MultiplyMatrixVector(triRotatedZ.point[2], triRotatedZX.point[2], matRotX);

                    // Offset into the screen
                    triTranslated = triRotatedZX;
                    triTranslated.point[0].z = triRotatedZX.point[0].z + 3.0f;
                    triTranslated.point[1].z = triRotatedZX.point[1].z + 3.0f;
                    triTranslated.point[2].z = triRotatedZX.point[2].z + 3.0f;

                    // Project triangles from 3D --> 2D
                    MultiplyMatrixVector(triRotatedZX.point[0], triProjected.point[0], projection_matrix);
                    MultiplyMatrixVector(triRotatedZX.point[1], triProjected.point[1], projection_matrix);
                    MultiplyMatrixVector(triRotatedZX.point[2], triProjected.point[2], projection_matrix);

                    // Scale into view
                    triProjected.point[0].x += 1.0f; 
                    triProjected.point[0].y += 1.0f;
                    triProjected.point[1].x += 1.0f; 
                    triProjected.point[1].y += 1.0f;
                    triProjected.point[2].x += 1.0f; 
                    triProjected.point[2].y += 1.0f;

                    triProjected.point[0].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.point[0].y *= 0.5f * (float)WINDOW_HEIGHT;
                    triProjected.point[1].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.point[1].y *= 0.5f * (float)WINDOW_HEIGHT;
                    triProjected.point[2].x *= 0.5f * (float)WINDOW_WIDTH;
                    triProjected.point[2].y *= 0.5f * (float)WINDOW_HEIGHT;
                }

                local_persist Primitive_Color white = { 255, 255, 255, 255 };

                Primitive_DrawTriangle(renderer,
                                        triProjected.point[0].x, triProjected.point[0].y,
                                        triProjected.point[1].x, triProjected.point[1].y,
                                        triProjected.point[2].x, triProjected.point[2].y, white);
                
                SDL_RenderPresent(renderer);
                SDL_Delay(50);
            }
        }
        else 
        {
            printf("Could not create window: %s\n", SDL_GetError());
            return 1;
        }
    }
    else
    {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Quit();
    return 0;
}