#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Phong.hpp"
#include "mesh.hpp"
#include "primitive.hpp"
#include "tiny_obj_loader.h"

#include <iostream>
#include <set>
#include <vector>

using namespace tinyobj;

static void PrintInfo(const ObjReader myObj)
{
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;

    // Iterate shapes
    auto it_shape = shapes.begin();
    for (; it_shape != shapes.end(); it_shape++)
    {
        // assume each face has 3 vertices
        // std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for (; it_vertex != it_shape->mesh.indices.end();)
        {
            // process 3 vertices
            for (int v = 0; v < 3; v++)
            {
                // std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            // std::cout << std::endl;
        }
        // std::cout << std::endl;

        // printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
    }
}






bool Scene::Load(const std::string &fname)
{

    ObjReader myObjReader;

    int FaceID = 0;

    if (!myObjReader.ParseFromFile(fname))
    {
        printf("Error loading file\n");
        return false;
    }

    PrintInfo(myObjReader);

    
    const std::vector<material_t> materials = myObjReader.GetMaterials();
    for (auto it = materials.begin(); it != materials.end(); it++)
    {
        Phong *mat = new Phong;
        
        // Ka
        mat->Ka.R = it->ambient[0];
        mat->Ka.G = it->ambient[1];
        mat->Ka.B = it->ambient[2];

        // Kd
        mat->Kd.R = it->diffuse[0];
        mat->Kd.G = it->diffuse[1];
        mat->Kd.B = it->diffuse[2];

        // Ns
        mat->Ns = it->shininess;

        // Ks
        mat->Ks.R = it->specular[0];
        mat->Ks.G = it->specular[1];
        mat->Ks.B = it->specular[2];

        // Kt
        mat->Kt.R = it->transmittance[0];
        mat->Kt.G = it->transmittance[1];
        mat->Kt.B = it->transmittance[2];

        BRDFs.push_back(mat);
        numBRDFs++;
    }

    // Access the vertices
    const tinyobj::attrib_t attrib = myObjReader.GetAttrib();
    const float *vtcs = attrib.vertices.data();

    // Access the shapes (each shape is one mesh)
    const std::vector<shape_t> shapes = myObjReader.GetShapes();

    // Iterate over shapes (meshes)
    for (auto shp = shapes.begin(); shp != shapes.end(); shp++)
    {

        size_t index_offset = 0;
        Primitive *p = new Primitive;

        Mesh *m = new Mesh;
        p->g = m;

        // Assume all faces in the mesh have the same material
        p->material_ndx = shp->mesh.material_ids[0];

        // The primitive's geometry bounding box is computed on the fly
        // Initially set BB.min and BB.max to be the first vertex
        const int V1st = shp->mesh.indices.begin()->vertex_index * 3;

        m->bb.min.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);
        m->bb.max.set(vtcs[V1st], vtcs[V1st + 1], vtcs[V1st + 2]);

        // Add faces and vertices
        std::set<std::pair<int, int>> vert_rehash;

        for (auto v_it = shp->mesh.indices.begin(); v_it != shp->mesh.indices.end();)
        {
            Face *f = new Face;
            Point myVtcs[3];

            // Process 3 vertices
            for (int v = 0; v < 3; v++)
            {

                tinyobj::index_t idx = shp->mesh.indices[index_offset + v];

                const int objNdx = v_it->vertex_index;

                myVtcs[v].set(vtcs[objNdx * 3], vtcs[objNdx * 3 + 1], vtcs[objNdx * 3 + 2]);

                if (v == 0)
                {
                    f->bb.min.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                    f->bb.max.set(myVtcs[0].X, myVtcs[0].Y, myVtcs[0].Z);
                }
                else
                    f->bb.update(myVtcs[v]);

                // add vertex to mesh if new
                std::pair<int, int> new_vert = {objNdx, 0}; // Create a pair

                auto known_vert = vert_rehash.find(new_vert);

                if (known_vert == vert_rehash.end())
                {                                     // new vertex, add it to the mesh
                    new_vert.second = m->numVertices; // Update the second element of the pair
                    vert_rehash.insert(new_vert);
                    m->vertices.push_back(myVtcs[v]);
                    m->numVertices++;

                    // register in the face
                    f->vert_ndx[v] = new_vert.second;
                    m->bb.update(myVtcs[v]);
                }
                else
                    f->vert_ndx[v] = known_vert->second;

                // if (v_it.normal_index)

                // next vertex within this face (there are 3)
                v_it++;

            } // end vertices

            // WE ARE TEMPORARILY USING GEOMETRIC NORMAL INSTEAD OF SHADING NORMAL

            // Add face to mesh: compute the geometric normal
            // Compute the edges of the triangle
            Vector v1 = myVtcs[0].vec2point(myVtcs[1]);
            Vector v2 = myVtcs[0].vec2point(myVtcs[2]);
            f->edge1 = v1;
            f->edge2 = v2;

            // Computing the normal of the face
            Vector normal = v1.cross(v2);
            normal.normalize();

            f->geoNormal.set(normal);

            f->FaceID = FaceID++;
            f->hasShadingNormals = false;
            // Add face to mesh
            m->faces.push_back(*f);
            m->numFaces++;

            index_offset += 3;
        } // End iterate vertices in the mesh (shape)

        // Add primitive to scene
        prims.push_back(p);
        numPrimitives++;
    } // End iterate over shapes

    std::cout << "Scene loaded successfully\n";
    std::cout << "--------------------------------------------------\n";
    return true;
}




bool Scene::trace(Ray r, Intersection *isect){
    Intersection curr_isect;
    bool intersection = false;

    if (numPrimitives == 0)
        return false;

    // Iterate over all primitives
    for (auto prim_itr = prims.begin(); prim_itr != prims.end(); prim_itr++)
    {
        if ((*prim_itr)->g->intersect(r, &curr_isect))
        {
            if (!intersection)
            { // First intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth)
            {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
        }
    }

    isect->isLight = false;

    // Iterate over all the lights
    for (auto l = lights.begin(); l != lights.end(); l++)
    {
        if ((*l)->type == AREA_LIGHT)
        {
            AreaLight *al = (AreaLight *)*l;

            if (al->gem->intersect(r, &curr_isect))
            {
                if (!intersection)
                { // First intersection
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
                else if (curr_isect.depth < isect->depth)
                {
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
            }
        }
    }

    return intersection;

}



bool Scene::trace(Ray r, Intersection *isect, std::discrete_distribution<int> *lightDistribution)
{
    Intersection curr_isect;
    bool intersection = false;

    if (numPrimitives == 0)
        return false;

    // Iterate over all primitives
    for (auto prim_itr = prims.begin(); prim_itr != prims.end(); prim_itr++)
    {
        if ((*prim_itr)->g->intersect(r, &curr_isect))
        {
            if (!intersection)
            { // First intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];

                // Copy light distribution of the intersected primitive
                if (lightDistribution != nullptr)
                {
                    *lightDistribution = (*prim_itr)->lightDistribution;
                }
            }
            else if (curr_isect.depth < isect->depth)
            {
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];

                // Copy light distribution of the intersected primitive
                if (lightDistribution != nullptr)
                {
                    *lightDistribution = (*prim_itr)->lightDistribution;
                }
            }
        }
    }

    isect->isLight = false;

    // Iterate over all the lights
    for (auto l = lights.begin(); l != lights.end(); l++)
    {
        if ((*l)->type == AREA_LIGHT)
        {
            AreaLight *al = (AreaLight *)*l;

            if (al->gem->intersect(r, &curr_isect))
            {
                if (!intersection)
                { // First intersection
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
                else if (curr_isect.depth < isect->depth)
                {
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                }
            }
        }
    }

    return intersection;
}







// checks whether a point on a light source (distance maxL) is visible
bool Scene::visibility(Ray s, const float maxL)
{
    bool visible = true;
    Intersection curr_isect;

    if (numPrimitives == 0)
        return true;

    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin(); prim_itr != prims.end() && visible; prim_itr++){

        // if the ray intersects a primitive and the depth is less than maxL, the point is not visible
        if ((*prim_itr)->g->intersect(s, &curr_isect)){

            if (curr_isect.depth < maxL)
                visible = false;
        }
    }

    return visible;
}