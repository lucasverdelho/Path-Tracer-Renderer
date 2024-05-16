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
    
    virtual RGB shade (bool intersected, Intersection isect, int depth) {return RGB();}
};

#endif /* shader_hpp */
