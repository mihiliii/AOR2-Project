#ifndef operations_h
#define operations_h

#include "helper.h"

namespace aor2 {

    void add(unsigned char* image_ptr, unsigned char value, COLOR);
    void sub(unsigned char* image_ptr, unsigned char value, COLOR);
    void mul(unsigned char* image_ptr, float value, COLOR);
    void div(unsigned char* image_ptr, float value, COLOR);
    void sub_inverse(unsigned char* image_ptr, unsigned char value, COLOR);
    void div_inverse(unsigned char* image_ptr, float value, COLOR);

    void power(unsigned char* image_ptr, float value, COLOR);
    void log(unsigned char* image_ptr, COLOR);
    void abs(unsigned char* image_ptr, COLOR);
    void min(unsigned char* image_ptr, unsigned char value, COLOR);
    void max(unsigned char* image_ptr, unsigned char value, COLOR);

    void inverse(unsigned char* image_ptr);
    void grayscale(unsigned char* image_ptr);

    void filter(Pixel* image_ptr, float* matrix, int N, Pixel* new_image_ptr);

}

#endif