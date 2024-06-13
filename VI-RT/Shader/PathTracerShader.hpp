#ifndef PathTracerShader_hpp
#define PathTracerShader_hpp

#include "shader.hpp"
#include "Phong.hpp"

class PathTracerShader: public Shader {

    RGB background;
  
    RGB directLighting (Intersection isect, 
                        Phong *f, 
                        std::default_random_engine& rng, 
                        std::uniform_real_distribution<float>& distribution,
                        std::discrete_distribution<int> lightDistribution) ;

    RGB specularReflection (Intersection isect, 
                            Phong *f, 
                            int depth, 
                            std::default_random_engine& rng, 
                            std::uniform_real_distribution<float>& distribution) ;

    RGB diffuseReflection (Intersection isect, 
                           Phong *f, 
                           int depth, 
                           std::default_random_engine& rng, 
                           std::uniform_real_distribution<float>& distribution) ;

    float continue_p;
    int MAX_DEPTH;

public:

    PathTracerShader (Scene *scene, RGB bg): background(bg), Shader(scene) {continue_p = 0.5f; MAX_DEPTH=3;}

    // RGB shade (bool intersected, 
    //            Intersection isect, 
    //            int depth, 
    //            std::default_random_engine& rng, 
    //            std::uniform_real_distribution<float>& distribution);

    RGB shade (bool intersected,   
               Intersection isect, 
               int depth, 
               std::default_random_engine& rng, 
               std::uniform_real_distribution<float>& distribution, 
               std::discrete_distribution<int> lightDistribution);
};

#endif /* DistributedShader_hpp */
