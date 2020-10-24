#include "BSTree.h"
#define DEBUG 1

BSTree::BSTree() {
    tree = NULL;
}

BSTree::~BSTree() {
    clearTree(tree);
}

void BSTree::clear() {
    clearTree(tree);
}

void BSTree::clearTree(BSTNode *&t) {
    if (t != NULL) {
        clearTree(t->left);
        clearTree(t->right);
        std::cout << t->key << "清理！" << std::endl;
        delete t;
        t = NULL;
    }
}

void BSTree::insertNodeOld(BSTNode* &t, const String& key) {
    if (t == NULL) {
        t = new BSTNode;
        t->key = key;
        t->left = t->right = NULL;
    } else {
        if (key != t->key) {
            if (key < t->key) {
                insertNodeOld(t->left, key);
            } else {
                insertNodeOld(t->right, key);
            }
        } else {
            std::cout << key << "已经存在！" << std::endl;
        }
    }
}

BSTNode* BSTree::findNode(BSTNode* t, const String& key) const {
    if (t == NULL)return NULL;
    if (key == t->key)return t;
    if (key < t->key)
        return findNode(t->left, key);
    else
        return findNode(t->right, key);
}

void BSTree::displayTree(BSTNode* t) const {
    if (t != NULL) {
        displayTree(t->left);
        std::cout << t->key << std::endl;
        displayTree(t->right);
    }
}

void BSTree::preorderTraversal(BSTNode* t) const {
    if (t != NULL) {
        std::cout << t->key << std::endl;
        preorderTraversal(t->left);
        preorderTraversal(t->right);
    }
}

void BSTree::postorderTraversal(BSTNode* t) const {
    if (t != NULL) {
        postorderTraversal(t->left);
        postorderTraversal(t->right);
        std::cout << t->key << std::endl;
    }
}

void BSTree::insertNode(BSTNode*& t, const String& key) {
    insertAVL(t, key);
}

int BSTree::insertAVL(BSTNode*& t, const String& key) {
    if (t == NULL) {
        t = new BSTNode;
        t->key = key;
        t->bf = 0;
        t->left = t->right = NULL;
        return +1;
    }
    if (key == t->key) {
        std::cout << key << "已经存在！" << std::endl;
        return 0;
    }
    if (key < t->key) {
        int delta = insertAVL(t->left, key); //三角符号△
        if (delta == 0)return 0;
        switch (t->bf) {
            case +1:
                t->bf = 0;
                return 0;
            case 0:
                t->bf = -1;
                return +1;
            case -1:
                fixLeftImbalance(t);
                return 0;
        }
    } else {
        int delta = insertAVL(t->right, key);
        if (delta == 0)return 0;
        switch (t->bf) {
            case -1:
                t->bf = 0;
                return 0;
            case 0:
                t->bf = +1;
                return +1;
            case +1:
                fixRightImbalance(t);
                return 0;
        }
    }
}

void BSTree::fixLeftImbalance(BSTNode*& t) {
    BSTNode* child = t->left;
    if (child->bf != t->bf) {
        int oldbf = child->right->bf;
        rotateLeft(t->left);
        rotateRight(t);
        t->bf = 0;
        switch (oldbf) {
            case -1:
                t->left->bf = 0;
                t->right->bf = +1;
                break;
            case 0:
                t->left->bf = 0;
                t->right->bf = 0;
                break;
            case +1:
                t->left->bf = -1;
                t->right->bf = 0;
                break;
        }
    } else {
        rotateRight(t);
        t->right->bf = t->bf = 0;
    }
}

void BSTree::rotateLeft(BSTNode*& t) {
    BSTNode* child = t->right;
    if (DEBUG) {
        std::cout << "rotateLeft(" << t->key << "-" << child->key << ")" << std::endl;
    }
    t->right = child->left;
    child->left = t;
    t = child;
}

void BSTree::fixRightImbalance(BSTNode*& t) {
    BSTNode* child = t->right;
    if (child->bf != t->bf) {
        int oldbf = child->left->bf;
        rotateRight(t->right);
        rotateLeft(t);
        t->bf = 0;
        switch (oldbf) {
            case -1:
                t->left->bf = 0;
                t->right->bf = +1;
                break;
            case 0:
                t->left->bf = 0;
                t->right->bf = 0;
                break;
            case +1:
                t->left->bf = -1;
                t->right->bf = 0;
                break;
        }
    } else {
        rotateLeft(t);
        t->left->bf = t->bf = 0;
    }
}

void BSTree::rotateRight(BSTNode*& t) {
    BSTNode* child = t->left;
    if (DEBUG) {
        std::cout << "rotateRight(" << t->key << "-" << child->key << ")" << std::endl;
    }
    t->left = child->right;
    child->right = t;
    t = child;
}

void BSTree::removeNode(BSTNode*& t, const String& key) {
    removeAVL(t, key);
}

int BSTree::removeAVL(BSTNode*& t, const String& key) {
    if (t == NULL) {
        return 0;
    }
    BSTNode *parent = NULL;
    BSTNode *move = NULL;
    if (key == t->key) {
        std::cout << key << " removing！" << std::endl;
        //        if (t->left == NULL && t->right == NULL) {
        //            delete t;
        //            return 1;
        //        }

        switch (t->bf) {
            case +1:
                //                t->bf = 0;
                move = t->right;
                if (move->left == NULL) {
                    t->key = move->key;
                    t->right = move->right;
                    delete move;
                    t->bf = 0;
                    return 1;
                }
                while (move->left != NULL) {
                    parent = move;
                    move = move->left;
                }
                //                String tmp = move->key;
                int delta;
                if (move->right == NULL) {
                    delta = removeAVL(move, move->key);
                    parent->left = NULL;
                } else {
                    delta = removeAVL(move, move->key);
                }
                //                t->key = tmp;
                if (delta == 0)return 0;
                //                parent->left = NULL;
                /*switch (parent->bf) {
                        if (parent->right != NULL) {
                            parent->bf = -1;
                            t->bf = 0;
                            return 0;
                        }
                }*/
            case 0:
                if (t->left == NULL && t->right == NULL) {
                    delete t;
                    return 1;
                }
                t->bf = -1;
                move = t->left;
                t->key = move->key;
                t->left = move->left;
                delete move;
                return 0;
            case -1:
                t->bf = 0;
                move = t->left;
                t->key = move->key;
                t->left = move->left;
                delete move;
                //                fixLeftImbalance(t);
                return 1;
        }
    }
    if (key < t->key) {
        int delta = removeAVL(t->left, key);
        if (delta == 0)return 0;
        switch (t->bf) {
            case +1:
                t->bf = 0;
                return 0;
            case 0:
                t->bf = -1;
                return +1;
            case -1:
                fixLeftImbalance(t);
                return 0;
        }
    } else {
        int delta = removeAVL(t->right, key);
        if (delta == 0)return 0;
        switch (t->bf) {
            case -1:
                t->bf = 0;
                return 0;
            case 0:
                t->bf = +1;
                return +1;
            case +1:
                fixRightImbalance(t);
                return 0;
        }
    }
}
