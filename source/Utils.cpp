#include <stdint-gcc.h>
#include "Utils.hpp"

uint16_t Utils::swapEndian16(uint16_t n) {
    return (n >> 8) | (n << 8);
}

uint32_t Utils::swapEndian32(uint32_t n) {
    return ((n >> 24) & 0xFF) | ((n << 24) & 0xFF000000) | ((n << 8) & 0xFF0000) | ((n >> 8) & 0xFF00);
}
