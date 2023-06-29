#include "../header/helper.h"
#include <fstream>
#include <string>

int aor2::readCacheInfo(int cpu_core) {
    std::string cache_dir = "/sys/devices/system/cpu/cpu" + std::to_string(cpu_core) + "/cache";

    std::ifstream cache_file(cache_dir + "/index0/size");
    std::string cache_size;
    if (cache_file) {
        getline(cache_file, cache_size);
        cache_file.close();
    }

    char unit = cache_size.back();
    cache_size.pop_back();

    switch (unit) {
        case 'K':
            return stoi(cache_size) << 10;
            break;
        case 'M':
            return stoi(cache_size) << 20;
            break;
        default:
            return stoi(cache_size);
    }

}