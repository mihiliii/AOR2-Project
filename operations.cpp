#include <emmintrin.h>
#include <immintrin.h>

extern int width, height, channels;

void add_op(unsigned char* image_ptr, int constant) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel] += constant;
        }
    }
}

void sub_op(unsigned char* image, int constant);
void mul_op(unsigned char* image, int constant);
void div_op(unsigned char* image, int constant);