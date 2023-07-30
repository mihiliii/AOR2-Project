#include <iostream>
#include <immintrin.h>
#include <cmath>

#include "../header/operations_optimized.h"
#include "../header/_Timer.h"
#include "../header/avx_mathfun.h"
#include "../header/helper.h"

extern int width, height, channels;
extern uint32_t cache_size, cache_associativity, cache_entriesPerSet, cache_blockSize;
using namespace std;

void aor2::add_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    StartTimer(ADD SIMD)
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);

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
    StartTimer(SUB SIMD)
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);

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
    StartTimer(MUL SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(DIV SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(SUBI SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(DIVI SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(POW SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_R_mask = _mm256_set1_epi32(0xFF);

    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_pixels = _mm256_cvtepi32_ps(_mm256_srli_epi32(_mm256_and_si256(vec_mem, vec_color_mask), 8 * color));

        __m256 temp = log256_ps(vec_pixels);
        temp = _mm256_mul_ps(temp, vec_value);
        temp = exp256_ps(temp);

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(temp),vec_R_mask);

        vec_result = _mm256_or_si256( _mm256_and_si256(vec_mem, vec_inverse_mask), _mm256_slli_epi32(vec_result, 8 * color));


        _mm256_storeu_si256((__m256i*) (img + i), vec_result);

    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) std::pow(img[i + color], value);
    }
    EndTimer
}

void aor2::log_op(unsigned char* img, aor2::COLOR color) {
    StartTimer(LOG SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(ABS SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

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
    StartTimer(MIN SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32(value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_pixels = _mm256_and_si256(vec_mem, vec_color_mask);

        __m256i vec_result = _mm256_min_epi32(vec_value, vec_pixels);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = std::min(img[i + color], value);
    }
    EndTimer
}

void aor2::max_op(unsigned char* img, unsigned char value, aor2::COLOR color) {
    StartTimer(MAX SIMD)
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    for (; i < array_size - 32; i += 32) {
        __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

        __m256i vec_pixels = _mm256_and_si256(vec_mem, vec_color_mask);

        __m256i vec_result = _mm256_max_epi32(vec_value, vec_pixels);

        vec_result = _mm256_or_si256(_mm256_and_si256(vec_mem, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_result);
    }
    for (; i < array_size; i += channels) {
        img[i + color] = std::max(img[i + color], value);
    }
    EndTimer
}

void aor2::inverse_op(unsigned char* img) {
    StartTimer(INVERSE SIMD)
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_value = _mm256_set1_epi8(255);

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
    StartTimer(GRAYSCALE SIMD)
    int i = 0;
    int array_size = height * width * channels;
    __m256i vec_red_mask = _mm256_set1_epi32(0x000000FF);
    __m256i vec_green_mask = _mm256_set1_epi32(0x0000FF00);
    __m256i vec_blue_mask = _mm256_set1_epi32(0x00FF0000);
    __m256 vec_divisor = _mm256_set1_ps(3);

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

void aor2::filter_op(Pixel* image_ptr, float* matrix, int N, Pixel* new_image_ptr) {
    StartTimer(FILTER SIMD)
    int k = N/2;
    for (int i = k; i < height - k; i++) {
        int j;
        for (j = k; j < width - (k + 8); j += 8) {
            __m256i vec_store = _mm256_set1_epi32(0);
            for (int color = 0; color < 3; color++) {
                __m256i vec_pixel_center = _mm256_set1_epi32(0);
                __m256i mask = _mm256_set1_epi32(0xFF << 8 * color);
                __m256i vec_max_char = _mm256_set1_epi32(0xFF << 8 * color);
                __m256i vec_min_char = _mm256_set1_epi32(0);
                for (int a = 0; a < N; a++) {
                    for (int b = 0; b < N; b++) {
                        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (image_ptr + (i + a - k) * width + j + b - k));
                        vec_pixels = _mm256_and_si256(vec_pixels, mask);

                        __m256 vec_pixels_float = _mm256_cvtepi32_ps(vec_pixels);
                        __m256 vec_matrix_elems = _mm256_set1_ps(matrix[a * N + b]);

                        __m256 vec_result = _mm256_mul_ps(vec_pixels_float, vec_matrix_elems);

                        vec_pixel_center = _mm256_add_epi32(vec_pixel_center, _mm256_cvtps_epi32(vec_result));
                    }
                }
                vec_pixel_center = _mm256_max_epi32(_mm256_min_epi32(vec_max_char, vec_pixel_center), vec_min_char);
                vec_store = _mm256_or_si256(vec_store, vec_pixel_center);
            }
            _mm256_storeu_si256((__m256i*) (new_image_ptr + i * width + j), vec_store);
        }
        for (; j < width - k; j++) {
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
}

//void aor2::filter_op(Pixel* image_ptr, float* matrix, int N, Pixel* new_image_ptr) {
//    StartTimer(FILTER SIMD)
//    int k = N/2;
//    int i;
//    int blockSize = cache_blockSize / sizeof(Pixel);
//    for (i = k; i < height - (k + blockSize); i += blockSize) {
//        int j;
//        for (j = k; j < width - (k + 8 + blockSize); j += blockSize) {
//            for (int ii = i; ii < i + blockSize; ii++) {
//                for (int jj = j; jj < j + blockSize; jj += 8) {
//                    __m256i vec_store = _mm256_set1_epi32(0);
//                    for (int color = 0; color < 3; color++) {
//                        __m256i vec_pixel_center = _mm256_set1_epi32(0);
//                        __m256i mask = _mm256_set1_epi32(0xFF << 8 * color);
//                        __m256i vec_max_char = _mm256_set1_epi32(0xFF << 8 * color);
//                        __m256i vec_min_char = _mm256_set1_epi32(0);
//                        for (int a = 0; a < N; a++) {
//                            for (int b = 0; b < N; b++) {
//                                __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (image_ptr + (ii + a - k) * width + jj + b - k));
//                                vec_pixels = _mm256_and_si256(vec_pixels, mask);
//
//                                __m256 vec_pixels_float = _mm256_cvtepi32_ps(vec_pixels);
//                                __m256 vec_matrix_elems = _mm256_set1_ps(matrix[a * N + b]);
//
//                                __m256 vec_result = _mm256_mul_ps(vec_pixels_float, vec_matrix_elems);
//
//                                vec_pixel_center = _mm256_add_epi32(vec_pixel_center, _mm256_cvtps_epi32(vec_result));
//                            }
//                        }
//                        vec_pixel_center = _mm256_max_epi32(_mm256_min_epi32(vec_max_char, vec_pixel_center), vec_min_char);
//                        vec_store = _mm256_or_si256(vec_store, vec_pixel_center);
//                    }
//                    _mm256_storeu_si256((__m256i*) (new_image_ptr + ii * width + jj), vec_store);
//                }
//            }
//        }
//        for (int ii = i; ii < i + blockSize; ii++) {
//            for (int jj = j; jj < width - k; jj++) {
//                int R = 0, G = 0, B = 0;
//                for (int a = 0; a < N; a++){
//                    for (int b = 0; b < N; b++) {
//                        R = R + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].R * matrix[a * N + b]);
//                        G = G + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].G * matrix[a * N + b]);
//                        B = B + (int) ((float) image_ptr[(ii + a - k) * width + jj + b - k].B * matrix[a * N + b]);
//                    }
//                }
//                R = std::max(0, std::min(255, R));
//                G = std::max(0, std::min(255, G));
//                B = std::max(0, std::min(255, B));
//                new_image_ptr[ii * width + jj] = { (unsigned char) R, (unsigned char) G, (unsigned char) B, 255};
//            }
//        }
//    }
//    for (int ii = i; ii < height - k; ii++) {
//        for (int jj = 0; jj < width - k; jj++) {
//            __m256i vec_store = _mm256_set1_epi32(0);
//            for (int color = 0; color < 3; color++) {
//                __m256i vec_pixel_center = _mm256_set1_epi32(0);
//                __m256i mask = _mm256_set1_epi32(0xFF << 8 * color);
//                __m256i vec_max_char = _mm256_set1_epi32(0xFF << 8 * color);
//                __m256i vec_min_char = _mm256_set1_epi32(0);
//                for (int a = 0; a < N; a++) {
//                    for (int b = 0; b < N; b++) {
//                        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (image_ptr + (ii + a - k) * width + jj + b - k));
//                        vec_pixels = _mm256_and_si256(vec_pixels, mask);
//
//                        __m256 vec_pixels_float = _mm256_cvtepi32_ps(vec_pixels);
//                        __m256 vec_matrix_elems = _mm256_set1_ps(matrix[a * N + b]);
//
//                        __m256 vec_result = _mm256_mul_ps(vec_pixels_float, vec_matrix_elems);
//
//                        vec_pixel_center = _mm256_add_epi32(vec_pixel_center, _mm256_cvtps_epi32(vec_result));
//                    }
//                }
//                vec_pixel_center = _mm256_max_epi32(_mm256_min_epi32(vec_max_char, vec_pixel_center), vec_min_char);
//                vec_store = _mm256_or_si256(vec_store, vec_pixel_center);
//            }
//            _mm256_storeu_si256((__m256i*) (new_image_ptr + ii * width + jj), vec_store);
//        }
//    }
//    EndTimer
//}