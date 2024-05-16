#ifndef image_hpp
#define image_hpp

#include "RGB.hpp"
#include <string>
#include <cstring>

class Image {

protected:

    RGB *imagePlane;
    int W,H;
public:
    Image(): W(0),H(0),imagePlane(NULL) {}

    Image(const int W, const int H): W(W),H(H) {
        imagePlane = new RGB[W*H];
        memset((void *)imagePlane, 0, W*H*sizeof(RGB));  // set image plane to 0
    }

    ~Image() {
        if (imagePlane!=NULL) delete[] imagePlane;
    }

    int getHeight() const { return H; } // Getter method for H

    int getWidth() const { return W; } // Getter method for W

    bool set (int x, int y, const RGB &rgb) {
        if (x>W || y>H) return false;
        imagePlane[y*W+x] = rgb;
        return true;
    }

    bool add (int x, int y, const RGB &rgb) {
        if (x>W || y>H) return false;
        imagePlane[y*W+x] += rgb;
        return true;
    }
    
    bool Save (std::string filename) {return true;}
};


#endif /* image_hpp */
