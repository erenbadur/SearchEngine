#ifndef AVLSEARCHTREE_CPP
#define AVLSEARCHTREE_CPP

#include "AVLSearchTree.h"

template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree() {
    root = nullptr;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::destroyTree(AvlNode * & t) {
    if (t != nullptr) {
        destroyTree(t->left);
        destroyTree(t->right);
        delete t;
        t = nullptr;
    }
}

template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() {
    destroyTree(root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key & key, Value value) {
    insert(key, value, root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key & key) {
    remove(key, root);
}


template <class Key, class Value>
Value* AVLSearchTree<Key, Value>::find(const Key& key) const {
    return find(key, root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::clear() {
    destroyTree(root);
    root = nullptr;
}

template <class Key, class Value>
bool AVLSearchTree<Key, Value>::contains(const Key & key) const {
    return contains(key, root);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::print() const {
    print(root);
}



template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key & key, Value value, typename AVLSearchTree<Key, Value>::AvlNode * & t) {
    if (t == nullptr) {
        t = new AvlNode(key, value, nullptr, nullptr);
    }
    else if (key < t->key) {
        insert(key, value, t->left);
    }
    else if (t->key < key) {
        insert(key, value, t->right);
    }
    else
    {
        t->value = value;
    }
    balance(t);
}


template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key & key, typename AVLSearchTree<Key, Value>::AvlNode * & t) {
    if (t == nullptr) {
        return;
    }

    if (key < t->key) {
        remove(key, t->left);
    }
    else if (t->key < key) {
        remove(key, t->right);
    }
    else if (t->left != nullptr && t->right != nullptr) {
        t->key = findMin(t->right)->key;
        t->value = findMin(t->right)->value;
        remove(t->key, t->right);
    }
    else {
        AvlNode *oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }

    if (t != nullptr) {
        balance(t);
    }
}

template <class Key, class Value>
Value* AVLSearchTree<Key, Value>::find(const Key & key, AvlNode *t) const {
    if (t == nullptr) {
        return nullptr;
    } else if (key < t->key) {
        return find(key, t->left);
    } else if (t->key < key) {
        return find(key, t->right);
    } else {
        return &(t->value);
    }
}

template <class Key, class Value>
typename AVLSearchTree<Key, Value>::AvlNode* AVLSearchTree<Key, Value>::findMin(AvlNode *t) const {
    if (t == nullptr) {
        return nullptr;
    } else if (t->left == nullptr) {
        return t;
    } else {
        return findMin(t->left);
    }
}

template <class Key, class Value>
typename AVLSearchTree<Key, Value>::AvlNode* AVLSearchTree<Key, Value>::findMax(AvlNode *t) const {
    if (t == nullptr) {
        return nullptr;
    } else if (t->right == nullptr) {
        return t;
    } else {
        return findMax(t->right);
    }
}

template <class Key, class Value>
bool AVLSearchTree<Key, Value>::contains(const Key & key, AvlNode *t) const {
    if (t == nullptr) {
        return false;
    }
    else if (key < t->key) {
        return contains(key, t->left);
    }
    else if (t->key < key) {
        return contains(key, t->right);
    }
    else {
        return true;
    }
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::balance(typename AVLSearchTree<Key, Value>::AvlNode * & t) {
    if (t == nullptr) {
        return;
    }

    if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE) {
        if (height(t->left->left) >= height(t->left->right)) {
            rotateWithLeftChild(t);
        }
        else {
            doubleWithLeftChild(t);
        }
    }
    else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) {
        if (height(t->right->right) >= height(t->right->left)) {
            rotateWithRightChild(t);
        }
        else {
            doubleWithRightChild(t);
        }
    }

    t->height = max(height(t->left), height(t->right)) + 1;
}


template <class Key, class Value>
int AVLSearchTree<Key, Value>::height(typename AVLSearchTree<Key, Value>::AvlNode *t) const {

    if (t == nullptr) {
        return -1;
    }
    else {
        return t->height;
    }
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(typename AVLSearchTree<Key, Value>::AvlNode * & k2) {
    AvlNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2 = k1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild(typename AVLSearchTree<Key, Value>::AvlNode * & k1) {
    AvlNode *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(typename AVLSearchTree<Key, Value>::AvlNode * & k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(typename AVLSearchTree<Key, Value>::AvlNode * & k4) {
    rotateWithLeftChild(k4->right);
    rotateWithRightChild(k4);
}


template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AvlNode * & t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }
}


template <class Key, class Value>
void AVLSearchTree<Key, Value>::print(AvlNode *node) const {
    if (node != nullptr) {
        print(node->left);
        print(node->right);
    }
}

template class AVLSearchTree<std::string, DocumentItem*>;

#endif