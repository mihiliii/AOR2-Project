#include <iostream>
#include <fstream>
#include <sstream>
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

int width, height, channels;
int cache_size;

//void printPixels(unsigned char* img, int size, int channels) {
//    if (channels == 3){
//        for (int i = 0; i < size; i += channels) {
//            cout << "[" << (int) img[i] << ", " << (int) img[i + 1] << ", " << (int) img[i + 2] << "]" << endl;
//        }
//    }
//    else {
//        for (int i = 0; i < size; i += channels) {
//            cout << "[" << (int) img[i] << ", " << (int) img[i + 1] << ", " << (int) img[i + 2] << ", " << (int) img[i + 3]
//                 << "]" << endl;
//        }
//    }
//}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Error: Insert arguments.";
        return -1;
    }

    cache_size = aor2::readCacheInfo(0);

    unsigned char* image_ptr = stbi_load(argv[1], &width, &height, &channels, 0);
    if (image_ptr == nullptr) {
        std::cout << "Error: Could not open or find the image. \n" << std::endl;
        return -1;
    }

//    channels = 4;

    if ((string)argv[2] == "-load-text") {
        cout << "Reading from txt file: \n";
        ifstream txt_file("operations.txt");
        if (txt_file.is_open()) {
            string line;
            while (getline(txt_file, line)) {
                istringstream ss(line);
                string instruction_str, value_str, color_str;
                ss >> instruction_str;
                if (func_1.find(instruction_str) != func_1.end()) {
                    func_1[instruction_str](image_ptr);
                    continue;
                }
                ss >> color_str;
                aor2::COLOR color;
                if (color_str == "R") {
                    color = aor2::COLOR::RED;
                } else if (color_str == "G") {
                    color = aor2::COLOR::GREEN;
                } else if (color_str == "B") {
                    color = aor2::COLOR::BLUE;
                } else {
                    cout << "Error: Invalid color.";
                }
                if (func_2.find(instruction_str) != func_2.end()) {
                    func_2[instruction_str](image_ptr, color);
                    continue;
                }
                ss >> value_str;
                if (func_3_char.find(instruction_str) != func_3_char.end()) {
                    func_3_char[instruction_str](image_ptr, stoi(value_str), color);
                    continue;
                } else if (func_3_float.find(instruction_str) != func_3_float.end()) {
                    func_3_float[instruction_str](image_ptr, stof(value_str), color);
                    continue;
                }
                cout << "Error: Invalid instruction";
            }
            txt_file.close();
        }
        else {
            cout << "Unable to open file.";
        }
        float matrix[3][3] = { 1/9., 1/9. , 1/9., 1/9., 1/9., 1/9., 1/9., 1/9., 1/9.};
        float matrix1[3][3] = { 0, -1, 0, -1, 4, -1, 0, -1, 0};
        float matrix2[3][3] = { 0, 0, 0, 0, 1, 0, 0, 0, 0};
        float matrix3[3][3] = { -1, 0, 1, -1, 0, 1, -1, 0 , 1};
        float matrix4[3][3] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
//        aor2::filter(image_ptr, &matrix1[0][0], 3);

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

