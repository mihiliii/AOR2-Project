#include <emmintrin.h>
#include <immintrin.h>

#include <iostream>
#include <cmath>

#include "../header/_Timer.h"
#include "../header/operations.h"
#include "../header/helper.h"

using namespace std;

extern int width, height, channels;
const unsigned char COLOR_MAX_VALUE = 255;

void aor2::add(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] += value;
        }
    }
    EndTimer
}

void aor2::sub(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] -= value;
        }
    }
}

void aor2::mul(unsigned char* image_ptr, float value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] *= value;
        }
    }
    EndTimer
}

void aor2::div(unsigned char* image_ptr, float value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] /= value;
        }
    }
    EndTimer
}

void aor2::sub_inverse(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = value - image_ptr[image_pixel + color];
        }
    }
    EndTimer
}

void aor2::div_inverse(unsigned char* image_ptr, float value, aor2::COLOR color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = value / image_ptr[image_pixel + color];
        }
    }
}

void aor2::power(unsigned char* image_ptr, float value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) pow(image_ptr[image_pixel + color],  value);
        }
    }
    EndTimer
}

void aor2::log(unsigned char* image_ptr, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::log(image_ptr[image_pixel + color]);
        }
    }
    EndTimer
}

void aor2::abs(unsigned char* image_ptr, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] = (unsigned char) std::abs((char) image_ptr[image_pixel + color]);
        }
    }
    EndTimer
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
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + 0] = COLOR_MAX_VALUE - image_ptr[image_pixel + 0];
            image_ptr[image_pixel + 1] = COLOR_MAX_VALUE - image_ptr[image_pixel + 1];
            image_ptr[image_pixel + 2] = COLOR_MAX_VALUE - image_ptr[image_pixel + 2];
        }
    }
    EndTimer
}

void aor2::grayscale(unsigned char* image_ptr) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            int mid_value = (image_ptr[image_pixel + 0] + image_ptr[image_pixel + 1] + image_ptr[image_pixel + 2]) / 3;
            image_ptr[image_pixel + 0] = mid_value;
            image_ptr[image_pixel + 1] = mid_value;
            image_ptr[image_pixel + 2] = mid_value;
        }
    }
    EndTimer
}

void aor2::filter(Pixel* image_ptr, float* matrix, int N, Pixel* new_image_ptr) {
    StartTimer(NOSIMD)
    int k = N/2;
    for (int i = k; i < height - k; i++) {
        for (int j = k; j < width - k; j++) {
            int R = 0, G = 0, B = 0;
            for (int a = 0; a < N; a++){
                for (int b = 0; b < N; b++) {
                    R = R + (int) ((float) image_ptr[(i + a - k) * width + j + b - k].R * matrix[a * N + b]);
                    G = G + (int) ((float) image_ptr[(i + a - k) * width + j + b - k].G * matrix[a * N + b]);
                    B = B + (int) ((float) image_ptr[(i + a - k) * width + j + b - k].B * matrix[a * N + b]);
                }
            }
            R = std::max(0, std::min(255, R));
            G = std::max(0, std::min(255, G));
            B = std::max(0, std::min(255, B));
            new_image_ptr[i * width + j] = { (unsigned char) R, (unsigned char) G, (unsigned char) B, 255};
        }
    }
    EndTimer
    stbi_image_free((unsigned char*) image_ptr);
    return (unsigned char*) new_image_ptr;
}
