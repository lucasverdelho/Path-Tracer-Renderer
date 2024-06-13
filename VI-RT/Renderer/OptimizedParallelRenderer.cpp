#include "OptimizedParallelRenderer.hpp"

void displayProgressBar(int processed_blocks, int total_blocks);

void renderWorker(int start_x, int end_x, int start_y, int end_y, int spp, Camera* cam, Scene* scene, Shader* shd, Image* img, bool jitter, std::atomic<int>& progress_counter, int total_blocks, std::default_random_engine& rng, std::uniform_real_distribution<float>& distribution) {
    
    for (int y = start_y; y < end_y; y++) {      // loop over rows
        for (int x = start_x; x < end_x; x++) {        // loop over columns

            RGB color(0., 0., 0.);
            Ray primary;
            Intersection isect;
            bool intersected;

            for (int ss = 0; ss < spp; ss++) {

                float jitterV[2];
                jitterV[0] = distribution(rng);
                jitterV[1] = distribution(rng);

                cam->GenerateRay(x, y, &primary, jitterV);

                // Declare a variable to store the map of lightWeights of the intersected object if it is a mesh
                std::vector<float> lightWeights;

                // Generate a ray and trace it
                intersected = scene->trace(primary, &isect, &lightWeights);

                std::discrete_distribution<int> lightDistribution(lightWeights.begin(), lightWeights.end());

                color += shd->shade(intersected, isect, 0, rng, distribution, lightDistribution);
            }

            color = color / spp;
            img->set(x, y, color);
        } 
    }

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

    int block_size = 64;
    
    int num_blocks_x = (W + block_size - 1) / block_size;
    int num_blocks_y = (H + block_size - 1) / block_size; 

    int total_blocks = num_blocks_x * num_blocks_y;
    

    // Create random number generator and uniform distribution
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);



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

    printf("Rendering %dx%d image with %d spp using %d threads\n and %d blocks of %dx%d pixels\n", W, H, spp, num_threads, total_blocks, block_size, block_size);

    std::mutex queue_mutex;
    std::atomic<int> progress_counter(0);

    auto thread_func = [&]() {
        while (true) {
            std::tuple<int, int, int, int> block;
            {
                std::lock_guard<std::mutex> lock(queue_mutex);
                if (blocks_queue.empty()) {
                    break;
                }
                block = blocks_queue.front();
                blocks_queue.pop();
            }
            renderWorker(std::get<0>(block), std::get<1>(block), std::get<2>(block), std::get<3>(block),
                         this->spp, this->cam, this->scene, this->shd, this->img, jitter, progress_counter, total_blocks, rng, distribution);
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(thread_func);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    printf("\nRendering complete.\n");
}
