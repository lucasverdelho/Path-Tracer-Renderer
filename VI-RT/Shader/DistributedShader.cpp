#include "DistributedShader.hpp"
#include "Phong.hpp"
#include "ray.hpp"
#include "AreaLight.hpp"
#include <stdlib.h>
#include <math.h>

//#include "DEB.h"


// No one enjoys Windows

#ifdef _WIN32
    const float M_PI = 3.14159265358979323846;
    const float M_PI_2 = 1.57079632679489661923;
    const float MAXFLOAT = std::numeric_limits<float>::max();
#endif




RGB DistributedShader::area_light_shade(Intersection isect, Phong* f, Light* l){

    RGB color(0., 0., 0.);
    if (!f->Kd.isZero()) {


        RGB L, Kd = f->Kd;
        Point lpoint;

        float l_pdf;
        AreaLight *al = (AreaLight *)l;

        // float rnd[2];
        // rnd[0] = distribution(rng);
        // rnd[1] = distribution(rng);

        float rnd[2];
        rnd[0] = ((float)rand()) / ((float)RAND_MAX);
        rnd[1] = ((float)rand()) / ((float)RAND_MAX);

        L = al->Sample_L(rnd, &lpoint, l_pdf);

        // Compute the direction from the intersection to the light
        Vector Ldir = isect.p.vec2point(lpoint);
        const float Ldistance = Ldir.norm();
        Ldir.normalize();

        // Cosine between the direction to the light and the shading normal at the intersection
        float cosL = Ldir.dot(isect.sn);

        // Cosine between the direction to the light and the normal of the area light
        float cosL_LA = Ldir.dot(al->gem->normal);

        // Shade if light is not behind the primitive and light normal points to the ray origin
        if (cosL>0. && cosL_LA<=0.) {

            Ray shadow(isect.p, Ldir);

            shadow.adjustOrigin(isect.gn);

            if (scene -> visibility(shadow, Ldistance - EPSILON)) // Light Source not occluded
                color += (Kd * L * cosL) / l_pdf;
            
        }

    }

    return color;

}


RGB DistributedShader::point_light_shade(Intersection isect, Phong* f, Light* l) {

    RGB color(0.,0.,0.);

    if (!f->Kd.isZero()) {

        Point lpoint;

        // get the position and radiance of the light source
        RGB L = (l)->Sample_L(nullptr, &lpoint);

        // compute the direction from the intersection to the light
        Vector Ldir = isect.p.vec2point(lpoint);

        // Compute the distance between the intersection and the light source
        const float Ldistance = Ldir.norm();

        Ldir.normalize(); // now normalize Ldir

        // compute the cosine (Ldir , shading normal)
        float cosL = Ldir.dot(isect.sn);

        if (cosL>0.) { // the light is NOT behind the primitive

            // generate the shadow ray
            Ray shadow(isect.p, Ldir);

            // adjust origin EPSILON along the normal: avoid self occlusion
            shadow.adjustOrigin(isect.gn);

            if (scene->visibility(shadow, Ldistance-EPSILON)) // light source not occluded
                color += f->Kd * L * cosL;
                
        } // end cosL > 0.
        // the light is behind the primitive
    }
    return color;
}


RGB DistributedShader::directLighting(Intersection isect, Phong* f) {

    RGB color(0.,0.,0.);

    // Choose a light source at random, basic idea: all lights have the same probability
    int random_light_index = rand() % scene->numLights;
    
    Light* chosen_light = scene->lights[random_light_index];


    if (chosen_light->type == AMBIENT_LIGHT) {

        if (!f->Ka.isZero()) {
            RGB Ka = f->Ka;
            color += Ka * chosen_light->L();

        }
    } 
    

    else if (chosen_light->type == POINT_LIGHT) 
        color = point_light_shade(isect, f, chosen_light);



    else if (chosen_light->type == AREA_LIGHT)
        color = area_light_shade(isect, f, chosen_light);
    

    return color;
}



RGB DistributedShader::specularReflection (Intersection isect, Phong *f, int depth) {

    RGB color(0.,0.,0.);

    Vector Rdir, s_dir;
    float pdf;
    
    // generate the specular ray
    
    // IDEAL SPECULAR REFLECTION
    // direction R = 2 (N.V) N - V
    float cos = isect.gn.dot(isect.wo);
    Rdir = 2.f * cos * isect.gn - isect.wo;
    

    if (f->Ns < 1000) { // glossy materials
        // actual direction distributed around Rdir according to the cosine lobe
        // generate the cosine lobel sampled direction around (0,0,1)
        // following item (36) of the Global illumination compendium
        // get 2 random number in [0,1[
        float rnd[2];
        rnd[0] = ((float)rand()) / ((float)RAND_MAX);
        rnd[1] = ((float)rand()) / ((float)RAND_MAX);
        
        Vector S_around_N;
        const float cos_theta = powf(rnd[1], 1./(f->Ns+1.));
        S_around_N.Z = cos_theta;
        const float aux_r1 = powf(rnd[1], 2./(f->Ns+1.));
        S_around_N.Y = sinf(2.*M_PI*rnd[0])*sqrtf(1.-aux_r1);
        S_around_N.X = cosf(2.*M_PI*rnd[0])*sqrtf(1.-aux_r1);
        const float cos_pow = powf(cos_theta, f->Ns)/(2.f*M_PI);
        pdf = (f->Ns+1.f)*cos_pow;
        //pdf = 3*f->Ns/(f->Ns+2)*powf(cos_theta, f->Ns);
        //fprintf (stderr, "Ns=%.0f, cos=%.6f, cos^Ns=%.6f pdf=%.6f\n", f->Ns, cos_theta, powf(cos_theta, f->Ns), pdf);
        
        // generate a coordinate system from Rdir
        Vector Rx, Ry;
        Rdir.CoordinateSystem(&Rx, &Ry);
        
        s_dir = S_around_N.Rotate  (Rx, Ry, Rdir);
        
        Ray specular(isect.p, s_dir);
        
        specular.pix_x = isect.pix_x;
        specular.pix_y = isect.pix_y;
        
        specular.FaceID = isect.FaceID;

        specular.adjustOrigin(isect.gn);

        // OK, we have the ray : trace and shade it recursively
        bool intersected;
        Intersection s_isect;
        // trace ray
        intersected = scene->trace(specular, &s_isect);

        // shade this intersection
        RGB Rcolor = shade (intersected, s_isect, depth+1);
        
        //color = (f->Ks * cos_pow * Rcolor) /pdf ;
        color = (f->Ks  * Rcolor) /pdf ;
        return color;

    }
    else {          // ideal specular reflection
        Ray specular(isect.p, Rdir);
        
        specular.pix_x = isect.pix_x;
        specular.pix_y = isect.pix_y;
        
        specular.FaceID = isect.FaceID;

        specular.adjustOrigin(isect.gn);

        // OK, we have the ray : trace and shade it recursively
        bool intersected;
        Intersection s_isect;
        // trace ray
        intersected = scene->trace(specular, &s_isect);

        // shade this intersection
        RGB Rcolor = shade (intersected, s_isect, depth+1);
        
        color = (f->Ks  * Rcolor)  ;
        return color;
    }
    
}



RGB DistributedShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0.,0.,0.);
    
    // if no intersection, return background
    if (!intersected)
        return (background);
    
    
    if (isect.isLight)  // intersection with a light source
        return isect.Le;
    
    
    // get the BRDF
    Phong *f = (Phong *)isect.f;
    
    // if there is a specular component sample it
    if (!f->Ks.isZero() && depth < 4) {
        color += specularReflection (isect, f, depth+1);
    }
    
    // if there is a diffuse component do direct light
    if (!f->Kd.isZero()) {
        color += directLighting(isect, f);
    }

    //Se estiver demasiado escuro podemos aumentar a intensidade das cores com o gamma
    float gamma = 1.3;
    color.R = powf(color.R, 1./gamma);
    color.G = powf(color.G, 1./gamma);
    color.B = powf(color.B, 1./gamma);
        
    return color;
};
