#ifndef BB_hpp
#define BB_hpp

#include "ray.hpp"
#include "vector.hpp"
#include <algorithm>




// No one enjoys Windows

#ifdef _WIN32
    const float MAXFLOAT = std::numeric_limits<float>::max();
#endif




typedef struct BB
{
public:
    Point min, max;
    
    void update(Point p)
    {
        if (p.X < min.X)
            min.X = p.X;

        else if (p.X > max.X)
            max.X = p.X;

        if (p.Y < min.Y)
            min.Y = p.Y;

        else if (p.Y > max.Y)
            max.Y = p.Y;

        if (p.Z < min.Z)
            min.Z = p.Z;

        else if (p.Z > max.Z)
            max.Z = p.Z;
    }

    // We chose to use the algortithm from Nvidia's RAY AXIS-ALIGNED BOUNDING BOX INTERSECTION
    /*
        Shirley, P., Wald, I., Marrs, A. (2021).
        Ray Axis-Aligned Bounding Box Intersection.
        Ray Tracing Gems II. Apress, Berkeley, CA.
    */

    // TODO: WE DO NOT KNOW WHAT THE VALUES OF rayTmin and rayTmax are
    bool intersect(Ray r)
    {
        float rayTmax = MAXFLOAT;
        float rayTmin = 0.0f;

        Vector p0Dir = (r.o).vec2point(this->min);
        Vector p1Dir = (r.o).vec2point(this->max);

        Vector invRayDir = r.invDir;
        
        // Vector tLower = p0Dir * invRayDir;
        Vector tLower = {p0Dir.X * invRayDir.X, p0Dir.Y * invRayDir.Y, p0Dir.Z * invRayDir.Z};
        Vector tUpper = {p1Dir.X * invRayDir.X, p1Dir.Y * invRayDir.Y, p1Dir.Z * invRayDir.Z};

        // The four t-intervals (for x-/y-/z-slabs , and ray p(t))
        // Compute minimum and maximum t-values for each axis individually

        float tMinX = std::min(tLower.X, tUpper.X);
        float tMinY = std::min(tLower.Y, tUpper.Y);
        float tMinZ = std::min(tLower.Z, tUpper.Z);
        float tMaxX = std::max(tLower.X, tUpper.X);
        float tMaxY = std::max(tLower.Y, tUpper.Y);
        float tMaxZ = std::max(tLower.Z, tUpper.Z);

        // Compute minimum and maximum t-values overall
        float tMin = std::max({tMinX, tMinY, tMinZ, rayTmin});
        float tMax = std::min({tMaxX, tMaxY, tMaxZ, rayTmax});

        return tMin <= tMax;
    }

} BB;

#endif /* AABB_hpp */
