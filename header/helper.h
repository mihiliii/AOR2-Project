#ifndef helper_h
#define helper_h

#include <iostream>

#include "../header/stb_image.h"
#include "../header/stb_image_write.h"

namespace aor2 {

    int readCacheInfo(int cpu_core);

    int decode_line(unsigned char*& image_ptr, const std::string& line);

    void printPixels(unsigned char* img, int size, int channels);

}

#endif