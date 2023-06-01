#include <emmintrin.h>
#include <immintrin.h>

#include <iostream>

extern int width, height, channels;

void add_op(unsigned char* image_ptr, unsigned char R_constant, unsigned char G_constant, unsigned char B_constant) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel] += R_constant;
            image_ptr[image_pixel + 1] += G_constant;
            image_ptr[image_pixel + 2] += B_constant;
        }
    }
}

void sub_op(unsigned char* image_ptr, char R_constant, char G_constant, char B_constant) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel] -= R_constant;
            image_ptr[image_pixel + 1] -= G_constant;
            image_ptr[image_pixel + 2] -= B_constant;
        }
    }
}

void mul_op(unsigned char* image_ptr, char R_constant, char G_constant, char B_constant) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel] *= R_constant;
            image_ptr[image_pixel + 1] *= G_constant;
            image_ptr[image_pixel + 2] *= B_constant;
        }
    }
}

void div_op(unsigned char* image_ptr, char R_constant, char G_constant, char B_constant) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int image_pixel = (i * width + j) * channels;
            image_ptr[image_pixel] /= R_constant;
            image_ptr[image_pixel + 1] /= G_constant;
            image_ptr[image_pixel + 2] /= B_constant;
        }
    }
}