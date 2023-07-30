#ifndef helper_h
#define helper_h

#include <iostream>

namespace aor2 {

    enum COLOR {
        RED, GREEN, BLUE, ALPHA
    };

    struct Pixel {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
    };

    void readCacheInfo();

    int decode_line(unsigned char*& image_ptr, const std::string& line);

    void printPixels(unsigned char* img, int size);

}

#endif