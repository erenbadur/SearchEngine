#pragma once

#ifndef AVLSEARCHTREE_H
#define AVLSEARCHTREE_H

#include <iostream>
#include <string>
using namespace std;

template <class Key, class Value>
class AVLSearchTree;

struct DocumentItem;
struct WordItem;

template <class Key, class Value>
class AVLSearchTree {
public:
    AVLSearchTree();
    ~AVLSearchTree();

    void insert(const Key & key, Value value);
    void remove(const Key & key);
    Value* find(const Key& key) const;
    void clear();
    bool contains(const Key & key) const;
    void print() const;
    static const int ALLOWED_IMBALANCE = 1;

private:
    struct AvlNode {
        Key key;
        Value value;
        AvlNode *left;
        AvlNode *right;
        int height;

        AvlNode(const Key & k, Value v, AvlNode *lt, AvlNode *rt, int h = 0) {
            key = k;
            value = v;
            left = lt;
            right = rt;
            height = h;
        }
    };

    AvlNode *root;

    template <class T>
    inline const T& max(const T& a, const T& b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    void destroyTree(AvlNode * & t);
    void insert(const Key & key, Value value, AvlNode * & t);
    void remove(const Key & key, AvlNode * & t);
    Value* find(const Key& key, AvlNode* t) const;
    AvlNode* findMin(AvlNode* t) const;
    AvlNode* findMax(AvlNode* t) const;
    bool contains(const Key & key, AvlNode *t) const;
    void balance(AvlNode * & t);
    int height(AvlNode *t) const;
    void rotateWithLeftChild(AvlNode * & k2);
    void rotateWithRightChild(AvlNode * & k1);
    void doubleWithLeftChild(AvlNode * & k3);
    void doubleWithRightChild(AvlNode * & k4);
    void makeEmpty(AvlNode * & t);
    void print(AvlNode *node) const;
};

#include "AVLSearchTree.cpp"

#endif