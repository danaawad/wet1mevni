//
// Created by Dana Awad on 5/12/23.
//

#ifndef WET1_AVL_H
#define WET1_AVL_H

template <class T>
class Node{
public:
    T* obj;
    int height;
    Node<T> *left;
    Node<T> *right;

    Node(T* obj) : obj(obj), height(1) {
        left = nullptr;
        right = nullptr;
    }
};

template <class T>
class AvlTree {
public:
    Node<T> *root;

    AvlTree();
    int getHeight(Node<T>* node);
    int max(int id1, int id2);
    int getBalance(Node<T> *node);
    int getBalanceFactor(Node<T> node);


    bool isBalanced();
    Node<T>* LL_rotate(Node<T>* node);
    Node<T>* RR_rotate(Node<T> *node);
    void LR_rotate();
    void RL_rotate();

    Node<T>* insert(Node<T>* node, int id);
    void find(int id);
    void remove(int id);
};



class Movie{
    int movieId;
    int genre;
    int views;
    bool vipOnly;
    int rating;
};

class User{
    int userId;
    bool isVip;
    int inGroupId;
    int genreCount[4];
};


#endif //WET1_AVL_H
