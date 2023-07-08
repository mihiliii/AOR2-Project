#ifndef operations_optimized_h
#define operations_optimized_h

#include "../header/operations.h"

namespace aor2 {

    void add_op(unsigned char* image_ptr, unsigned char value, COLOR);
    void sub_op(unsigned char* image_ptr, unsigned char value, COLOR);
    void mul_op(unsigned char* image_ptr, float value, COLOR);
    void div_op(unsigned char* image_ptr, float value, COLOR);
    void sub_inverse_op(unsigned char* image_ptr, unsigned char value, COLOR);
    void div_inverse_op(unsigned char* image_ptr, float value, COLOR);

    void power_op(unsigned char* image_ptr, float value, COLOR);
    void log_op(unsigned char* image_ptr, COLOR);
    void abs_op(unsigned char* image_ptr, COLOR);
    void min_op(unsigned char* image_ptr, unsigned char value, COLOR);
    void max_op(unsigned char* image_ptr, unsigned char value, COLOR);

    void inverse_op(unsigned char* image_ptr);
    void grayscale_op(unsigned char* image_ptr);

    void filter_op(Pixel* image_ptr, float* matrix, int N, Pixel* new_image_ptr);

}

#endif