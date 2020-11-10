#pragma once 

#include <vector> 

#define internal static 
#define local_persist static
#define global static

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480 

struct Vec3f { 
    float x;
    float y; 
    float z;
};

struct Matrix_4x4 { 
    float element[4][4] = { 0 };
};

struct Triangle { 
    Vec3f point[3]; 
}; 

struct Mesh { 
    std::vector<Triangle> triangles;
};
