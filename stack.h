#ifndef STACK_H
#define STACK_H

#include "bt.h"  // For Node structure

// Stack node for tree traversal
typedef struct StackNode {
    Node* treeNode;          // Pointer to Huffman tree node
    struct StackNode* next;  // Next in stack
} StackNode;

// Stack structure
typedef struct Stack {
    StackNode* top;
} Stack;

// Stack operations
void initStack(Stack* stack);
int isEmpty(Stack* stack);
void push(Stack* stack, Node* treeNode);
Node* pop(Stack* stack);
Node* peek(Stack* stack);
void freeStack(Stack* stack);

#endif