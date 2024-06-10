#include "OptimizedParallelRenderer.hpp"

void displayProgressBar(int processed_blocks, int total_blocks);

void renderWorker(int start_x, int end_x, int start_y, int end_y, int spp, Camera* cam, Scene* scene, Shader* shd, Image* img, bool jitter, std::atomic<int>& progress_counter, int total_blocks) {
    // Print Information about this render block with the coordinates for debugging
    // printf("Rendering block: (%d, %d) to (%d, %d)\n", start_x, start_y, end_x, end_y);

    for (int y = start_y; y < end_y; y++) {      // loop over rows
        for (int x = start_x; x < end_x; x++) {        // loop over columns

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

    // Increment the progress counter and display the progress bar
    int processed_blocks = ++progress_counter;
    displayProgressBar(processed_blocks, total_blocks);
}


void displayProgressBar(int processed_blocks, int total_blocks) {
    float progress = (float)processed_blocks / total_blocks;
    int barWidth = 50;

    std::cout << "[";

    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else std::cout << " ";
    }

    std::cout << "] " << std::setw(3) << int(progress * 100.0) << " %\r";
    std::cout.flush();
}


void OptimizedParallelRenderer::Render(int depth, bool jitter, int num_threads) {

    int W = 0, H = 0;
    this->cam->getResolution(&W, &H);

    int block_size = 32;
    // Calculate the number of blocks in the x and y directions
    int num_blocks_x = (W + block_size - 1) / block_size; // Ceiling division
    int num_blocks_y = (H + block_size - 1) / block_size; // Ceiling division

    int total_blocks = num_blocks_x * num_blocks_y;
    

    // Create a queue of all the blocks that need to be rendered
    std::queue<std::tuple<int, int, int, int>> blocks_queue;
    for (int by = 0; by < num_blocks_y; by++) {
        for (int bx = 0; bx < num_blocks_x; bx++) {
            int start_x = bx * block_size;
            int end_x = std::min(start_x + block_size, W);
            int start_y = by * block_size;
            int end_y = std::min(start_y + block_size, H);
            blocks_queue.push(std::make_tuple(start_x, end_x, start_y, end_y));
        }
    }

    // Print all of the information about the render
    printf("Rendering %dx%d image with %d spp using %d threads\n and %d blocks of %dx%d pixels\n", W, H, spp, num_threads, total_blocks, block_size, block_size);

    // Create a mutex for thread synchronization
    std::mutex queue_mutex;

    // Atomic counter to track progress
    std::atomic<int> progress_counter(0);

    // Function to be executed by each thread
    auto thread_func = [&]() {
        while (true) {
            // Get the next block from the queue
            std::tuple<int, int, int, int> block;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (blocks_queue.empty()) {
                    break;
                }
                block = blocks_queue.front();
                blocks_queue.pop();
            }

            // Render the block
            renderWorker(std::get<0>(block), std::get<1>(block), std::get<2>(block), std::get<3>(block),
                         this->spp, this->cam, this->scene, this->shd, this->img, jitter, progress_counter, total_blocks);
        }
    };

    // Create and launch the threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(thread_func);
    }

    // Join the threads
    for (auto &thread : threads) {
        thread.join();
    }

    printf("\nRendering complete.\n");
}
