#include "../header/helper.h"
#include <fstream>
#include <sstream>
#include <string>
#include <functional>
#include <unordered_map>
#include "../header/operations_optimized.h"

using namespace std;

unordered_map<string, function<void(unsigned char*)>> func_1 = {
        {"INVERSE", aor2::inverse_op},
        {"GRAYSCALE", aor2::grayscale_op},
//        {"FILTER", aor2::filter_op}
};

unordered_map<string, function<void(unsigned char*, aor2::COLOR)>> func_2 = {
        {"LOG", aor2::log_op},
        {"ABS", aor2::abs_op},
};

unordered_map<string, function<void(unsigned char*, unsigned char, aor2::COLOR)>> func_3_char = {
        {"ADD", aor2::add_op},
        {"SUB", aor2::sub_op},
        {"SUBI", aor2::sub_inverse_op},
        {"DIVI", aor2::div_inverse_op},
        {"MIN", aor2::min_op},
        {"MAX", aor2::max_op},
};

unordered_map<string, function<void(unsigned char*, float, aor2::COLOR)>> func_3_float = {
        {"MUL", aor2::mul_op},
        {"DIV", aor2::div_op},
        {"POW", aor2::power_op},
};

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

int aor2::decode_line(const std::string& line) {
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