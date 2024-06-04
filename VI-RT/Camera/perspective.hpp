#ifndef perspective_hpp
#define perspective_hpp

#include "camera.hpp"
#include "ray.hpp"
#include "vector.hpp"

class Perspective : public Camera
{
    Point Eye, At;
    Vector Up;

    float fovW, fovH; // In radians horizontal and vertical field of view respectively
    int W, H;
    float c2w[3][3]; // camera 2 world transform
    
public:
    Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH); 

    bool GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter = NULL);
    void getResolution(int *_W, int *_H)
    {
        *_W = W;
        *_H = H;
    }
};

#endif /* perspective_hpp */
