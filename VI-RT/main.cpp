#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <iomanip> // for std::setw
#include <sstream> // for std::ostringstream
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
#include "scene.hpp"
#include <time.h>


#ifdef _WIN32
#include <direct.h> // For Windows
#define GetCurrentDir _getcwd
#else
#include <unistd.h> // For Unix systems
#define GetCurrentDir getcwd
#endif




#include <iomanip> // for std::setw
#include <sstream> // for std::ostringstream

void save_image(ImagePPM *img, std::string filename = "cornell_box_VI") {

    // Save the image in the rendered_images folder
    std::string save_path = "rendered_images/";

    save_path += filename;

    // Append Screen Resolution to the filename
    save_path += "_";
    save_path += std::to_string(img->getHeight());
    save_path += "x";
    save_path += std::to_string(img->getWidth());

    // Get Current day and time
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // Format date and time components into a string
    std::ostringstream date_time;
    date_time << "_"
              << std::setw(4) << std::setfill('0') << (1900 + ltm->tm_year) << "-"   // Year
              << std::setw(2) << std::setfill('0') << (1 + ltm->tm_mon) << "-"       // Month
              << std::setw(2) << std::setfill('0') << ltm->tm_mday << "_"            // Day
              << std::setw(2) << std::setfill('0') << ltm->tm_hour << "-"            // Hour
              << std::setw(2) << std::setfill('0') << ltm->tm_min;                   // Minute

    // Append the formatted date and time to the filename
    save_path += date_time.str();

    save_path += ".ppm";

    if (img->Save(save_path)) {
        std::cout << "Image saved successfully.\n";
    }
    else {
        std::cout << "Error: Image not saved.\n";
    }
}



// Function that will store inside the input full_path variable the current working directory
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

        // Output the result
        std::cout << "--------------------------------------------------\n";
        std::cout << "Full Path:\n" << full_path << std::endl;
        std::cout << "--------------------------------------------------\n";
        return 1;
    }
    else // The input object/scene file is not found
    {
        std::cerr << "Error: Unable to locate the object." << std::endl;
        return 0;
    }
}


// Function to add area lights to the scene
void add_area_lights(Scene &scene)
{
    // Add first triangle of square light in the middle
    AreaLight *area_light_1 = new AreaLight(
        RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
        Point(213, 548, 227),     
        Point(343, 548, 227),       
        Point(343, 548, 332),     
        Vector(0, -1, 0)               // Normal vector of the area light triangle
    );

    scene.lights.push_back(area_light_1);
    scene.numLights++;

    // Add second triangle of square light in the middle
    AreaLight *area_light_2 = new AreaLight(
        RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
        Point(213, 548, 227),     
        Point(343, 548, 332),       
        Point(213, 548, 332),     
        Vector(0, -1, 0)               // Normal vector of the area light triangle
    );

    scene.lights.push_back(area_light_2);
    scene.numLights++;


    
    // // More Lights for Testing
    // AreaLight *area_light_3 = new AreaLight(
    //     RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
    //     Point(50, 548, 70),     
    //     Point(180, 548, 70),       
    //     Point(180, 548, 170),     
    //     Vector(0, -1, 0)               // Normal vector of the area light triangle
    // );

    // scene.lights.push_back(area_light_3);
    // scene.numLights++;

    // // Add second triangle of square light in the middle
    // AreaLight *area_light_4 = new AreaLight(
    //     RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
    //     Point(50, 548, 70),     
    //     Point(180, 548, 170),       
    //     Point(50, 548, 170),     
    //     Vector(0, -1, 0)               // Normal vector of the area light triangle
    // );

    // scene.lights.push_back(area_light_4);
    // scene.numLights++;

    //     // More Lights for Testing
    // AreaLight *area_light_5 = new AreaLight(
    //     RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
    //     Point(400, 548, 420),     
    //     Point(530, 548, 420),       
    //     Point(530, 548, 570),     
    //     Vector(0, -1, 0)               // Normal vector of the area light triangle
    // );

    // scene.lights.push_back(area_light_5);
    // scene.numLights++;

    // // Add second triangle of square light in the middle
    // AreaLight *area_light_6 = new AreaLight(
    //     RGB(0.85, 0.85, 0.85),        // Power of the light (RGB)
    //     Point(400, 548, 420),     
    //     Point(530, 548, 570),       
    //     Point(400, 548, 570),     
    //     Vector(0, -1, 0)               // Normal vector of the area light triangle
    // );

    // scene.lights.push_back(area_light_6);
    // scene.numLights++;

}


// Function to add point lights to the scene
void add_point_lights(Scene &scene)
{
    // Add a point Light to the scene
    PointLight *point_light_1 = new PointLight(RGB(0.85, 0.85, 0.85), Point(284, 508, 284));
    scene.lights.push_back(point_light_1);
    scene.numLights++;

    // Add a point Light to the scene
    PointLight *point_light_2 = new PointLight(RGB(0.85, 0.85, 0.85), Point(284, 508, 286));
    scene.lights.push_back(point_light_1);
    scene.numLights++;

    // // Add a point Light to the scene
    // PointLight *point_light_3 = new PointLight(RGB(0.85, 0.85, 0.85), Point(282, 508, 284));
    // scene.lights.push_back(point_light_1);
    // scene.numLights++;

    // // Add a point Light to the scene
    // PointLight *point_light_4 = new PointLight(RGB(0.85, 0.85, 0.85), Point(282, 508, 286));
    // scene.lights.push_back(point_light_1);
    // scene.numLights++;


}


// Function to add Ambient lights to the scene
void add_ambient_lights(Scene &scene)
{
    // Add an ambient light to the scene
    AmbientLight ambient(RGB(0.05, 0.05, 0.05));
    scene.lights.push_back(&ambient);
    scene.numLights++;
}



// Function to render the scene
int render_scene(Scene &scene, 
                 ImagePPM *img, 
                 Camera *cam, 
                 Shader *shd, 
                 std::string filename = "cornell_box_VI", 
                 bool parallel = true, 
                 int spp = 32, 
                 int num_threads = 12) {

    printf("Scene: %s\n", filename.c_str());

    // Image resolution
    const int W = 1024;
    const int H = 1024;

    img = new ImagePPM(W, H);



    
    // -------------------------------------------------------------------------------------------- //
    // -------------------------/!\ TEMPORARILY HARDCODED CAMERA /!\------------------------------- //
    // -------------------------------------------------------------------------------------------- //

    // Camera parameters
    const Point Eye = {280, 275, -330}, At = {280, 265, 0};
    const Vector Up = {0, 1, 0};
    
    const float fovW = 90.f;                             // in degrees     
    const float fovH = fovW * (float)H / (float)W;       // in degrees                       
    const float fovWrad = fovW * 3.14f / 180.f;          // to radians
    const float fovHrad = fovH * 3.14f / 180.f;          // to radians

    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    // -------------------------------------------------------------------------------------------- //
    // -------------------------/!\ TEMPORARILY HARDCODED CAMERA /!\------------------------------- //
    // -------------------------------------------------------------------------------------------- //







    // Get current time
    auto start_cpu = std::clock();
    auto start_real = std::chrono::steady_clock::now();


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








    // Calculate CPU time
    double cpu_time_used = static_cast<double>(end_cpu - start_cpu) / CLOCKS_PER_SEC;

    // Calculate real time with timestamps
    std::chrono::duration<double> real_time_seconds = end_real - start_real;

    // Output results
    std::cout << "CPU Time Used: " << cpu_time_used << " seconds" << std::endl;
    std::cout << "Real Time Elapsed: " << real_time_seconds.count() << " seconds" << std::endl;






    save_image(img, filename);



    return 0;
}







// Function to load a custom scene
void load_custom_scene() {

    int camera_choice;
    int shader_choice;
    int scene_choice;

    Scene scene;
    Shader *shd;
    Camera *cam;
    ImagePPM *img;



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


    // Expor os shaders disponiveis
    std::cout << "\nSelect a shader:\n";
    std::cout << "1. Ambient Shader\n";
    std::cout << "2. Whitted Shader\n";
    std::cout << "3. Distributed Shader\n";
    std::cout << "4. Path Tracer Shader\n";

    // Ler a escolha do utilizador
    std::cin >> shader_choice;


    RGB background(0.05, 0.05, 0.55);
    switch (shader_choice) {
        case 1:
            // Ambient Shader
            shd = new AmbientShader(&scene, background);
            break;
        case 2:
            // Whitted Shader
            shd = new WhittedShader(&scene, background);
            break;
        case 3:
            // Distributed Shader
            shd = new DistributedShader(&scene, background);
            break;
        case 4:
            // Path Tracer Shader
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

    // vector to store file names
    std::vector<std::string> file_names;


    char buffer[FILENAME_MAX];

    GetCurrentDir(buffer, sizeof(buffer));

    std::string full_path = std::string(buffer) + "/VI-RT/Scene/tinyobjloader/models";

    // For loop to read all the .obj file names in the tinyobjloader/models folder and store them in a vector
    for (const auto & entry : std::filesystem::directory_iterator(full_path))
        if (entry.path().extension() == ".obj") {
            file_names.push_back(entry.path().filename().string());
        }

    // Print the names of the files to the user
    for (int i = 0; i < file_names.size(); i++) {
        std::cout << i+1 << ". " << file_names[i] << "\n";
    }

    // Ler a escolha do utilizador
    std::cin >> scene_choice;
    std::string selected_file = file_names[scene_choice-1];

    if (get_working_directory(full_path, selected_file) == 0) 
        return;


    // Attempt to load the scene
    if(!scene.Load(full_path))
    {
        std::cout << "Loading the scene failed.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    scene.printSummary();


    // Add lights to the scene
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


    render_scene(scene, img, cam, shd, selected_file, parallel, spp, num_threads);


    std::cout << "Program finished successfully.\n";
    std::cout << "--------------------------------------------------\n";

    return;

}


// Function to load the default scene
void load_default_scene() {

    Perspective *cam; // Camera
    ImagePPM *img;    // Rendered Image
    Shader *shd;
    Scene scene;

    // Ambient Shader
    RGB background(0.05, 0.05, 0.55);

    // Path Tracer Shader
    shd = new PathTracerShader(&scene, background);

    // Get the current working directory ny storing it in the full_path variable
    std::string full_path;

    if (get_working_directory(full_path) == 0) 
        return; 


    // Attempt to load the scene
    if(!scene.Load(full_path))
    {
        std::cout << "Loading the scene failed.\n";
        std::cout << "--------------------------------------------------\n";
        return;
    }

    scene.printSummary();


    // Add lights to the scene
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