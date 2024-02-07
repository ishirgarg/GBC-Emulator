#ifndef UTILBYTES_INCLUDED_H
#define UTILBYTES_INCLUDED_H

#include <inttypes.h>

int16_t mergeBytes(int8_t MSB, int8_t LSB) {
    return (((uint16_t) MSB) << 8) | ((uint16_t) LSB);
}

#endif
