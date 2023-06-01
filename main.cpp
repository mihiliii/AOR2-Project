#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./header/stb_image.h"
#include "./header/stb_image_write.h"

using namespace std;

void add_op(unsigned char*, unsigned char, unsigned char, unsigned char);
void sub_op(unsigned char*, char, char, char);
void div_op(unsigned char*, char, char, char);
void mul_op(unsigned char*, char, char, char);

unordered_map<string, function<void(unsigned char*, char, char, char)>> func_hashmap = {
        {"add", add_op},
        {"sub", sub_op},
        {"div", div_op},
        {"mul", mul_op}
};

int width, height, channels;

//void do_operation(unsigned char* image_ptr, const string& operation_name) {
//    func_hashmap[operation_name](image_ptr, 5);
//}

int main(int argc, char** argv) {

    if (argc < 2) {
        cout << "Error";
        return -1;
    }

    unsigned char* image_ptr = stbi_load("test.jpg", &width, &height, &channels, 0);
    if (image_ptr == nullptr) {
        std::cout << "Could not open or find the image." << std::endl;
        return -1;
    }

    if ((string)argv[1] == "-from-text") {
        cout << "\nludilo";
        ifstream text_file("operations.txt");
        string operation_name;
//        while (getline(text_file, operation_name)) {
//        }
        div_op(image_ptr, 2, 2, 2);
        if (!stbi_write_jpg("output_image.jpg", width, height, channels, image_ptr, 100)) {
            cout << "\nFailed to save image";
            stbi_image_free(image_ptr);
            return -1;
        }
        stbi_image_free(image_ptr);
        return 1;
    }
    cout << "nije ludilo";
    return 0;
}

