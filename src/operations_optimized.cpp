#include "../header/operations_optimized.h"
#include "../header/_Timer.h"
#include "../header/avx_mathfun.h"
#include <iostream>
#include <immintrin.h>
#include <cmath>

extern int width, height, channels;
using namespace std;

union m256i {
    __m256i vector;
    char element[32];
};

union m256 {
    __m256 vector;
    float element[8];
};

inline __m256i vec_load_color(__m256i vector, unsigned int mask) {
    __m256i vec_mask = _mm256_set1_epi32((int) mask);

    return _mm256_and_si256(vector, vec_mask);
}

inline void vec_store_colors(char* img, int index, __m256i starting_vector, __m256i result, unsigned int mask) {
    __m256i vec_mask = _mm256_set1_epi32((int) mask);

    __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(starting_vector, vec_mask), result);

    _mm256_storeu_si256((__m256i*) (img + index), vec_dest);
}

void aor2::add_op(unsigned char* img, char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int multiplier = 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << multiplier);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256i vec_dest = _mm256_add_epi8(vec_pixels, vec_value);

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (char) (img[i + color] + value);
        }
    EndTimer
}

void aor2::sub_op(char* img, char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int multiplier = 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << multiplier);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256i vec_dest = _mm256_sub_epi8(vec_pixels, vec_value);

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (char) (img[i + color] - value);
        }
    EndTimer
}

void aor2::mul_op(char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256 vec_color_data = _mm256_cvtepi32_ps(_mm256_and_si256(vec_pixels, vec_color_mask));

            __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                    _mm256_mul_ps(vec_color_data, vec_value)),vec_color_mask
                    );

            __m256i vec_dest = _mm256_or_si256(
                    _mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result
                    );

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < height * width * channels; i += channels) {
            img[i + color] = (char) ((float) img[i + color] * value);
        }
    EndTimer
}

void aor2::div_op(char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256 vec_color_data = _mm256_cvtepi32_ps(_mm256_and_si256(vec_pixels, vec_color_mask));

            __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                    _mm256_div_ps(vec_color_data, vec_value)),vec_color_mask
            );

            __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < array_size; i += channels) {
            img[i] = (char) ((float) img[i] / value);
        }
    EndTimer
}

void aor2::sub_inverse_op(char* img, char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    unsigned int mask = 0x000000FF << 8 * color;
    __m256i vec_value = _mm256_set1_epi32((int) value << 8 * color);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_mem = _mm256_loadu_si256((__m256i*) (img + i));

            __m256i vec_color = vec_load_color(vec_mem, mask);

            __m256i vec_result = _mm256_sub_epi8(vec_value, vec_color);

            vec_store_colors(img, i, vec_mem, vec_result, mask ^ 0xFFFFFFFF);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (char) (img[i + color] - value);
        }
    EndTimer
}

void aor2::div_inverse_op(char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int mask = 0x000000FF << 8 * color;
    __m256 vec_value = _mm256_set1_ps(value);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256 vec_color_data = _mm256_cvtepi32_ps(_mm256_and_si256(vec_pixels, vec_color_mask));

            __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(
                    _mm256_div_ps(vec_value, vec_color_data)),vec_color_mask
            );

            __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (char) ((float) value / img[i + color]);
        }
    EndTimer
}

void aor2::power_op(unsigned char* img, float value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels, stride = channels;
    int mask = 0x000000FF << 8 * color;
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);
    __m256 vec_value = _mm256_set1_ps(value);

    StartTimer(SIMD)
    for (; i < array_size - 32; i += 32) {
        __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

        __m256 vec_color_data = _mm256_cvtepi32_ps(_mm256_and_si256(vec_pixels, vec_color_mask));

        __m256 temp = log256_ps(vec_color_data);
        temp = _mm256_mul_ps(temp, vec_value);
        temp = exp256_ps(temp);

        __m256i vec_result = _mm256_and_si256(_mm256_cvtps_epi32(temp),vec_color_mask);

        __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

        _mm256_storeu_si256((__m256i*) (img + i), vec_dest);

    }
    for (; i < array_size; i += channels) {
        img[i + color] = (unsigned char) std::pow(img[i + color], value);
    }
    EndTimer
}

void aor2::log_op(unsigned char* img, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels, stride = channels;
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

void aor2::abs_op(char* img, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int multiplier = 8 * color;
    int mask = 0x000000FF << multiplier;
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
            img[i + color] = (char) std::abs(img[i + color]);
        }
    EndTimer
}

void aor2::min_op(char* img, char value, aor2::COLOR color) {
    int i = 0;
    int array_size = height * width * channels;
    int multiplier = 8 * color;
    int mask = 0x000000FF << multiplier;
    __m256i vec_value = _mm256_set1_epi32((int) value << multiplier);
    __m256i vec_color_mask = _mm256_set1_epi32(mask);
    __m256i vec_inverse_mask = _mm256_set1_epi32(mask ^ 0xFFFFFFFF);

    StartTimer(SIMD)
        for (; i < array_size - 32; i += 32) {
            __m256i vec_pixels = _mm256_loadu_si256((__m256i*) (img + i));

            __m256i vec_color_data = _mm256_and_si256(vec_pixels, vec_color_mask);

            __m256i vec_result = _mm256_sub_epi8(vec_value, vec_color_data);

            __m256i vec_dest = _mm256_or_si256(_mm256_and_si256(vec_pixels, vec_inverse_mask), vec_result);

            _mm256_storeu_si256((__m256i*) (img + i), vec_dest);
        }
        for (; i < array_size; i += channels) {
            img[i + color] = (char) (img[i + color] - value);
        }
    EndTimer
}

void aor2::max_op(char* img, char value, aor2::COLOR color) {
    int i;
    int array_size = height * width * channels, stride = channels;
    __m256i vec_value = _mm256_set1_epi8(value);

    StartTimer(SIMD)
    for (i = color; i < array_size - 32 * stride; i += 32 * stride) {
        __m256i vec_pixels = _mm256_set_epi8(
                img[i + 31 * stride], img[i + 30 * stride], img[i + 29 * stride],
                img[i + 28 * stride],img[i + 27 * stride], img[i + 26 * stride],
                img[i + 25 * stride],img[i + 24 * stride], img[i + 23 * stride],
                img[i + 22 * stride],img[i + 21 * stride], img[i + 20 * stride],
                img[i + 19 * stride],img[i + 18 * stride], img[i + 17 * stride],
                img[i + 16 * stride],img[i + 15 * stride], img[i + 14 * stride],
                img[i + 13 * stride],img[i + 12 * stride], img[i + 11 * stride],
                img[i + 10 * stride],img[i + 9 * stride], img[i + 8 * stride],
                img[i + 7 * stride],img[i + 6 * stride], img[i + 5 * stride],
                img[i + 4 * stride],img[i + 3 * stride], img[i + 2 * stride],
                img[i + 1 * stride],img[i + 0 * stride]
        );

        m256i vec_dest {_mm256_max_epi8(vec_pixels, vec_value)};
        for (int j = 0; j < 32; j++) {
            img[i + j * channels] = vec_dest.element[j];
        }
    }
    for (; i < height * width * channels; i += channels) {
        img[i] = std::max(img[i], value);
    }
    EndTimer
}

void aor2::inverse_op(unsigned char* img) {
    int i;
    int array_size = height * width * channels;
    m256i vec_value {_mm256_set1_epi8(255)};

    StartTimer(SIMD)
    for (i = 0; i < array_size - 32; i += 32) {
        m256i vec_pixels {_mm256_loadu_si256((__m256i*)(img + i))};
        m256i vec_dest {_mm256_sub_epi8(vec_value.vector, vec_pixels.vector)};
        _mm256_storeu_si256((__m256i*)(img + i), vec_dest.vector);
    }
    for (; i < height * width * channels; i++) {
        img[i] = (unsigned char) (255 - img[i]);
    }
    EndTimer
}

void aor2::grayscale_op(unsigned char *) {

}
