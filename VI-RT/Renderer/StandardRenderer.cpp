#include "StandardRenderer.hpp"

void StandardRenderer::Render(int depth, bool jitter)
{
    int W = 0, H = 0; // resolution
    int x, y;

    jitter = true; // We should add the option to turn this on/off in the main

    depth = 1;
    // get resolution from the camera
    cam->getResolution(&W, &H);

    printf("Rendering %d x %d image\n", W, H);
    printf("Samples per pixel = %d\n", spp);
    printf("--------------------------------------------------\n");


    // main rendering loop: get primary rays from the camera until done
    for (y = 0; y < H; y++)
    { // loop over rows
        for (x = 0; x < W; x++) { // loop over columns

            RGB color(0., 0., 0.);
            Ray primary;
            Intersection isect;
            bool intersected;


            // If jitter then we need to loop over the samples per pixel
            if (jitter) {
                for (int ss = 0; ss < spp ; ss++) {
                    float jitterV[2];
                    jitterV[0] = (float)rand() / (float)RAND_MAX;
                    jitterV[1] = (float)rand() / (float)RAND_MAX;
                    cam->GenerateRay(x, y, &primary, jitterV);

                    // Trace Ray (scene)
                    intersected = scene->trace(primary, &isect);

                    // Shade this intersection (shader) - remember: depth=0
                    color += shd->shade(intersected, isect, 0);
                }
                
                // Divide by the number of samples per pixel
                color = color / spp; 
            }
            else {
                cam->GenerateRay(x, y, &primary, NULL);

                // Trace Ray (scene)
                intersected = scene->trace(primary, &isect);

                // Shade this intersection (shader) - remember: depth=0
                color = shd->shade(intersected, isect, 0);
            }

            // Write the result into the image frame buffer (image)
            img->set(x, y, color);
            
        } // loop over columns
    }     // loop over rows
}
