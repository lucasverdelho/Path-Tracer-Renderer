#ifndef ImagePPM_hpp
#define ImagePPM_hpp
#include "image.hpp"

class ImagePPM: public Image {

    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;

    PPM_pixel *imageToSave;

    void ToneMap ();
    
public:
    ImagePPM(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
};

#endif /* ImagePPM_hpp */
