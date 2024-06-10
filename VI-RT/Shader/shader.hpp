#ifndef shader_hpp
#define shader_hpp

#include "scene.hpp"
#include "RGB.hpp"

class Shader {

protected:

    Scene *scene;

public:

    Shader (Scene *_scene): scene(_scene) {}

    ~Shader () {}
    
    virtual RGB shade(bool intersected, Intersection isect, int depth) {
        return RGB(); // Default implementation without random number generation
    }

    virtual RGB shade(bool intersected, Intersection isect, int depth, std::default_random_engine& rng, std::uniform_real_distribution<float>& distribution) {
        return RGB(); // Placeholder implementation for shaders requiring random number generation
    }
};

#endif /* shader_hpp */
