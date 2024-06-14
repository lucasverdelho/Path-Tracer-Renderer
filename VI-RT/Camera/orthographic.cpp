#include "orthographic.hpp"
#include <stdio.h>

Orthographic::Orthographic(const Point Eye, const Point At, const Vector Up, const int W, const int H) 
    : Eye(Eye), At(At), Up(Up), W(W), H(H)
{
    // Compute camera to world transform
    Vector F = this->Eye.vec2point(At);
    F.normalize();

    Vector R = F.cross(Up);
    R.normalize();

    Vector U = R.cross(F);
    U.normalize();

    c2w[0][0] = R.X;
    c2w[0][1] = R.Y;
    c2w[0][2] = R.Z;

    c2w[1][0] = U.X;
    c2w[1][1] = U.Y;
    c2w[1][2] = U.Z;

    c2w[2][0] = F.X;
    c2w[2][1] = F.Y;
    c2w[2][2] = F.Z;

}

bool Orthographic::GenerateRay(int x, int y, Ray *r, const float *cam_jitter)
{
 
    // Orthographic projection ray generation
    // float sx = (2.0f * (x + 0.5f) / W - 1.0f);  // Normalized device coordinates x
    // float sy = (2.0f * (y + 0.5f) / H - 1.0f);  // Normalized device coordinates y

    //no need to normalize, but we need to put them in the right place in the world
    float sx = -W/2 + x + cam_jitter[0];
    float sy = H/2 - y + cam_jitter[1];

    // Calculate ray origin
    Point origin = Eye + sx * Vector(c2w[0][0], c2w[0][1], c2w[0][2]) + sy * Vector(c2w[1][0], c2w[1][1], c2w[1][2]);
    
    // printf("Origin: %f %f %f\n", origin.X, origin.Y, origin.Z);
    // Orthographic camera has a constant direction
    Vector direction = Vector(c2w[2][0], c2w[2][1], c2w[2][2]);

    // Create the ray
    *r = Ray(origin, direction);

    return true;
}

