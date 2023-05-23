//
// Created by Dana Awad on 5/12/23.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H
#include <iostream>
#include <stack>

using namespace std;


template <class T>
struct Node{
    T* obj;
    int height;
    Node<T> *left;
    Node<T> *right;
    int m_key;

    explicit Node(T* obj, int key = 0): obj(obj), left(nullptr), right(nullptr), height(0), m_key(key) {}
};

template <class T>
class AvlTree {
public:
    Node<T> *root;

    explicit AvlTree<T>():root(nullptr) {}
    ~AvlTree<T>();
    int getHeight(Node<T>* node);
    int getBalanceFactor(Node<T> *node);
    void inorderPrint();
    Node<T>* LL_rotate(Node<T>* node);
    Node<T>* RR_rotate(Node<T> *node);
    Node<T>* LR_rotate(Node<T> *node);
    Node<T>* RL_rotate(Node<T> *node);
    Node<T>* insertNode(Node<T> *node, T* obj);
    Node<T>* insert(T* obj);
    Node<T>* find(T* obj);
    void remove(T* obj);
    Node<T>* getFirstNodeInOrder(Node<T>* node);
    Node<T>* getFirstInOrder();
    Node<T>* getLastNodeInOrder(Node<T>* node);
    Node<T>* getLastInOrder();



    //help functions
    static Node<T>* findNode(Node<T>* root, T* obj)
    {
        if (root == nullptr || *(root->obj) == *obj) {
            return root;
        }

        if (*obj < *(root->obj)) {
            return findNode(root->left, obj);
        } else {
            return findNode(root->right, obj);
        }
    }

    void destroyTree(Node<T>* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node<T>* findMinNode(Node<T>* node)
    {
        if (node == nullptr || node->left == nullptr) {
            return node;
        }
        return findMinNode(node->left);
    }

    int max(int x, int y)
    {
        return x > y ? x : y;
    }



    void inorderPrintAux(Node<T>* node)
    {
        if (node == nullptr)
        {
            return;
        }

        inorderPrintAux(node->left);
        cout << *(node->obj) << "" ;
        inorderPrintAux(node->right);
    }
    Node<T>* removeNode(Node<T>* node, T* obj) {
        if (node == nullptr)
        {
            return nullptr;
        }

        if (*obj < *(node->obj))
        {
            node->left = removeNode(node->left, obj);
        }
        else if (*obj > *(node->obj))
        {
            node->right = removeNode(node->right, obj);
        }
        else
        {
            if (node->left == nullptr || node->right == nullptr) {
                Node<T>* temp = node->left ? node->left : node->right;

                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }

                delete temp;
            }
            else
            {
                Node<T>* temp = findMinNode(node->right);
                node->obj = temp->obj;
                node->right = removeNode(node->right, temp->obj);
            }
        }

        if (node == nullptr) {
            return nullptr;
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor == 2)
        {
            if (getBalanceFactor(node->left) >= 0)
            {
                return LL_rotate(node);
            }
            else if (getBalanceFactor(node->left) == -1) {
                return LR_rotate(node);
            }
        }
        if (balanceFactor == -2)
        {
            if (getBalanceFactor(node->right) == 1)
            {
                return RL_rotate(node);
            }
            else if (getBalanceFactor(node->right) <= 0)
            {
                return RR_rotate(node);
            }
        }
        return node;
    }
};


template <class T>
int AvlTree<T>::getHeight(Node<T> *node) {
    if(node == nullptr) return -1;
    return node->height;
}

template <class T>
int AvlTree<T>:: getBalanceFactor(Node<T> *node){
    if(node == nullptr) return -1;
    return getHeight(node->left) - getHeight(node->right);
}


//rotate right
template <class T>
Node<T>* AvlTree<T>::LL_rotate(Node<T> *node){
    Node<T> *x = node->left;
    Node<T> *xRight = x->right;

    x->right = node;
    node->left = xRight;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    if(node == this->root)
    {
        this->root = x;
    }
    return x;
}

//rotate left
template <class T>
Node<T>* AvlTree<T>::RR_rotate(Node<T> *node){

    Node<T> *x = node->right;
    Node<T> *xLeft = x->left;

    x->left = node;
    node->right = xLeft;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    if(node == this->root)
    {
        this->root = x;
    }
    return x;
}

//RL rotate
template <class T>
Node<T>* AvlTree<T>::RL_rotate(Node<T> *node)
{

    node->right = LL_rotate(node->right);
    return RR_rotate(node);
}

//LR rotate
template <class T>
Node<T>* AvlTree<T>::LR_rotate(Node<T> *node)
{
    node->left = RR_rotate(node->left);
    return LL_rotate(node);
}

template <class T>
Node<T>* AvlTree<T>::insert(T *obj)
{
    return insertNode(root, obj);
}

template <class T>
Node<T>* AvlTree<T>::insertNode(Node<T> *node, T* obj) {
    // Find the correct postion and insert the node
    if (node == nullptr)
    {
        Node<T>* newNode = new Node<T>(obj);
        if(this->root == nullptr)
        {
            this->root = newNode;
        }
        return newNode;
    }

    if (*obj < *(node->obj))
        node->left = insertNode(node->left, obj);
    else if (*obj > *(node->obj))
        node->right = insertNode(node->right, obj);
    else
        node->obj = obj;

    // Update the balance factor of each node and
    // balance the tree
    node->height = 1 + max(getHeight(node->left),
                           getHeight(node->right));
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor == 2)
    {
        if (getBalanceFactor(node->left) >= 0)
        {
            return LL_rotate(node);
        }
        else if (getBalanceFactor(node->left)== -1)
        {
            return LR_rotate(node);
        }
    }
    if (balanceFactor == -2)
    {
        if (getBalanceFactor(node->right) == 1)
        {
            return RL_rotate(node);
        }
        else if (getBalanceFactor(node->right) <= 0)
        {
            return RR_rotate(node);
        }
    }
    return node;
}

template <class T>
Node<T>* AvlTree<T>::find(T* obj)
{
    return findNode(root, obj);
}

template <class T>
void AvlTree<T>::remove(T* obj)
{
    root = removeNode(root, obj);
}


template<class T>
AvlTree<T>::~AvlTree()
{
    destroyTree(root);
}

template<class T>
void AvlTree<T>::inorderPrint()
{
    inorderPrintAux(this->root);
}

template<class T>
Node<T>* AvlTree<T>::getFirstInOrder(){
    return getFirstNodeInOrder(this->root);
}

template<class T>
Node<T>* AvlTree<T>::getFirstNodeInOrder(Node<T>* node) {
    if (root == nullptr) {
        return nullptr;
    }
    Node<T>* current = root;
    while (current->left != nullptr) {
        current = current->left;
    }
    return current;
}



template<class T>
Node<T>* AvlTree<T>::getLastInOrder(){
    return getLastNodeInOrder(this->root);
}

template<class T>
Node<T>* AvlTree<T>::getLastNodeInOrder(Node<T>* node) {
    if (root == nullptr) {
        return nullptr;
    }
    Node<T>* current = root;
    while (current->right != nullptr) {
        current = current->right;
    }
    return current;
}
#endif //WET1_AVL_H