#include "primitive.hpp"


void Primitive::compute_light_weights(const std::vector<Light *> &lights) {
    Point middle = get_bbox_midpoint();
    Light *l;
    float weight = 0.0f;
    std::vector<float> lightWeights;
    lightWeights.resize(lights.size(), 0.0f);

    for (size_t i = 0; i < lights.size(); ++i) {
        l = lights[i];

        if (l->type == POINT_LIGHT) {
            PointLight *pointLight = static_cast<PointLight*>(l);
            
            // Access the position member of PointLight
            Point lightPos = pointLight->pos;
            float distance = (lightPos - middle).toVector().norm();
            weight = 1.0f / (distance * distance * distance);
        }
        else if (l->type == AREA_LIGHT) {
            AreaLight *areaLight = static_cast<AreaLight*>(l);
            Point lightPos = areaLight->gem->v1;
            float distance = (lightPos - middle).toVector().norm();
            weight = 1.0f / (distance * distance * distance);
        }
        else {
            std::cerr << "Error: Light source is null" << std::endl;
            continue;
        }
        // Store the computed weight in the vector
        lightWeights[i] = weight;
    }

    // Create a light distribution based on the lightWeights
    lightDistribution = std::discrete_distribution<int>(lightWeights.begin(), lightWeights.end());

    // Iterate over each index and print its probability
    for (size_t i = 0; i < lightWeights.size(); ++i) {
        std::cout << "Probability for index " << i << ": " 
                  << std::fixed << std::setprecision(6) << lightDistribution.probabilities()[i] << std::endl;
    }
}

// Implementation of the method to calculate the middle of the bounding box
Point Primitive::get_bbox_midpoint() const {
    float midX = (g->bb.min.X + g->bb.max.X) / 2.0f;
    float midY = (g->bb.min.Y + g->bb.max.Y) / 2.0f;
    float midZ = (g->bb.min.Z + g->bb.max.Z) / 2.0f;
    return Point(midX, midY, midZ);
}
