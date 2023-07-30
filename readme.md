# Architecture and organisation 2 - Project

TO RUN PROGRAM YOU NEED AVX2 ARCHITECTURE SET

## How to use:

- compile using: g++ -mavx2 ./src/main.cpp ./src/operations_optimized.cpp ./src/operations.cpp ./src/helper.cpp -o aor2
- run program with arguments: ./aor2 "image path" "output image extention" (OP)
- program arguments in brackets should be used only if optimisation is needed

# Algorithms:

- Image processing algorithms should be written in operations.txt file

# List of operations:

- ADD /COLOR /VALUE
- SUB /COLOR /VALUE
- MUL /COLOR /VALUE
- DIV /COLOR /VALUE
- SUBI /COLOR /VALUE
- DIVI /COLOR /VALUE
- MIN /COLOR /VALUE
- POW /COLOR /VALUE
- LOG /COLOR
- ABS /COLOR
- INVERSE
- GRAYSCALE
- FILTER /MATRIX_SIZE /MATRIX_ELEMENTS...