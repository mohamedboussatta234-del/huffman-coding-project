#ifndef BT_H
#define BT_H
typedef struct node {
                    unsigned char symbol;
                    int freq ;
                    struct node *left;
                    struct node *right;
                    } Node;
Node* createNode( unsigned char symbol , int freq);

Node* insertNode(Node* root, int freq);
Node* searchNode(Node* root , int freq);
Node* deleteNode(Node* root , int freq);

void inorderTraversal(Node *root);
#endif // BT_H
