#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "bt.h"

#define MAX_TREE_HT 256

// Tree functions
int isleaf(Node* node);
void swapNodes(Node** a, Node** b);
void sortNodes(Node* nodes[], int n);
Node* buildHuffmanTree(unsigned char data[], int freq[], int size);
void printCodes(Node* root, int tabl[], int top);
void storeCodes(Node* root, char* codes[256], int arr[], int top);

// File functions  
void huffmanCodingFromFile(const char* filename);

// Compression helper functions
void build_frequency_table(const char *input_filename, int freq[256]);
int count_distinct_symbols(int freq[256]);
void extract_symbols(int freq[256], unsigned char data[], int frequencies[], int size);
Node* build_huffman_from_freq(int freq[256], char* codes[256]);

#endif