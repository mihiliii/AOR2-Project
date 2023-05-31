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

extern void add_op(unsigned char*, int);
extern void sub_op(unsigned char*, int);
extern void mul_op(unsigned char*, int);
extern void div_op(unsigned char*, int);

unordered_map<string, function<void(unsigned char*, int)>> func_hashmap = {
        {"add", add_op},
        {"sub", sub_op},
        {"div", div_op},
        {"mul", mul_op}
};

int width, height, channels;

void do_operation(unsigned char* image_ptr, const string& operation_name) {
    func_hashmap[operation_name](image_ptr, 5);
}

int main(int argc, char** argv) {


    if (argc < 2) {
        cout << "Error";
        return -1;
    }

    unsigned char* image_ptr = stbi_load("test.jpg", &width, &height, &channels, 0);
    if (image_ptr == nullptr) {
        // Image loading failed
        std::cout << "Could not open or find the image." << std::endl;
        return -1;
    }

    if ((string)argv[1] == "-from-text") {
        cout << "ludilo";
        ifstream text_file("operations.txt");
        string operation_name;
        while (getline(text_file, operation_name)) {
            do_operation(image_ptr, operation_name);
        }
        return 1;
    }
    cout << "nije ludilo";
    return 0;
}

