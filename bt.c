#include <stdio.h>
#include <stdlib.h>
#include "bt.h"


Node* createNode(unsigned char symbol, int freq)
{
    Node *Newnode = (Node*)malloc(sizeof(Node));
    if (Newnode == NULL)
        return NULL;
    Newnode->symbol = symbol;
    Newnode->freq = freq;
    Newnode->left = Newnode->right = NULL;
    return Newnode;
}


Node* insertNode(Node* root, int freq)
{
    if(root == NULL)
    {
        return createNode('\0', freq);
    }
    if(freq < root->freq)
    {
        root->left = insertNode(root->left, freq);
    }
    else if (freq > root->freq)
    {
        root->right = insertNode(root->right, freq);
    }
    return root;
}

Node* searchNode(Node* root, int freq)
{
    if (root == NULL || root->freq == freq)
        return root;

    if (freq < root->freq)
        return searchNode(root->left, freq);
    else
        return searchNode(root->right, freq);
}

Node* findMin(Node* root)
{
    while(root && root->left)
        root = root->left;
    return root;
}


Node* deleteNode(Node* root, int freq)
{
    if(root == NULL)
        return root;

    if(freq < root->freq)
        root->left = deleteNode(root->left, freq);
    else if(freq > root->freq)
        root->right = deleteNode(root->right, freq);
    else
    {

        if(root->left == NULL)
        {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        if(root->right == NULL)
        {
            Node* temp = root->left;
            free(root);
            return temp;
        }


        Node* temp = findMin(root->right);
        root->freq = temp->freq;
        root->symbol = temp->symbol;
        root->right = deleteNode(root->right, temp->freq);
    }
    return root;
}


void inorderTraversal(Node* root)
{
    if(root != NULL)
    {
        inorderTraversal(root->left);
        printf("[%c|%d] ", root->symbol ? root->symbol : ' ', root->freq);
        inorderTraversal(root->right);
    }
}
