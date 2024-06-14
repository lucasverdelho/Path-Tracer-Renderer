#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <iomanip> 
#include <sstream> 
#include "AmbientLight.hpp"
#include "AmbientShader.hpp"
#include "AreaLight.hpp"
#include "DistributedShader.hpp"
#include "ImagePPM.hpp"
#include "PathTracerShader.hpp"
#include "PointLight.hpp"
#include "StandardRenderer.hpp"
#include "ParallelRenderer.hpp"
#include "OptimizedParallelRenderer.hpp"
#include "WhittedShader.hpp"
#include "perspective.hpp"
#include "orthographic.hpp"
#include "scene.hpp"
#include <time.h>


#ifdef _WIN32

#include <direct.h> // For Windows
#define GetCurrentDir _getcwd
    const float M_PI = 3.14159265358979323846;
    const float M_PI_2 = 1.57079632679489661923;
    const float MAXFLOAT = std::numeric_limits<float>::max();

#else

#include <unistd.h> // For Unix systems
#define GetCurrentDir getcwd

#endif


void save_image(ImagePPM *img, std::string filename, std::chrono::duration<double> real_time_seconds) {

    std::string save_path = "rendered_images/optimising/";

    save_path += filename;

    save_path += "_";
    save_path += std::to_string(img->getHeight());
    save_path += "x";
    save_path += std::to_string(img->getWidth());

   // Format the rendering time
    double render_time_seconds = real_time_seconds.count();
    std::ostringstream render_time_stream;
    render_time_stream << std::fixed << std::setprecision(2) << render_time_seconds;

    save_path += "_";
    save_path += render_time_stream.str();
    save_path += "s"; // Indicating seconds


    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::ostringstream date_time;
    date_time << "_"
              << std::setw(4) << std::setfill('0') << (1900 + ltm->tm_year) << "-"   // Year
              << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"       // Month
              << std::setw(2) << std::setfill('0') << ltm->tm_mday << "_"            // Day
              << std::setw(2) << std::setfill('0') << ltm->tm_hour << "-"            // Hour
              << std::setw(2) << std::setfill('0') << ltm->tm_min;                   // Minute

    save_path += date_time.str();
    save_path += ".ppm";

    if (img->Save(save_path)) {
        std::cout << "Image saved successfully.\n";
    }
    else {
        std::cout << "Error: Image not saved.\n";
    }
}



int get_working_directory(std::string &full_path, std::string selected_file = "")
{
    printf("Full Path: %s\n", full_path.c_str());

    char buffer[FILENAME_MAX];

    if (GetCurrentDir(buffer, sizeof(buffer)) != nullptr)
    {
        std::string current_path(buffer);

        const char *file_path;

        // Define the file path to concatenate
        if (selected_file == "") {
            file_path = "/VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj"; 
            full_path = current_path + file_path;
        }

        else {
            file_path = selected_file.c_str();
            full_path += std::string("/") + file_path;
        }

        std::cout << "--------------------------------------------------\n";
        std::cout << "Full Path:\n" << full_path << std::endl;
        std::cout << "--------------------------------------------------\n";
        return 1;
    }
    else 
    {
        std::cerr << "Error: Unable to locate the object." << std::endl;
        return 0;
    }
}


void add_area_lights(Scene &scene)
{
    // Add first triangle of square light in the middle
    AreaLight *area_light_1 = new AreaLight(
        RGB(1, 1, 1),        // Power of the light (RGB)
        Point(213, 548, 227),     
        Point(343, 548, 227),       
        Point(343, 548, 332),     
        Vector(0, -1, 0)               // Normal vector of the area light triangle
    );

    scene.lights.push_back(area_light_1);
    scene.numLights++;

    AreaLight *area_light_2 = new AreaLight(
        RGB(1, 1, 1),      
        Point(213, 548, 227),     
        Point(343, 548, 332),       
        Point(213, 548, 332),     
        Vector(0, -1, 0)             
    );

    scene.lights.push_back(area_light_2);
    scene.numLights++;

    /*
    // More Lights for Testing
    AreaLight *area_light_3 = new AreaLight(
        RGB(0.85, 0.85, 0.85),     
        Point(50, 548, 70),     
        Point(180, 548, 70),       
        Point(180, 548, 170),     
        Vector(0, -1, 0)           
    );

    scene.lights.push_back(area_light_3);
    scene.numLights++;

    AreaLight *area_light_4 = new AreaLight(
        RGB(0.85, 0.85, 0.85),      
        Point(50, 548, 70),     
        Point(180, 548, 170),       
        Point(50, 548, 170),     
        Vector(0, -1, 0)           
    );

    scene.lights.push_back(area_light_4);
    scene.numLights++;

        // More Lights for Testing
    AreaLight *area_light_5 = new AreaLight(
        RGB(0.85, 0.85, 0.85),      
        Point(400, 548, 420),     
        Point(530, 548, 420),       
        Point(530, 548, 570),     
        Vector(0, -1, 0)           
    );

    scene.lights.push_back(area_light_5);
    scene.numLights++;

    AreaLight *area_light_6 = new AreaLight(
        RGB(0.85, 0.85, 0.85),      
        Point(400, 548, 420),     
        Point(530, 548, 570),       
        Point(400, 548, 570),     
        Vector(0, -1, 0)           
    );

    scene.lights.push_back(area_light_6);
    scene.numLights++;
    */

}


// Function to add point lights to the scene
void add_point_lights(Scene &scene)
{
    // Add a point Light to the scene
    PointLight *point_light_1 = new PointLight(RGB(0.80, 0.80, 0.80), Point(20, 530, 280));
    scene.lights.push_back(point_light_1);
    scene.numLights++;

    // Add a point Light to the scene
    PointLight *point_light_2 = new PointLight(RGB(0.80, 0.80, 0.80), Point(280, 530, 20));
    scene.lights.push_back(point_light_2);
    scene.numLights++;

    // // Add a point Light to the scene
    PointLight *point_light_3 = new PointLight(RGB(0.80, 0.80, 0.80), Point(540, 530, 280));
    scene.lights.push_back(point_light_3);
    scene.numLights++;

    // Add a point Light to the scene
    PointLight *point_light_4 = new PointLight(RGB(0.80, 0.80, 0.80), Point(280, 530, 540));
    scene.lights.push_back(point_light_4);
    scene.numLights++;

    
    
    
    // // Add a point Light to the scene
    // PointLight *point_light_5 = new PointLight(RGB(0.80, 0.80, 0.80), Point(20, 530, 280));
    // scene.lights.push_back(point_light_5);
    // scene.numLights++;

    // // Add a point Light to the scene
    // PointLight *point_light_6 = new PointLight(RGB(0.80, 0.80, 0.80), Point(280, 530, 20));
    // scene.lights.push_back(point_light_6);
    // scene.numLights++;

    // // // Add a point Light to the scene
    // PointLight *point_light_7 = new PointLight(RGB(0.80, 0.80, 0.80), Point(540, 530, 280));
    // scene.lights.push_back(point_light_7);
    // scene.numLights++;

    // // Add a point Light to the scene
    // PointLight *point_light_8 = new PointLight(RGB(0.80, 0.80, 0.80), Point(280, 530, 540));
    // scene.lights.push_back(point_light_8);
    // scene.numLights++;

    
    
    
    
    for (int i = 0; i < 50; i++) {
        PointLight *point_light = new PointLight(RGB(0.80, 0.80, 0.80), Point(1000 + i * 50, 1000 + i * 50, 1000 + i * 50));
        scene.lights.push_back(point_light);
        scene.numLights++;
    }
    // // for loop to add more point lights in a line for testing the light selection algorithm
    // for (int i = 0; i < 10; i++) {
    //     PointLight *point_light = new PointLight(RGB(0.05, 0.05, 0.05), Point(184 + i * 50, i * 50 + 308, 384 + i * 50));
    //     scene.lights.push_back(point_light);
    //     scene.numLights++;
    // }

    // // for loop to add very far away point lights for testing the light selection algorithm
    // for (int i = 0; i < 20; i++) {
    //     PointLight *point_light = new PointLight(RGB(0.05, 0.05, 0.05), Point(1000 + i*1000, 1000 + i*1000, 1000 + i*1000));
    //     scene.lights.push_back(point_light);
    //     scene.numLights++;
    // }
}


void add_ambient_lights(Scene &scene)
{
    AmbientLight ambient(RGB(0.05, 0.05, 0.05));
    scene.lights.push_back(&ambient);
    scene.numLights++;
}


void pre_compute_light_weights(Scene &scene) {

    auto start = std::chrono::high_resolution_clock::now();

    int i = 0;
    
    for (auto prim_itr = scene.getPrimitives().begin(); prim_itr != scene.getPrimitives().end(); prim_itr++) {
        std::cout << "--------------------------------------------------\n";
        std::cout << "Computing light weights for primitive: " << i << std::endl;
        std::cout << "--------------------------------------------------\n";
        (*prim_itr)->compute_light_weights(scene.lights);
        i++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "--------------------------------------------------\n";
    std::cout << "Time taken to compute light weights for all primitives: " 
              << duration.count() << " seconds" << std::endl;
    std::cout << "--------------------------------------------------\n";
}




int render_scene(Scene &scene, 
                 ImagePPM *img, 
                 Camera *cam, 
                 Shader *shd, 
                 std::string filename = "cornell_box_VI", 
                 bool parallel = true, 
                 int spp = 32, 
                 int num_threads = 12, int W = 1024, int H = 1024) {

    printf("Scene: %s\n", filename.c_str());
    img = new ImagePPM(W, H);


    // -------------------------------------------------------------------------------------------- //
    // -------------------------/!\ TEMPORARILY HARDCODED CAMERA /!\------------------------------- //
    // -------------------------------------------------------------------------------------------- //

    // // Camera parameters
    // const Point Eye = {280, 275, -330}, At = {280, 265, 0};
    // const Vector Up = {0, 1, 0};

    // // Calculate the aspect ratio
    // const float aspectRatio = W / H;
    
    // const float fovW = 90.f;                         // horizontal field of view in degrees
    // const float fovH = fovW * (H / W);                // vertical field of view in degrees
    // const float fovWrad = fovW * (M_PI / 180.0f);     // to radians
    // const float fovHrad = fovH * (M_PI / 180.0f);     // to radians

    // cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);


    // Orthographic Camera
    const Point Eye = {400, 400, -700}, At = {278, 273, 0};
    const Vector Up = {0, 1, 0};

    cam = new Orthographic(Eye, At, Up, W, H);


    // -------------------------------------------------------------------------------------------- //
    // -------------------------/!\ TEMPORARILY HARDCODED CAMERA /!\------------------------------- //
    // -------------------------------------------------------------------------------------------- //

    auto start_cpu = std::clock();
    auto start_real = std::chrono::steady_clock::now();


    pre_compute_light_weights(scene);



    if (parallel) {
        // ParallelRenderer myRender(cam, &scene, img, shd, spp);
        OptimizedParallelRenderer myRender(cam, &scene, img, shd, spp);
        myRender.Render(0, true, num_threads);
    }

    else {
        StandardRenderer myRender(cam, &scene, img, shd, spp);
        myRender.Render(0, true);
    }

    auto end_cpu = std::clock();
    auto end_real = std::chrono::steady_clock::now();

    double cpu_time_used = static_cast<double>(end_cpu - start_cpu) / CLOCKS_PER_SEC;
    std::chrono::duration<double> real_time_seconds = end_real - start_real;

    std::cout << "CPU Time Used: " << cpu_time_used << " seconds" << std::endl;
    std::cout << "Real Time Elapsed: " << real_time_seconds.count() << " seconds" << std::endl;

    save_image(img, filename, real_time_seconds);

    return 0;
}








void load_custom_scene() {

    int camera_choice;
    int shader_choice;
    int scene_choice;

    Scene scene;
    Shader *shd;
    Camera *cam;
    ImagePPM *img;

    // -------------------------------------------------------------------------------------------- //
    // ------------------------------------Resolution Selection------------------------------------ //
    // -------------------------------------------------------------------------------------------- //

    int W, H;

    std::cout << "Enter the resolution of the image (W x H):\n";
    std::cin >> W >> H;

    // -------------------------------------------------------------------------------------------- //
    // ------------------------------------Camera Selection---------------------------------------- //
    // -------------------------------------------------------------------------------------------- //

    // Expor as camaras disponiveis
    std::cout << "Select a camera:\n";
    std::cout << "1. Perspective Camera\n";
    std::cout << "2. Orthographic Camera\n";
    std::cout << "3. Fisheye Camera\n";

    // Ler a escolha do utilizador
    std::cin >> camera_choice;

    // Camera TEMPORARILY hardcoded in render_scene function
    // switch (camera_choice) {
    //     case 1:
    //         // Perspective Camera
    //         break;
    //     case 2:
    //         // Orthographic Camera
    //         break;
    //     case 3:
    //         // Fisheye Camera
    //         break;
    //     default:
    //         std::cout << "Invalid Camera Choice. Exiting Program.\n";
    //         return;
    // }

    // -------------------------------------------------------------------------------------------- //
    // ------------------------------------Shader Selection---------------------------------------- //
    // -------------------------------------------------------------------------------------------- //

    std::cout << "\nSelect a shader:\n";
    std::cout << "1. Ambient Shader\n";
    std::cout << "2. Whitted Shader\n";
    std::cout << "3. Distributed Shader\n";
    std::cout << "4. Path Tracer Shader\n";

    std::cin >> shader_choice;


    RGB background(0.00, 0.00, 0.00);
    switch (shader_choice) {
        case 1:
            shd = new AmbientShader(&scene, background);
            break;
        case 2:
            shd = new WhittedShader(&scene, background);
            break;
        case 3:
            shd = new DistributedShader(&scene, background);
            break;
        case 4:
            shd = new PathTracerShader(&scene, background);
            break;
        default:
            std::cout << "Invalid Shader Choice. Exiting Load Custom Scene.\n";
            return;
    }


    // -------------------------------------------------------------------------------------------- //
    // ------------------------------------Scene Selection----------------------------------------- //
    // -------------------------------------------------------------------------------------------- //



    // Expor as cenas disponiveis (ficheiros .obj na pasta tinyobjloader/models)
    std::cout << "\nSelect a scene:\n";

    std::vector<std::string> file_names;

    char buffer[FILENAME_MAX];
    GetCurrentDir(buffer, sizeof(buffer));
    std::string full_path = std::string(buffer) + "/VI-RT/Scene/tinyobjloader/models";


    for (const auto & entry : std::filesystem::directory_iterator(full_path))
        if (entry.path().extension() == ".obj") {
            file_names.push_back(entry.path().filename().string());
        }


    for (int i = 0; i < file_names.size(); i++) {
        std::cout << i+1 << ". " << file_names[i] << "\n";
    }


    std::cin >> scene_choice;
    std::string selected_file = file_names[scene_choice-1];

    if (get_working_directory(full_path, selected_file) == 0) 
        return;

    if(!scene.Load(full_path))
    {
        std::cout << "Loading the scene failed.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    scene.printSummary();

    add_area_lights(scene);
    // add_point_lights(scene);


    // -------------------------------------------------------------------------------------------- //
    // -------------------------------------Render Scene------------------------------------------- //
    // -------------------------------------------------------------------------------------------- //


    int parallel_choice;
    int spp;
    int num_threads = 0;

    std::cout << "\nParallelize the rendering process?\n";
    std::cout << "1. Yes\n";
    std::cout << "2. No\n";

    std::cin >> parallel_choice;

    bool parallel;

    if (parallel_choice == 1) {

        std::cout << "Enter the number of threads:\n";
        std::cin >> num_threads;    
        parallel = true;
    }
    else 
        parallel = false;
    

    std::cout << "Enter the number of samples per pixel (spp):\n";
    std::cin >> spp;


    render_scene(scene, img, cam, shd, selected_file, parallel, spp, num_threads, W, H);


    std::cout << "Program finished successfully.\n";
    std::cout << "--------------------------------------------------\n";

    return;

}
















void load_default_scene() {

    Perspective *cam; 
    ImagePPM *img;   
    Shader *shd;
    Scene scene;

    RGB background(0.05, 0.05, 0.55);

    shd = new PathTracerShader(&scene, background);

    std::string full_path;
    if (get_working_directory(full_path) == 0) 
        return; 

    if(!scene.Load(full_path))
    {
        std::cout << "Loading the scene failed.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    scene.printSummary();

    add_area_lights(scene);
    // add_point_lights(scene);

    render_scene(scene, img, cam, shd);

    std::cout << "Program finished successfully.\n";
    std::cout << "--------------------------------------------------\n";
    
    return;
}






void print_cli_header() {
    std::cout << "--------------------------------------------------\n";
    std::cout << "╭━━━┳━━╮╱╱╱╱╱╱╭━━━╮╱╱╱╱╱╱╱╭╮\n";
    std::cout << "┃╭━╮┃╭╮┃╱╱╱╱╱╱┃╭━╮┃╱╱╱╱╱╱╱┃┃\n";
    std::cout << "┃╰━╯┃╰╯╰╮╱╱╱╱╱┃╰━╯┣━━┳━╮╭━╯┣━━┳━┳━━┳━╮\n";
    std::cout << "┃╭━━┫╭━╮┣━━┳━━┫╭╮╭┫┃━┫╭╮┫╭╮┃┃━┫╭┫┃━┫╭╯\n";
    std::cout << "┃┃╱╱┃╰━╯┣━━┻━━┫┃┃╰┫┃━┫┃┃┃╰╯┃┃━┫┃┃┃━┫┃\n";
    std::cout << "╰╯╱╱╰━━━╯╱╱╱╱╱╰╯╰━┻━━┻╯╰┻━━┻━━┻╯╰━━┻╯\n";
    std::cout << "--------------------------------------------------\n";
}


void print_cli_options() {
    std::cout << "Select an operation:\n";
    std::cout << "1. Load Default Scene (Cornell Box VI)\n";
    std::cout << "2. Load Custom Scene\n";
    std::cout << "3. Load Random Scene\n";
    std::cout << "4. Exit\n";
    std::cout << "--------------------------------------------------\n";
}


int main(int argc, const char *argv[])
{

    print_cli_header();

    int user_input;

    while(1){

        print_cli_options();
        std::cin >> user_input;

        switch (user_input) {

            case 1:
                std::cout << "Loading Default Scene (Cornell Box)...\n";
                load_default_scene();
                break;;
                
            case 2:
                std::cout << "Load Custom Scene...\n";
                load_custom_scene();
                break;;

            case 3:
                std::cout << "Load Random Scene...\n";
                break;;

            case 4:
                std::cout << "Exiting Program...\n";
                return 0;

            default:
                std::cout << "Invalid Input. Please try again.\n";
                continue;
        }
    }

    return 0;
}