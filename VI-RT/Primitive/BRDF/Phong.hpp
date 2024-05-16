#ifndef Phong_hpp
#define Phong_hpp

#include "RGB.hpp"
#include "BRDF.hpp"

class Phong: public BRDF {
public:

    RGB Ka, Kd, Ks, Kt;
    
    float Ns;
};

#endif /* Phong_hpp */
