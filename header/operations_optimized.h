#ifndef operations_optimized_h
#define operations_optimized_h

#include "../header/operations.h"

namespace aor2 {

    void add_op(unsigned char*, unsigned char, COLOR);
    void sub_op(unsigned char*, unsigned char, COLOR);
    void mul_op(unsigned char*, float, COLOR);
    void div_op(unsigned char*, float, COLOR);
    void sub_inverse_op(unsigned char*, unsigned char, COLOR);
    void div_inverse_op(unsigned char*, float, COLOR);

    void power_op(unsigned char*, float, COLOR);
    void log_op(unsigned char*, COLOR);
    void abs_op(unsigned char*, COLOR);
    void min_op(unsigned char*, unsigned char, COLOR);
    void max_op(unsigned char*, unsigned char, COLOR);

    void inverse_op(unsigned char*);
    void grayscale_op(unsigned char*);

    unsigned char* filter_op(Pixel*, float*, int);

}

#endif