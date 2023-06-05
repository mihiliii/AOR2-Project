#include <emmintrin.h>
#include <immintrin.h>

#include <iostream>
#include <cmath>

#include "../header/_Timer.h"
#include "../header/operations.h"

using namespace std;

extern int width, height, channels;
const unsigned char COLOR_MAX_VALUE = 255;

void aor2::add(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] += value;
        }
    }
}

void aor2::sub(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] -= value;
        }
    }
}

void aor2::mul(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] *= value;
        }
    }
}

void aor2::div(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] /= value;
        }
    }
}

void aor2::sub_inverse(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = value - image_ptr[image_pixel + color];
        }
    }
}

void aor2::div_inverse(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = value / image_ptr[image_pixel + color];
        }
    }
}

void aor2::power(unsigned char* image_ptr, float value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) pow(image_ptr[image_pixel + color],  value);
        }
    }
}

void aor2::log(unsigned char* image_ptr, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::log(image_ptr[image_pixel + color]);
        }
    }
}

void aor2::abs(unsigned char* image_ptr, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::abs(image_ptr[image_pixel + color]);
        }
    }
}

void aor2::min(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::min(image_ptr[image_pixel + color], value);
        }
    }
}

void aor2::max(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::max(image_ptr[image_pixel + color], value);
        }
    }
}

void aor2::inverse(unsigned char* image_ptr) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + 0] = COLOR_MAX_VALUE - image_ptr[image_pixel + 0];
            image_ptr[image_pixel + 1] = COLOR_MAX_VALUE - image_ptr[image_pixel + 1];
            image_ptr[image_pixel + 2] = COLOR_MAX_VALUE - image_ptr[image_pixel + 2];
        }
    }
}

void aor2::grayscale(unsigned char* image_ptr) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            int mid_value = (image_ptr[image_pixel + 0] + image_ptr[image_pixel + 1] + image_ptr[image_pixel + 2]) / 3;
            image_ptr[image_pixel + 0] = mid_value;
            image_ptr[image_pixel + 1] = mid_value;
            image_ptr[image_pixel + 2] = mid_value;
        }
    }
}


