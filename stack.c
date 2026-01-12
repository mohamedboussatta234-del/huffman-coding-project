#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Initialize stack
void initStack(Stack* stack) {
    stack->top = NULL;
}

// Check if stack is empty
int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

// Push a tree node onto stack
void push(Stack* stack, Node* treeNode) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) {
        printf("Memory allocation error in push!\n");
        return;
    }
    newNode->treeNode = treeNode;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pop from stack
Node* pop(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    StackNode* temp = stack->top;
    Node* treeNode = temp->treeNode;
    stack->top = temp->next;
    free(temp);
    return treeNode;
}

// Peek at top of stack
Node* peek(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->top->treeNode;
}

// Free stack memory
void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}