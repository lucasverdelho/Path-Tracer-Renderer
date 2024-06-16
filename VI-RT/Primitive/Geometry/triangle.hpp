#ifndef triangle_hpp
#define triangle_hpp

#include "geometry.hpp"
#include "vector.hpp"
#include <math.h>

class Triangle: public Geometry {
public:
    Point v1, v2, v3;
    Vector normal;           // geometric normal
    Vector edge1, edge2;
    BB bb;      // face bounding box
                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
    bool intersect (Ray r, Intersection *isect);
    bool isInside(Point p);
    
    Triangle(Point _v1, Point _v2, Point _v3, Vector _normal): v1(_v1), v2(_v2), v3(_v3), normal(_normal) {
        edge1 = v1.vec2point(v2);
        edge2 = v1.vec2point(v3);
        bb.min.set(v1.X, v1.Y, v1.Z);
        bb.max.set(v1.X, v1.Y, v1.Z);
        bb.update(v2);
        bb.update(v3);
    }
    
    // Heron's formula
    // https://www.mathopenref.com/heronsformula.html
    float area () {
        
        Vector cross = edge1.cross(edge2);

        double length = std::sqrt(cross.X * cross.X + cross.Y * cross.Y + cross.Z * cross.Z);

        double area = 0.5 * length;
        
        return area;

    }
    float points_area (Point v1, Point v2, Point v3) {
        
        Vector edge1 = v1.vec2point(v2);
        Vector edge2 = v1.vec2point(v3);
        
        Vector cross = edge1.cross(edge2);

        double length = std::sqrt(cross.X * cross.X + cross.Y * cross.Y + cross.Z * cross.Z);

        double area = 0.5 * length;
        
        return area;

    }

    Point centroid() {
        double centroidX = (v1.X + v2.X + v3.X) / 3.0;
        double centroidY = (v1.Y + v2.Y + v3.Y) / 3.0;
        double centroidZ = (v1.Z + v2.Z + v3.Z) / 3.0;
        return Point(centroidX, centroidY, centroidZ);
    }
};

#endif /* triangle_hpp */
