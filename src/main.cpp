#include <iostream>
#include <fstream>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../header/operations.h"
#include "../header/operations_optimized.h"
#include "../header/helper.h"


using namespace std;

int width, height, channels;
char* filename;
int cache_size;

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Error: Insert arguments.";
        return -1;
    }

    cache_size = aor2::readCacheInfo(0);

    filename = argv[1];
    unsigned char* image_ptr = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (image_ptr == nullptr) {
        std::cout << "Error: Could not open or find the image. \n" << std::endl;
        return -1;
    }

    channels = 4;

    if ((string)argv[2] == "-load-text") {
        cout << "Reading from txt file: \n";
        ifstream txt_file("operations.txt");
        if (txt_file.is_open()) {
            string line;
            while (getline(txt_file, line)) {
                 aor2::decode_line(image_ptr, line);
            }
            txt_file.close();
        }
        else {
            cout << "Unable to open file.";
        }

        if ((string)argv[3] == ".jpg") {
            if (!stbi_write_jpg("output_image.jpg", width, height, channels, image_ptr, 100)) {
                cout << "Error: Failed to save image. \n";
                stbi_image_free(image_ptr);
                return -1;
            }
        }
        else if ((string)argv[3] == ".bmp") {
            if (!stbi_write_bmp("output_image.jpg", width, height, channels, image_ptr)) {
                cout << "Error: Failed to save image. \n";
                stbi_image_free(image_ptr);
                return -1;
            }
        }
        else {
            cout << "Error: Wrong output file format";
            stbi_image_free(image_ptr);
            return -1;
        }
    }

    stbi_image_free(image_ptr);

    return 0;
}
