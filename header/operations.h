#ifndef operations_h
#define operations_h

namespace aor2 {

    enum COLOR {
        RED, GREEN, BLUE
    };

    struct Pixel {
        unsigned char R;
        unsigned char G;
        unsigned char B;
        unsigned char A;
    };

    void add(unsigned char*, unsigned char, COLOR);
    void sub(unsigned char*, unsigned char, COLOR);
    void mul(unsigned char*, unsigned char, COLOR);
    void div(unsigned char*, unsigned char, COLOR);
    void sub_inverse(unsigned char*, unsigned char, COLOR);
    void div_inverse(unsigned char*, unsigned char, COLOR);

    void power(unsigned char*, float, COLOR);
    void log(unsigned char*, COLOR);
    void abs(unsigned char*, COLOR);
    void min(unsigned char*, unsigned char, COLOR);
    void max(unsigned char*, unsigned char, COLOR);

    void inverse(unsigned char*);
    void grayscale(unsigned char*);

    unsigned char* filter(Pixel*, float*, int);

    //void do_operation(unsigned char* image_ptr, const string& operation_name) {
//    func_hashmap[operation_name](image_ptr, 5);
//}

}

#endif