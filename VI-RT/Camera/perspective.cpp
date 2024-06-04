#include "perspective.hpp"

Perspective::Perspective(const Point Eye, const Point At, const Vector Up, const int W, const int H, const float fovW, const float fovH) : Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH)
{

    // compute camera 2 world transform
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

bool Perspective::GenerateRay(int x, int y, Ray *r, const float *cam_jitter) {

    float xc, yc;

    // Apply the jitter to add noise (stochastically)
    if (cam_jitter==NULL) {
        xc = (2.0f * ((float)x + 0.5f) / W) - 1.0f;
        yc = (2.0f * ((float)(H - y - 1) + 0.5f) / H) - 1.0f;
    } else {
        xc = 2.f * ((float)x + cam_jitter[0])/W - 1.f;
        yc = 2.f * ((float)(H - y - 1) + cam_jitter[1])/H - 1.f;
    }


    xc *= tan(fovW / 2);
    yc *= tan(fovH / 2); 

    Vector direction = Vector(xc, yc, 1);

    Vector worldDirection = Vector(
            c2w[0][0] * direction.X + c2w[0][1] * direction.Y + c2w[0][2] * direction.Z,
            c2w[1][0] * direction.X + c2w[1][1] * direction.Y + c2w[1][2] * direction.Z,
            c2w[2][0] * direction.X + c2w[2][1] * direction.Y + c2w[2][2] * direction.Z
    );

    *r = Ray(Eye, worldDirection);

    return true;
}
