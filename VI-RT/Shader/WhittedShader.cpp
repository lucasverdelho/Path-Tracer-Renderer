#include "WhittedShader.hpp"
#include "Phong.hpp"
#include "ray.hpp"

RGB WhittedShader::directLighting (Intersection isect, Phong *f) {
    RGB color(0.,0.,0.);
    
    // Loop over scene's light sources
    for (auto l = scene->lights.begin() ; l != scene->lights.end() ; l++) {
        
        if ((*l)->type == AMBIENT_LIGHT) {  // Ambient Light Source

            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                color += Ka * (*l)->L();
            }

            continue;
        }

        if ((*l)->type == POINT_LIGHT) {  // Point Light Source

            if (!f -> Kd.isZero()) { // Diffuse Component

                Point lpoint;

                // Get the position and radiance of the light source
                RGB L = (*l)->Sample_L(NULL, &lpoint);

                // Compute the direction from the intersection point to the light source
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();

                // Compute the cosine (Ldir, shading normal)
                float cosL = Ldir.dot(isect.sn);

                if (cosL > 0.) {

                    // Generate the new Shadow Ray
                    Ray shadow(isect.p, Ldir);

                    // Adjust origin EPSILON along the normal: avoid self occlusion
                    shadow.adjustOrigin(isect.gn);

                    if (scene -> visibility(shadow, Ldistance - EPSILON))  // Light Source
                        color += f -> Kd * L * cosL;

                } 
            }
        }          
        continue;
    }
    return color;
}

RGB WhittedShader::specularReflection (Intersection isect, Phong *f, int depth) {

    // Generate the Specular Ray
    float cos = isect.gn.dot(isect.wo);
    Vector Rdir = 2.f * cos * isect.gn - isect.wo;
    Ray specular(isect.p, Rdir);

    Intersection s_isect;

    // Trace the Specular Ray
    bool intersected = scene -> trace(specular, &s_isect);

    // Shade the intersection point
    RGB color = shade(intersected, s_isect, depth + 1);

    return color;
}

RGB WhittedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected) 
        return (background);
    
    
    // Get the BRDF
    Phong *f = (Phong *)isect.f;
    
    // if there is a specular component sample it
    if (!f->Ks.isZero() && depth < 3) 
        color += specularReflection (isect, f, depth+1);
    
    
    color += directLighting(isect, f);
        
    return color;
};
