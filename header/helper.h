#ifndef helper_h
#define helper_h

#include <iostream>

namespace aor2 {

    int readCacheInfo(int cpu_core);

    int decode_line(unsigned char* image_ptr, const std::string& line);

}

#endif