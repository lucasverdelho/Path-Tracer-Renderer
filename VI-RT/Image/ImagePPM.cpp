//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>

void ImagePPM::ToneMap () {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }

}

bool ImagePPM::Save (std::string filename) {
    
    // convert from float to {0,1,..., 255}
    ToneMap();

    // write imageToSave to file
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // Need to specify binary mode for Windows users
        if (ofs.fail()) throw("Can't open output file");
        ofs << "P6\n" << W << " " << H << "\n255\n";
        for (int i = 0; i < W * H; ++i) {
            ofs.write(reinterpret_cast<char*>(imageToSave[i].val), 3); // Write each pixel
        }
        ofs.close();
    }
    catch (const char* err) {
        std::cerr << err << std::endl;
        ofs.close();
        return false;
    }
    return true;
    // Details and code on PPM files available at:
    // https://www.scratchapixel.com/lessons/digital-imaging/simple-image-manipulations/reading-writing-images.html
    
    return true;
}
