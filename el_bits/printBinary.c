#include <el_bits.h>

void printBinary(unsigned int n) {
    if (n > 1)
        printBinary(n >> 1);
    printf("%u", n & 1);
}