#include "orthographic.hpp"

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


    // Compute the real width and height coordinates of the camera
    realW = Eye.X - W/2;
    realH = Eye.Y + H/2;

}

bool Orthographic::GenerateRay(int x, int y, Ray *r, const float *cam_jitter)
{
    float xc, yc;

    xc = realW + x + cam_jitter[0];
    yc = realH - y - cam_jitter[1];

    Point origin = Point(xc, yc, Eye.Z);

    // The direction is the same for all rays in an orthographic camera
    Vector direction = Vector(
        c2w[2][0],
        c2w[2][1],
        c2w[2][2]
    );

    *r = Ray(origin, direction);

    return true;
}
