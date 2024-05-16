#include "ParallelRenderer.hpp"

void renderWorker(int start, int end, int W, int spp, Camera* cam, Scene* scene, Shader* shd, Image* img, bool jitter) {

    printf("Thread %d rendering rows %d to %d\n", std::this_thread::get_id(), start, end);

    for (int y = start; y < end; y++) {      // loop over rows
        for (int x = 0; x < W; x++) {        // loop over columns

            RGB color(0., 0., 0.);
            Ray primary;
            Intersection isect;
            bool intersected;

            if (jitter) {
                for (int ss = 0; ss < spp; ss++) {

                    // Array to store the jitter values (random offsets) for the ray
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

                // Trace Ray (scene
                intersected = scene->trace(primary, &isect);

                // Shade this intersection (shader) - remember: depth=0
                color = shd->shade(intersected, isect, 0);

            }


            // Write the result into the image frame buffer (image)
            img->set(x, y, color);

        }
    }
}



void ParallelRenderer::Render(int depth, bool jitter, int num_threads) {

    // Canvas resolution
    int W = 0, H = 0;

    // Get resolution from the camera
    this->cam->getResolution(&W, &H);




    // Create a vector of threads and reserve the intended number of threads
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    printf("Rendering with %d threads\n", num_threads);
    



    // Dividing the rows among the threads evenly since the workload is uniform (I think)
    int rows_per_thread = H / num_threads;

    for (int thread_counter = 0; thread_counter < num_threads; thread_counter++) {

        int start_row = thread_counter * rows_per_thread;

        // The last thread will handle the remaining rows until the end of the image
        int end_row;
        if (thread_counter == num_threads - 1) 
            end_row = H;
        else 
            end_row = start_row + rows_per_thread; 
        

        threads.emplace_back(renderWorker, start_row, end_row, W, spp, this->cam, this->scene, this->shd, this->img, jitter);
    }




    // join the threads (wait for them to complete)
    for (auto& thread : threads) {
        thread.join();
    }



    std::cout << std::endl;
}
