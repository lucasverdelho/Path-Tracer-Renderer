#include "primitive.hpp"

void Primitive::compute_light_weights(const std::vector<Light *> &lights) {

    lightWeights.clear();

    Point middle = get_bbox_midpoint();
    Light *l;
    float weight = 0.0f;

    for (size_t i = 0; i < lights.size(); ++i) {
        l = lights[i];

        if (l->type == POINT_LIGHT) {
            PointLight *pointLight = static_cast<PointLight*>(l);
            
            // Access the position member of PointLight
            Point lightPos = pointLight->pos;
            float distance = (lightPos - middle).toVector().norm();
            weight = 1.0f / (distance * distance);
            
        }
        else {
            std::cerr << "Error: Light source is null" << std::endl;
            continue;
        }
        
        std::cout << "Processing light ID: " << i << " with weight: " << weight << std::endl;

        // Store the light ID and its computed weight
        lightWeights[static_cast<int>(i)] = weight;
    }
}





// Implementation of the method to calculate the middle of the bounding box
Point Primitive::get_bbox_midpoint() const {
    float midX = (g->bb.min.X + g->bb.max.X) / 2.0f;
    float midY = (g->bb.min.Y + g->bb.max.Y) / 2.0f;
    float midZ = (g->bb.min.Z + g->bb.max.Z) / 2.0f;
    return Point(midX, midY, midZ);
}