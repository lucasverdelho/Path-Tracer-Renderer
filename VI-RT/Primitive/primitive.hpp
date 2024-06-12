#ifndef primitive_hpp
#define primitive_hpp

#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>
#include "PointLight.hpp"
#include "AreaLight.hpp"

struct Primitive {
    Geometry *g;
    int material_ndx;

    // Map to store light IDs and their weights
    std::unordered_map<int, float> lightWeights;

    // Method to compute light weights used for selecting lights stochasticly
    void compute_light_weights(const std::vector<Light *> &lights);

    // Method to calculate the center point of the bounding box 
    // necessary to calculate the distance to the light sources
    Point get_bbox_midpoint() const;
};

#endif /* primitive_hpp */
