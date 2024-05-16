#ifndef ParallelRenderer_hpp
#define ParallelRenderer_hpp

#include "renderer.hpp"
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

class ParallelRenderer: public Renderer {
private:
    int spp;
public:
    ParallelRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd, int _spp): Renderer(cam, scene, img, shd) {
        spp = _spp;
    }
    void Render (int depth, bool jitter, int num_threads);
};

#endif /* ParallelRenderer_hpp */
