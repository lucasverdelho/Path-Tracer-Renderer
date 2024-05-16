#include "mesh.hpp"
#include "iostream"
#include "triangle.hpp"

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
//
// Suggestion: use:
// // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// Moller Trumbore intersection algorithm

#ifdef _WIN32
const float MAXFLOAT = std::numeric_limits<float>::max();
#endif

// We used Moller-Trumbore intersection algorithm
bool Mesh::TriangleIntersect(Ray r, Face f, Intersection *isect)
{

    Point vertex1 = vertices[f.vert_ndx[0]];
    Point vertex2 = vertices[f.vert_ndx[1]];
    Point vertex3 = vertices[f.vert_ndx[2]];
    Vector normal;

    // Interpolated normal
    if (f.hasShadingNormals)
    {
        Vector normal_vertex1 = normals[f.vert_normals_ndx[0]];
        Vector normal_vertex2 = normals[f.vert_normals_ndx[1]];
        Vector normal_vertex3 = normals[f.vert_normals_ndx[2]];

        printf("CALCULATING TRIANGLE INTERSECTION\n");

        normal = normal_vertex1 + normal_vertex2 + normal_vertex3;
        normal.normalize();
    }
    else
        normal = f.geoNormal;

    Triangle triangle = Triangle(vertex1, vertex2, vertex3, normal);
    return triangle.intersect(r, isect);
}

bool Mesh::intersect(Ray r, Intersection *isect)
{
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth = MAXFLOAT;

    // intersect the ray with the mesh BB
    if (!bb.intersect(r))
        return false;

    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it = faces.begin(); face_it != faces.end(); face_it++)
    {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face)
            continue;

        intersect = true;
        if (curr_isect.depth < min_depth)
        { // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }

    if (intersect)
        *isect = min_isect;

    return intersect;
}
