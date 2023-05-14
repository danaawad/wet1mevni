//
// Created by Dana Awad on 5/12/23.
//

#include "AVL.h"
template <class T>
AvlTree<T>::AvlTree(){
    root = nullptr;
}

template <class T>
int AvlTree<T>::getHeight(Node<T> *node) {
        if(node == nullptr) return -1;
        return node->height;
}

template <class T>
int AvlTree<T>:: getBalance(Node<T> *node){
        if(node == nullptr) return -1;
        return height(node->left) - height(node->right);
}


template <class T>
int AvlTree<T>::max(int x, int y){
    return x > y ? x : y;
}

//rotate right
template <class T>
Node<T>* AvlTree<T>::LL_rotate(Node<T> *node){
    Node<T> *x = node->left;
    Node<T> *xRight = x->right;

    x->right = node;
    node->left = xRight;

    node->height = max(height(node->left), height(node->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

//rotate left
template <class T>
Node<T>* AvlTree<T>::RR_rotate(Node<T> *node){

    Node<T> *x = node->right;
    Node<T> *xLeft = x->left;

    x->left = node;
    node->right = xLeft;

    node->height = max(height(node->left), height(node->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Insert a node
template <class T>
Node<T>* AvlTree<T>::insert(Node<T> *node, int id) {
    // Find the correct postion and insert the node
    if (node == nullptr)
        return (Node(id));
    if (id < node->id)
        node->left = insert(node->left, id);
    else if (id > node->key)
        node->right = insert(node->right, id);
    else
        return node;

    // Update the balance factor of each node and
    // balance the tree
    node->height = 1 + max(height(node->left),
                           height(node->right));
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (id < node->left->key) {
            return rightRotate(node);
        } else if (id > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    if (balanceFactor < -1) {
        if (id > node->right->key) {
            return leftRotate(node);
        } else if (id < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}

