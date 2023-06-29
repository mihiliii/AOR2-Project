#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../header/stb_image.h"
#include "../header/stb_image_write.h"

#include "../header/operations.h"
#include "../header/operations_optimized.h"
#include "../header/helper.h"

using namespace std;

unordered_map<string, function<void(unsigned char*, unsigned char, aor2::COLOR)>> func_hashmap = {
        {"add", aor2::add}, {"sub", aor2::sub}, {"mul", aor2::mul}, {"div", aor2::div}
};

int width, height, channels;
int cache_size;

void printPixels(unsigned char* img, int size) {
    for (int i = 0; i < size; i += 4) {
        cout << "[" << (int) img[i] << ", " << (int) img[i + 1] << ", " << (int) img[i + 2] << ", " << (int) img[i + 3]
        << "]" << endl;
    }
    cout << endl << endl << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Error";
        return -1;
    }

    cache_size = aor2::readCacheInfo(0);

    unsigned char* image_ptr = stbi_load("./images/test.jpg", &width, &height, &channels, STBI_rgb_alpha);
    if (image_ptr == nullptr) {
        std::cout << "Could not open or find the image. \n" << std::endl;
        return -1;
    }

    channels = 4;

    if ((string)argv[1] == "-load-txt") {
//        cout << "Reading from txt file: \n";
//        ifstream txt_file("operations.txt");
//        string operation_name;
//        while (getline(txt_file, operation_name)) {
//        }

//        aor2::add_op((char*) image_ptr, 30, aor2::COLOR::RED);
//        aor2::add_op(image_ptr, 10, aor2::COLOR::RED);
//        aor2::add_op(image_ptr, 30, aor2::COLOR::BLUE);
//        aor2::div_inverse_op((char*) image_ptr, 200, aor2::COLOR::RED);
        //aor2::power_op(image_ptr, 2, aor2::COLOR::RED);
//        aor2::log_op(image_ptr, aor2::COLOR::RED);
//        aor2::abs(image_ptr, aor2::COLOR::RED);
        aor2::sub_inverse(image_ptr, 20, aor2::COLOR::RED);
        aor2::sub_inverse_op((char*) image_ptr, 20, aor2::COLOR::RED);

        if (!stbi_write_jpg("output_image.jpg", width, height, channels, image_ptr, 100)) {
            cout << "Failed to save image. \n";
            stbi_image_free(image_ptr);
            return -1;
        }

    }

    stbi_image_free(image_ptr);

    return 0;
}

