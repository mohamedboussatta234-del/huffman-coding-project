#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "compress.h"

void compress_file(const char* input_filename, const char* output_filename) {
    printf("Starting compression of %s\n", input_filename);
    
    // 1. Build frequency table
    int freq[256] = {0};
    build_frequency_table(input_filename, freq);
    
    // 2. Build Huffman tree and codes
    char* codes[256];
    Node* root = build_huffman_from_freq(freq, codes);
    (void)root;  // Silences "unused variable" warning
    
    // 3. Open input file
    FILE* in = fopen(input_filename, "rb");
    if (!in) {
        printf("Error opening input file\n");
        return;
    }
    
    // 4. Open output file
    FILE* out = fopen(output_filename, "wb");
    if (!out) {
        printf("Error opening output file\n");
        fclose(in);
        return;
    }
    
    // 5. Write frequency table (for decompression)
    fwrite(freq, sizeof(int), 256, out);
    
    // 6. Bit packing variables
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    
    // 7. Encode file
    unsigned char byte;
    while (fread(&byte, 1, 1, in) == 1) {
        char* code = codes[byte];
        for (int i = 0; code[i] != '\0'; i++) {
            bit_buffer <<= 1;
            if (code[i] == '1') {
                bit_buffer |= 1;
            }
            bit_count++;
            
            if (bit_count == 8) {
                fwrite(&bit_buffer, 1, 1, out);
                bit_buffer = 0;
                bit_count = 0;
            }
        }
    }
    
    // 8. Handle remaining bits
    int last_bits_count = bit_count;
    if (bit_count > 0) {
        bit_buffer <<= (8 - bit_count);
        fwrite(&bit_buffer, 1, 1, out);
    }
    
    // 9. Write how many bits were valid in last byte
    fwrite(&last_bits_count, sizeof(int), 1, out);
    
    // 10. Close files and free memory
    fclose(in);
    fclose(out);
    
    // Free codes memory
    for (int i = 0; i < 256; i++) {
        if (codes[i] != NULL) {
            free(codes[i]);
        }
    }
    
    printf("Compression completed: %s -> %s\n", input_filename, output_filename);
}