#include <iostream>
#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../header/stb_image.h"
#include "../header/stb_image_write.h"

#include "../header/helper.h"

using namespace std;

int width, height, channels;
bool optimized;

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Error: Insert valid arguments.";
        return -1;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "OP") == 0) {
            optimized = true;
        } else {
            optimized = false;
        }
    }

    aor2::readCacheInfo();

    char* filename = argv[1];
    unsigned char* image_ptr = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (image_ptr == nullptr) {
        std::cout << "Error: Could not open or find the image. \n" << std::endl;
        return -1;
    }

    channels = 4;
    void* aligned_mem_ptr = _mm_malloc(width * height * channels, 32);

    memcpy(aligned_mem_ptr, image_ptr, width * height * channels);
    stbi_image_free(image_ptr);

    image_ptr = (unsigned char*) aligned_mem_ptr;

    ifstream txt_file("operations.txt");
    if (txt_file.is_open()) {
        string line;
        while (getline(txt_file, line)) {
            if (aor2::decode_line(image_ptr, line) < 0) {
                _mm_free(image_ptr);
                return -1;
            }
        }
        txt_file.close();
    } else {
        cout << "Unable to open file.";
        return -1;
    }

    if ((string)argv[2] == ".jpg") {
        if (!stbi_write_jpg("output_image.jpg", width, height, channels, image_ptr, 100)) {
            cout << "Error: Failed to save image. \n";
            _mm_free(image_ptr);
            return -1;
        }
    } else if ((string)argv[2] == ".bmp") {
        for (int i = aor2::COLOR::ALPHA; i < width * height * channels; i += channels) {
            image_ptr[i] = 255;
        }
        if (!stbi_write_bmp("output_image.bmp", width, height, channels, image_ptr)) {
            cout << "Error: Failed to save image. \n";
            _mm_free(image_ptr);
            return -1;
        }
    } else {
        cout << "Error: Wrong output file format";
        _mm_free(image_ptr);
        return -1;
    }

    _mm_free(image_ptr);

    return 0;
}
