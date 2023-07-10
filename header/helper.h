#ifndef helper_h
#define helper_h

#include <iostream>

namespace aor2 {

    void readCacheInfo();

    int decode_line(unsigned char*& image_ptr, const std::string& line);

    void printPixels(unsigned char* img, int size);

}

#endif