# Architecture and organization 2 - Project

## How to use:

- Compile using command: g++ -mavx2 ./src/main.cpp ./src/operations_optimized.cpp ./src/operations.cpp ./src/helper.cpp -o aor2
- Run program using command line arguments: ./aor2 "image path" "output image extention" (OP)
- Program arguments in brackets should be used only if optimization is needed
- Image processing operations should be written in operations.txt file before running program
- Note: In order to run program in optimized mode you need avx2 architecture set

## List of operations:
- ADD /COLOR /VALUE - adds /VALUE to the current pixel value with chosen /COLOR
- SUB /COLOR /VALUE - substracts /VALUE of the current pixel value with chosen /COLOR
- MUL /COLOR /VALUE - multiplies /VALUE to the current pixel value with chosen /COLOR
- DIV /COLOR /VALUE - divides /VALUE of the current pixel value with chosen /COLOR
- SUBI /COLOR /VALUE - substracts the current pixel value of /VALUE with chosen /COLOR
- DIVI /COLOR /VALUE - divides the current pixel value of /VALUE with chosen /COLOR
- MIN /COLOR /VALUE - current pixel value of chosen /COLOR will have minimum value between current pixel value and /VALUE
- MAX /COLOR /VALUE - current pixel value of chosen /COLOR will have maximum value between current pixel value and /VALUE
- POW /COLOR /VALUE
- LOG /COLOR
- ABS /COLOR
- INVERSE
- GRAYSCALE
- FILTER /MATRIX_SIZE /MATRIX_ELEMENTS...

##### Note:
- /Color has values of R, G or B depending of which color is chosen for 
- /Value has values of CHAR or FLOAT type
