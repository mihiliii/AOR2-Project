#include "../header/operations_optimized.h"
#include "../header/_Timer.h"
#include "../header/avx_mathfun.h"
#include <iostream>
#include <immintrin.h>
#include <cmath>
#include "../header/helper.h"

extern int width, height, channels;
using namespace std;

void aor2::add_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_dest = _mm256_add_epi8(vec_pixels, vec_value);

        _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = img[i + color] + value;
    }
    EndTimer
}

void aor2::sub_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_result = _mm256_sub_epi8(vec_pixels, vec_value);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = (img[i + color] - value);
    }
    EndTimer
}

void aor2::mul_op(unsigned char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_pixels = _mm256_cvtepi32_ps(_mm256_and_si256(vec_mem, vec_color_mask));

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                _mm256_mul_ps(vec_pixels, vec_value)),vec_color_mask
                );

        vec_result = _mm256_or_si256(
                _mm256_and_si256(vec_mem, vec_inverse_mask), vec_result
                );

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < height * width * channels; i += channels) {
        img[i + color] = (unsigned char) ((float) img[i + color] * value);
    }
    EndTimer
}

void aor2::div_op(unsigned char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_pixels = _mm256_cvtepi32_ps(_mm256_and_si256(vec_mem, vec_color_mask));

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                _mm256_div_ps(vec_pixels, vec_value)),vec_color_mask
        );

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i] = (unsigned char) ((float) img[i] / value);
    }
    EndTimer
}

void aor2::sub_inverse_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_pixels = _mm256_and_si256(vec_mem, vec_color_mask);

        __m256i vec_result = _mm256_sub_epi8(vec_value, vec_pixels);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) (value - img[i + color]);
    }
    EndTimer
}

void aor2::div_inverse_op(unsigned char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

            __m256 vec_pixels = _mm256_cvtepi32_ps(_mm256_and_si256(vec_mem, vec_color_mask));

            __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                    _mm256_div_ps(vec_value, vec_pixels)),vec_color_mask
            );

            vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

            _mm256_storeu_si256((__m256i*) (img + i), vec_result);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (unsigned char) ((float) value / img[i + color]);
        }
    EndTimer
}

void aor2::power_op(unsigned char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);
    __m256 vec_value = _mm256_set1_ps(value);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_pixels = _mm256_cvtepi32_ps(_mm256_and_si256(vec_mem, vec_color_mask));

        __m256 temp = log256_ps(vec_pixels);
        temp = _mm256_mul_ps(temp, vec_value);
        temp = exp256_ps(temp);

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(temp),vec_color_mask);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);

    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) std::pow(img[i + color], value);
    }
    EndTimer
}

void aor2::log_op(unsigned char* img, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_color_data = _mm256_cvtepi32_ps(_mm256_and_si256(vec_pixels, vec_color_mask));

        __m256 temp = log256_ps(vec_color_data);

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(temp),vec_color_mask);

        __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_dest);

    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) std::log(img[i + color]);
    }
    EndTimer
}

void aor2::abs_op(unsigned char* img, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_color_data = _mm256_and_si256(vec_pixels, vec_color_mask);

        __m256i vec_result = _mm256_abs_epi8(vec_color_data);

        __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) std::abs((char) img[i + color]);
    }
    EndTimer
}

void aor2::min_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_pixels = _mm256_and_si256(vec_mem, vec_color_mask);

        __m256i vec_result = _mm256_min_epi8(vec_value, vec_pixels);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = std::min(img[i + color], value);
    }
    EndTimer
}

void aor2::max_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_pixels = _mm256_and_si256(vec_mem, vec_color_mask);

        __m256i vec_result = _mm256_max_epi8(vec_value, vec_pixels);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = std::max(img[i + color], value);
    }
    EndTimer
}

void aor2::inverse_op(unsigned char* img) {
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi8(255);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*)(img + i));

        __m256i vec_result = _mm256_sub_epi8(vec_value, vec_pixels);

        _mm256_storeu_si256((__m256i*)(img + i), vec_result);
    }
    for (; i < height * width * channels; i++) {
        img[i] = (255 - img[i]);
    }
    EndTimer
}

void aor2::grayscale_op(unsigned char* img) {
    int i = 0;
    int array_size = height * width * channels;
    int R_mask = 0x000000FF;
    int G_mask = 0x0000FF00;
    int B_mask = 0x00FF0000;
    __m256i vec_red_mask = _mm256_set1_epi32(R_mask);
    __m256i vec_green_mask = _mm256_set1_epi32(G_mask);
    __m256i vec_blue_mask = _mm256_set1_epi32(B_mask);
    __m256 vec_divisor = _mm256_set1_ps(3);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_red = _mm256_and_si256(vec_mem, vec_red_mask);
        __m256i vec_green = _mm256_srli_si256(_mm256_and_si256(vec_mem, vec_green_mask), 1);
        __m256i vec_blue = _mm256_srli_si256(_mm256_and_si256(vec_mem, vec_blue_mask), 2);

        __m256i vec_pixels = _mm256_add_epi32(vec_red, vec_green);
        vec_pixels = _mm256_add_epi32(vec_pixels, vec_blue);

        __m256i vec_result = _mm256_cvtps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(vec_pixels), vec_divisor));

        __m256i vec_dest = _mm256_or_si256(_mm256_slli_si256(vec_result, 1), vec_result);
        vec_dest = _mm256_or_si256(_mm256_slli_si256(vec_result, 2), vec_dest);

        _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
    }
    for (; i < array_size; i += channels) {
        int avg_value = (img[i + 0] + img[i + 1] + img[i + 2]) / 3;
        img[i + 0] = avg_value;
        img[i + 1] = avg_value;
        img[i + 2] = avg_value;
    }
    EndTimer
}

unsigned char* aor2::filter_op(Pixel* image_ptr, float* matrix, int N) {
    auto copy = new aor2::Pixel[width * height];
    StartTimer(SIMD)
    int k = N/2;
    for (int i = k; i < height - (k + 64); i += 64) {
        for (int j = k; j < width - (k + 16); j += 16) {
            for (int ii = i; ii < i + 64; ii++) {
                for (int jj = j; jj < j + 16; jj++) {
                    int R = 0, G = 0, B = 0;
                    for (int a = 0; a < N; a++){
                        for (int b = 0; b < N; b++) {
                            R = R + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].R * matrix[a * N + b]);
                            G = G + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].G * matrix[a * N + b]);
                            B = B + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].B * matrix[a * N + b]);
                        }
                    }
                    R = std::max(0, std::min(255, R));
                    G = std::max(0, std::min(255, G));
                    B = std::max(0, std::min(255, B));
                    copy[ii * width + jj] = { (unsigned char) R, (unsigned char) G, (unsigned char) B, 255};
                }
            }
        }
    }
    EndTimer
    stbi_image_free((unsigned char*) image_ptr);
    return (unsigned char*) copy;
}