#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>
#include <cpuid.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../header/stb_image.h"
#include "../header/stb_image_write.h"

#include "../header/operations.h"

using namespace std;

unordered_map<string, function<void(unsigned char*, unsigned char, aor2::COLOR)>> func_hashmap = {
        {"add", aor2::add}, {"sub", aor2::sub}, {"mul", aor2::mul}, {"div", aor2::div}
};

int width, height, channels;

void getCacheDetails() {
    int registers[4];
    int cacheType, cacheLevel, cacheWays, cacheSets, cacheLineSize;

    // Execute CPUID instruction with input 4 to get cache details
    __cpuid(4, registers[0], registers[1], registers[2], registers[3]);

    // Extract cache details from the registers
    cacheType = (registers[0] & 0x1F);
    cacheLevel = ((registers[0] >> 5) & 0x7);
    cacheWays = ((registers[1] >> 22) & 0x3FF) + 1;
    cacheSets = ((registers[1] >> 12) & 0x3FF) + 1;
    cacheLineSize = (registers[1] & 0xFFF) + 1;

    // Print the cache details
    std::cout << "Cache Type: " << cacheType << std::endl;
    std::cout << "Cache Level: " << cacheLevel << std::endl;
    std::cout << "Cache Ways: " << cacheWays << std::endl;
    std::cout << "Cache Sets: " << cacheSets << std::endl;
    std::cout << "Cache Line Size: " << cacheLineSize << " bytes" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Error";
        return -1;
    }

    getCacheDetails();

    unsigned char* image_ptr = stbi_load("./images/test1.jpg", &width, &height, &channels, 0);
    if (image_ptr == nullptr) {
        std::cout << "Could not open or find the image. \n" << std::endl;
        return -1;
    }

    if ((string)argv[1] == "-load-txt") {
//        cout << "Reading from txt file: \n";
//        ifstream txt_file("operations.txt");
//        string operation_name;
//        while (getline(txt_file, operation_name)) {
//        }

        if (!stbi_write_jpg("output_image.jpg", width, height, channels, image_ptr, 100)) {
            cout << "Failed to save image. \n";
            stbi_image_free(image_ptr);
            return -1;
        }

        stbi_image_free(image_ptr);
        return 1;
    }

    cout << "nije ludilo";
    return 0;
}

