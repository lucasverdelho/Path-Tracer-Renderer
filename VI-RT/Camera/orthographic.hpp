#ifndef orthographic_hpp
#define orthographic_hpp

#include "camera.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Orthographic : public Camera
{
    Point Eye, At; // Camera position and look-at point
    Vector Up; // Vector pointing upwards from the camera
    int W, H; // Resolution of the camera
    float c2w[3][3]; // camera 2 world transform
    
public:
    Orthographic(const Point Eye, const Point At, const Vector Up, const int W, const int H);

    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = NULL);

    void getResolution(int *_W, int *_H)
    {
        *_W = W;
        *_H = H;
    }
};

#endif /* orthographic_hpp */
