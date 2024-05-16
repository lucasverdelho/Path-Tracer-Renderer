#ifndef PathTracerShader_hpp
#define PathTracerShader_hpp

#include "shader.hpp"
#include "Phong.hpp"

class PathTracerShader: public Shader {

    RGB background;
    
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int depth);
    RGB diffuseReflection (Intersection isect, Phong *f, int depth);

    float continue_p;
    int MAX_DEPTH;

public:

    PathTracerShader (Scene *scene, RGB bg): background(bg), Shader(scene) {continue_p = 0.5f; MAX_DEPTH=2;}

    RGB shade (bool intersected, Intersection isect, int depth);
};

#endif /* DistributedShader_hpp */
