#ifndef primitive_hpp
#define primitive_hpp

#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>
#include "light.hpp"

struct Primitive {
    Geometry *g;
    int material_ndx;

    // Map to store light IDs and their weights
    std::unordered_map<int, float> lightWeights;

    // Method to compute light weights
    void compute_light_weights(const std::vector<Light *> &lights);
};

#endif /* primitive_hpp */
