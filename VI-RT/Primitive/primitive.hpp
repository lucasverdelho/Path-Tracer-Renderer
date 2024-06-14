#ifndef primitive_hpp
#define primitive_hpp

#include "Geometry/geometry.hpp"
#include "BRDF/BRDF.hpp"
#include <vector>
#include <iostream>
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include <stdlib.h>
#include <iomanip> 
#include <random> 

struct Primitive {
    Geometry *g;
    int material_ndx;

    // Discrete distribution for selecting lights stochastically
    std::discrete_distribution<int> lightDistribution;

    // Method to compute light weights used for selecting lights stochastically
    void compute_light_weights(const std::vector<Light *> &lights);

    // Method to calculate the center point of the bounding box 
    // necessary to calculate the distance to the light sources
    Point get_bbox_midpoint() const;
};

#endif /* primitive_hpp */
