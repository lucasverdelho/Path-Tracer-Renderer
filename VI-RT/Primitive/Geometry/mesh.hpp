#ifndef mesh_hpp
#define mesh_hpp

#include "geometry.hpp"
#include "vector.hpp"
#include <vector>

// partially inspired in pbrt book (3rd ed.), sec 3.6, pag 152

typedef struct Face {
    int vert_ndx[3];            // indices to our internal vector of vertices (in Mesh)
    Vector geoNormal;           // geometric normal
    bool hasShadingNormals;     // are there per vertex shading normals ??
    int vert_normals_ndx[3];    // indices to vertices normals
    BB bb;			            // face Bounding Box
    int FaceID;
    Vector edge1, edge2;        // edges of the triangle   
                                // this is min={0.,0.,0.} , max={0.,0.,0.} due to the Point constructor
} Face;

class Mesh: public Geometry {

private:
    bool TriangleIntersect (Ray r, Face f, Intersection *isect);

public:

    int numFaces;
    std::vector<Face> faces;

    int numVertices;
    std::vector<Point> vertices;

    int numNormals;
    std::vector<Vector> normals;
    
    bool intersect (Ray r, Intersection *isect);
    
    Mesh(): numFaces(0), numVertices(0), numNormals(0) {}


    int get_index(Point vertex) {

        int res = -1;
        for(int ver = 0; ver < numVertices; ver++){
            if(vertex.X == vertices[ver].X && vertex.Y == vertices[ver].Y && vertex.Z == vertices[ver].Z) res = ver;
        }

        return res;
    }
};

#endif /* mesh_hpp */
