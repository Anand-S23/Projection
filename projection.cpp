#include <include/SDL.h> 
#include <math.h> 
#include <vector> 

#include "projection.h"


global bool Running = true; 

internal void MultiplyMatrixVector()

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("3D Projection",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED, 
                                           640, 480, 0);

    Mesh cube_mesh;
    Matrix_4x4 projection_matrix; 
    float theta; 

    cube_mesh.trianlges = {
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
        float fov = 90.0f;
        float aspect_ratio = (float)WINDOW_HEIGHT / (float)WINDOW_HEIGHT;
        float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

        projection_matrix.element[0][0] = aspect_ratio * fov_rad;
        projection_matrix.element[1][1] = fov_rad;
        projection_matrix.element[2][2] = far / (far - near);
        projection_matrix.element[3][2] = (-far * near) / (far - near);
        projection_matrix.element[2][3] = 1.0f;
        projection_matrix.element[3][3] = 0.0f;


    if (window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer)
        {
            while(Running)
            {
                SDL_Event event;
                while(SDL_PollEvent(&event))
                {
                }
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