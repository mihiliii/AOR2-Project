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

void aor2::mul(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
    StartTimer(NOSIMD)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel + color] *= value;
        }
    }
    EndTimer
}

void aor2::div(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
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

void aor2::div_inverse(unsigned char* image_ptr, unsigned char value, aor2::COLOR color) {
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
            image_ptr[image_pixel + color] = std::abs((char) image_ptr[image_pixel + color]);
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

//void aor2::filter(unsigned char* image_ptr, float* matrix, int N) {
//    int a = N/2;
//    for (int i = 1; i < height - 1; i ++) {
//        for (int j = 3; j < width * channels - 3; j += 3) {
//            int temp = 0;
//            for (int ii = -a; ii <= a; ii++) {
//                for (int jj = -a; jj <= a; j++) {
//                    temp += image_ptr[(i + ii) * width + j + jj] * matrix[(ii + 1) * N + (jj + 1)];
//                }
//            }
//            if (temp > 255) temp = 255;
//            if (temp < 0) temp = 0;
//            image_ptr[i * width + j] = (unsigned char) temp;
//        }
//    }
//}

void aor2::filter(unsigned char* image_ptr, float* matrix, int N) {
    int a = N/2;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 3; j < width * channels - 3; j++) {
            int temp = 0;
            for (int ii = -a; ii <= a; ii++){
                for (int jj = -a; jj <= a; jj++) {
                    temp = temp + (int) (image_ptr[(i + ii) * width * channels + j + (jj * channels)] * matrix[(ii + 1) * N + (jj + 1)]);
                }
            }
            temp = std::min(255, temp);
            temp = std::max(0, temp);
            image_ptr[i * width * channels + j] = (unsigned char) temp;
        }
    }
}
