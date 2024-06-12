#include "primitive.hpp"

// Example implementation of the compute_light_weights method
void Primitive::compute_light_weights(const std::vector<Light *> &lights) {
    // Clear the existing weights
    lightWeights.clear();

    // Iterate over all lights and compute weights
    for (size_t i = 0; i < lights.size(); ++i) {
        // Placeholder logic for computing weights
        // Actual computation would depend on the Phong shading model
        // and the properties of each light source
        float weight = 1.0f;  // Example weight calculation
        std::cout << "Processing light ID: " << i << " with weight: " << weight << std::endl;

        // Store the light ID and its computed weight
        lightWeights[static_cast<int>(i)] = weight;
    }
}
