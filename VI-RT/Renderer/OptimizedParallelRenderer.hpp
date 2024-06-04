#ifndef OptimizedParallelRenderer_hpp
#define OptimizedParallelRenderer_hpp

#include "renderer.hpp"
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <cmath>

class OptimizedParallelRenderer: public Renderer {
private:
    int spp;
public:
    OptimizedParallelRenderer (Camera *cam, Scene * scene, Image * img, Shader *shd, int _spp): Renderer(cam, scene, img, shd) {
        spp = _spp;
    }
    void Render (int depth, bool jitter, int num_threads);
};

#endif /* OptimizedParallelRenderer_hpp */
