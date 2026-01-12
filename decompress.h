#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "bt.h"

// Decompression functions
void decompress_file(const char* input_filename, const char* output_filename);
unsigned char read_bit(FILE* file, unsigned char* bit_buffer, int* bit_count, int* bits_remaining);

#endif