#ifndef Scene_hpp
#define Scene_hpp

#include "BRDF.hpp"
#include "intersection.hpp"
#include "light.hpp"
#include "primitive.hpp"
#include "ray.hpp"
#include "AreaLight.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>


class Scene
{
    std::vector<Primitive *> prims;
    std::vector<BRDF *> BRDFs;

public:
    std::vector<Light *> lights;
    
    int numPrimitives, numLights, numBRDFs;

    Scene() : numPrimitives(0), numLights(0), numBRDFs(0) {}

    bool Load(const std::string &fname);

    bool SetLights(void) { return true; };

    bool trace(Ray r, Intersection *isect);

    bool visibility(Ray s, const float maxL);

    void printSummary(void)
    {
        std::cout << "Scene Summary:" << std::endl;
        std::cout << "  # Primitives: " << numPrimitives << std::endl;
        std::cout << "  # Lights: " << numLights << std::endl;
        std::cout << "  # Materials: " << numBRDFs << std::endl;
        std::cout << "--------------------------------------------------\n";
    }
};

#endif /* Scene_hpp */