#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "stack.h"
#include "decompress.h"

// ... read_bit function stays the same ...
// Helper: Read one bit from file
unsigned char read_bit(FILE* file, unsigned char* bit_buffer, int* bit_count, int* bits_remaining) {
    if (*bit_count == 0) {
        if (*bits_remaining > 0) {
            fread(bit_buffer, 1, 1, file);
            *bit_count = 8;
            (*bits_remaining)--;
        } else {
            return 2; // End of file
        }
    }
    
    unsigned char bit = (*bit_buffer >> 7) & 1;  // Get MSB
    *bit_buffer <<= 1;  // Shift left
    (*bit_count)--;
    
    return bit;
}
void decompress_file(const char* input_filename, const char* output_filename) {
    printf("Starting decompression of %s\n", input_filename);
    
    // 1. Open compressed file
    FILE* in = fopen(input_filename, "rb");
    if (!in) {
        printf("Error opening compressed file: %s\n", input_filename);
        return;
    }
    
    // 2. Read frequency table
    int freq[256];
    size_t read_items = fread(freq, sizeof(int), 256, in);
    if (read_items != 256) {
        printf("Error reading frequency table\n");
        fclose(in);
        return;
    }
    
    // 3. Read padding info
    int last_bits_count;
    fread(&last_bits_count, sizeof(int), 1, in);
    
    // 4. Build Huffman tree
    char* codes[256];
    Node* root = build_huffman_from_freq(freq, codes);
    
    // 5. Open output file
    FILE* out = fopen(output_filename, "wb");
    if (!out) {
        printf("Error opening output file: %s\n", output_filename);
        fclose(in);
        return;
    }
    
    // 6. Calculate file sizes (same as before)
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in);
    fseek(in, 256 * sizeof(int) + sizeof(int), SEEK_SET);
    
    long header_size = 256 * sizeof(int) + sizeof(int);
    long compressed_data_size = file_size - header_size;
    int total_bytes_to_read = (int)compressed_data_size;
    
    if (last_bits_count == 0) {
        last_bits_count = 8;
    }
    
    // 7. Initialize STACK for tree traversal
    Stack stack;
    initStack(&stack);
    push(&stack, root);  // Start with root on stack
    
    // 8. Initialize bit reading
    unsigned char bit_buffer = 0;
    int bit_count = 0;
    
    // 9. Calculate total original bytes
    int total_bytes = 0;
    for (int i = 0; i < 256; i++) {
        total_bytes += freq[i];
    }
    
    printf("Decompressing %d original bytes...\n", total_bytes);//checkpoints
    printf("(Using LIFO stack for tree traversal)\n");
    
    // 10. Decode bits USING STACK
    int bytes_decoded = 0;
    
    while (bytes_decoded < total_bytes) {
        // Read one bit
        unsigned char bit = read_bit(in, &bit_buffer, &bit_count, &total_bytes_to_read);
        
        if (bit == 2) {  // End of file
            printf("Warning: Reached EOF before decoding all bytes\n");
            break;
        }
        
        // Get current position from stack (LIFO - Last In First Out)
        Node* current = peek(&stack);
        if (!current) {
            printf("Error: Stack is empty!\n");
            break;
        }
        
        // Navigate based on bit
        if (bit == 0) {
            if (current->left == NULL) {
                printf("Error: Invalid bit stream\n");
                break;
            }
            // Go deeper - push left child onto stack
            push(&stack, current->left);
        } else {
            if (current->right == NULL) {
                printf("Error: Invalid bit stream\n");
                break;
            }
            // Go deeper - push right child onto stack
            push(&stack, current->right);
        }
        
        // Get new position (top of stack)
        current = peek(&stack);
        
        // Check if we reached a leaf
        if (current->left == NULL && current->right == NULL) {
            // Found a character - write it
            fwrite(&(current->symbol), 1, 1, out);
            bytes_decoded++;
            
            // Go back to root for next character
            // Pop everything until we're back at root
            while (!isEmpty(&stack) && peek(&stack) != root) {
                pop(&stack);
            }
            
            // Stack should now have just root
            // If stack got empty somehow, push root back
            if (isEmpty(&stack)) {
                push(&stack, root);
            }
        }
    }
    
    // 11. Clean up
    freeStack(&stack);
    fclose(in);
    fclose(out);
    
    // Free codes memory
    for (int i = 0; i < 256; i++) {
        if (codes[i] != NULL) {
            free(codes[i]);
        }
    }
    
    printf("Decompression completed: %s -> %s\n", input_filename, output_filename);
    printf("Decoded %d/%d bytes\n", bytes_decoded, total_bytes);
    
    if (bytes_decoded == total_bytes) {
        printf("SUCCESS: All bytes decoded correctly!\n");
    } else {
        printf(" WARNING: Only %d/%d bytes decoded\n", bytes_decoded, total_bytes);
    }
}