#ifndef renderer_hpp
#define renderer_hpp

#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "shader.hpp"

class Renderer {
protected:

    Camera *cam;
    Scene *scene;
    Image * img;
    Shader *shd;

public:

    Renderer (Camera *cam, Scene * scene, Image * img, Shader *shd): cam(cam), scene(scene), img(img), shd(shd) {}
    
    virtual void Render () {}
};

#endif /* renderer_hpp */
