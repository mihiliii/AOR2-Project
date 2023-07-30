#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <unordered_map>
#include <immintrin.h>
#include <iostream>

#include "../header/operations_optimized.h"
#include "../header/operations.h"
#include "../header/helper.h"

using namespace std;

extern int width, height, channels;
extern bool optimized;

uint32_t cache_size, cache_associativity, cache_entriesPerSet, cache_blockSize;

unordered_map<string, function<void(unsigned char*)>> func_1 = {
        {"INVERSE", aor2::inverse},
        {"GRAYSCALE", aor2::grayscale}
};

unordered_map<string, function<void(unsigned char*, aor2::COLOR)>> func_2 = {
        {"LOG", aor2::log},
        {"ABS", aor2::abs},
};

unordered_map<string, function<void(unsigned char*, unsigned char, aor2::COLOR)>> func_3_char = {
        {"ADD", aor2::add},
        {"SUB", aor2::sub},
        {"SUBI", aor2::sub_inverse},
        {"DIVI", aor2::div_inverse},
        {"MIN", aor2::min},
        {"MAX", aor2::max},
};

unordered_map<string, function<void(unsigned char*, float, aor2::COLOR)>> func_3_float = {
        {"MUL", aor2::mul},
        {"DIV", aor2::div},
        {"POW", aor2::power},
};

unordered_map<string, function<void(unsigned char*)>> func_1_op = {
        {"INVERSE", aor2::inverse_op},
        {"GRAYSCALE", aor2::grayscale_op}
};

unordered_map<string, function<void(unsigned char*, aor2::COLOR)>> func_2_op = {
        {"LOG", aor2::log_op},
        {"ABS", aor2::abs_op},
};

unordered_map<string, function<void(unsigned char*, unsigned char, aor2::COLOR)>> func_3_char_op = {
        {"ADD", aor2::add_op},
        {"SUB", aor2::sub_op},
        {"SUBI", aor2::sub_inverse_op},
        {"DIVI", aor2::div_inverse_op},
        {"MIN", aor2::min_op},
        {"MAX", aor2::max_op},
};

unordered_map<string, function<void(unsigned char*, float, aor2::COLOR)>> func_3_float_op = {
        {"MUL", aor2::mul_op},
        {"DIV", aor2::div_op},
        {"POW", aor2::power_op},
};

int aor2::decode_line(unsigned char*& image_ptr, const std::string& line) {
    istringstream ss(line);
    string instruction_str, value_str, color_str;
    ss >> instruction_str;

    if (instruction_str == "FILTER") {
        string matrix_size, element;
        ss >> matrix_size;
        int N = stoi(matrix_size);
        auto matrix = new float[N * N];
        for (int i = 0; i < N * N; i++) {
            ss >> element;
            matrix[i] = stof(element);
        }
        auto new_image_ptr = (unsigned char*) _mm_malloc(width * height * channels, 32);

        if (optimized) {
            aor2::filter_op((Pixel*) image_ptr, &matrix[0], N, (Pixel*) new_image_ptr);
        } else {
            aor2::filter((Pixel*) image_ptr, &matrix[0], N, (Pixel*) new_image_ptr);
        }

        _mm_free(image_ptr);
        image_ptr = new_image_ptr;
        delete[] matrix;

        return 1;
    }

    if (optimized) {
        if (func_1_op.find(instruction_str) != func_1_op.end()) {
            func_1_op[instruction_str](image_ptr);
            return 1;
        }
    } else {
        if (func_1.find(instruction_str) != func_1.end()) {
            func_1[instruction_str](image_ptr);
            return 1;
        }
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
        return -1;
    }

    if (func_2_op.find(instruction_str) != func_2_op.end() && optimized) {
        func_2_op[instruction_str](image_ptr, color);
        return 1;
    }
    if (func_2.find(instruction_str) != func_2.end() && !optimized) {
        func_2[instruction_str](image_ptr, color);
        return 1;
    }

    ss >> value_str;

    if (func_3_char_op.find(instruction_str) != func_3_char_op.end() && optimized) {
        func_3_char_op[instruction_str](image_ptr, stoi(value_str), color);
        return 1;
    }
    if (func_3_char.find(instruction_str) != func_3_char.end() && !optimized) {
        func_3_char[instruction_str](image_ptr, stoi(value_str), color);
        return 1;
    }
    if (func_3_float_op.find(instruction_str) != func_3_float_op.end() && optimized) {
        func_3_float_op[instruction_str](image_ptr, stof(value_str), color);
        return 1;
    }
    if (func_3_float.find(instruction_str) != func_3_float.end() && !optimized) {
        func_3_float[instruction_str](image_ptr, stof(value_str), color);
        return 1;
    }

    cout << "Error: Invalid instruction";
    return -1;
}

void aor2::printPixels(unsigned char* img, int size) {
   if (channels == 3){
       for (int i = 0; i < size; i += channels) {
           cout << "[" << (int) img[i]
                << ", " << (int) img[i + 1] 
                << ", " << (int) img[i + 2]
                << "]" << endl;
       }
   }
   else {
       for (int i = 0; i < size * 4; i += channels) {
           cout << i / 4 << "[" << (int) img[i]
                << ", " << (int) img[i + 1] 
                << ", " << (int) img[i + 2]
                << ", " << (int) img[i + 3] 
                << "]" << endl;
       }
   }
}

void aor2::readCacheInfo() {
    uint32_t eax, ebx, ecx, edx;
    {
        eax = 0x80000005;

        __asm__ (
                "cpuid"
                : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
                );

        cache_size = (ecx >> 24) & 0xFF,
        cache_associativity = (ecx >> 16) & 0xFF,
        cache_blockSize = ecx & 0xFF,
        cache_entriesPerSet = (cache_size * 1024 / cache_associativity) / cache_blockSize;

//        printf(
//                "L1 Data Cache:\n"
//                "\tSize: %d KB\n"
//                "\tAssociativity: %d\n"
//                "\tLines per Tag: %d\n"
//                "\tLine Size: %d B\n"
//                "\n",
//                cache_size,
//                cache_associativity,
//                cache_entriesPerSet,
//                cache_blockSize
//        );
    }
}
