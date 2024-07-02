#include <el_bits.h>

unsigned int replaceByte(unsigned int num, unsigned char byte) {
    return (num & 0xFFFF00FF) | (byte << 8);
}