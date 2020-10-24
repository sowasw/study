#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include "AVLTree.h"
#include <time.h>
#include <unistd.h>

using std::stack;


int removeAVL(BSTnode * & t, const BSTKEY & key);
int insertAVL(BSTnode * & t, const BSTKEY & key);
int updateBf(BSTnode *&t, bool & removeleftchild);
void fixLeftImbalance(BSTnode * & t);
void rotateLeft(BSTnode * & t);
void fixRightImbalance(BSTnode * & t);
void rotateRight(BSTnode * & t);
std::string bfString(int bf);
std::string integerToString_W(int n);



void displayTree(BSTnode *t, const std::string & prefix) {
    if (t != NULL) {
        std::cout << prefix << t->key << " (" << bfString(t->bf) << ")" << std::endl;
        int len = prefix.length();
        std::string indent = (len == 0) ? "" : prefix.substr(0, len - 2);
        displayTree(t->left, indent + "  L:");
        displayTree(t->right, indent + "  R:");
    }
}

void display(BSTnode *t) {
    if (t != NULL) {
        display(t->left);
        std::cout << t->key << " (" << bfString(t->bf) << ")" << std::endl;
        //std::cout << t->key << std::endl;
        display(t->right);
    }
}



void insertNode(BSTnode * & t, const BSTKEY & key) {
    insertAVL(t, key);
}

int removeNode(BSTnode * & t, const BSTKEY &key) {
    if (t == NULL)
        return 0;
    //    if (t->left == NULL && t->right == NULL) {
    //        delete t;
    //        t = NULL;
    //        return 0;
    //    }

    bool foundKey = false;
    bool parentRemoveLeftChild = true;
    int delta = 0;
    removeAVL(t,key);
    
//
//    //    BSTnode * parent = NULL;
//    BSTnode * bp = t;
//    stack<BSTnode **> s;
//    stack<bool> r;
//
//    s.push(&t);
//
//    while (bp != NULL) {
//        if (key == bp->key) {
//            if (bp->left == NULL && bp->right == NULL) {
//                delete bp;
//                *s.top() = NULL;
//                delta = -1;
//                s.pop();
//                break;
//            }
//            foundKey = true;
//            break;
//        } else if (key < bp->key) {
//            //            parent = bp;
//            s.push(&bp->left);
//            r.push(parentRemoveLeftChild);
//            bp = bp->left;
//        } else {
//            //            parent = bp;
//            s.push(&bp->right);
//            r.push(!parentRemoveLeftChild);
//            bp = bp->right;
//        }
//    }
//
//    //    if (!foundKey)
//    //        return 0;
//    if (foundKey) {
//        if (bp->left == NULL) {
//            *s.top() = bp->right;
//            delete bp;
//            s.pop();
//            delta = -1;
//
//        } else if (bp->right == NULL) {
//            *s.top() = bp->left;
//            delete bp;
//            s.pop();
//            delta = -1;
//        } else {
//            r.push(parentRemoveLeftChild);
//            BSTnode *successor = bp->left;
//            while (successor->right != NULL) {
//                successor = successor->right;
//            }
//            bp->key = successor->key;
//            delta = removeNode(bp->left, successor->key);
//        }
//    }
//
//    while (!r.empty()) {
//        if (delta != 0)
//            delta = updateBf(*s.top(), r.top());
//        s.pop();
//        r.pop();
//    }

    return delta;
}

int removeAVL(BSTnode * & t, const BSTKEY & key) {
    if (t == NULL)
        return 0;
    if (key == t->key) {
        ;
        //        if (t->left == NULL && t->right == NULL) {
        //            delete t;
        //            t = NULL;
        //            return -1;
        //        }
    } else if (key < t->key) {
        bool rl = true;
        int delta = removeAVL(t->left, key);
        if (delta == 0)
            return 0;
        delta = updateBf(t, rl);
        return delta;
    } else {
        bool rl = false;
        int delta = removeAVL(t->right, key);
        if (delta == 0)
            return 0;
        delta = updateBf(t, rl);
        return delta;
    }

    if (t->left == NULL) {
        BSTnode *bp = t;
        t = bp->right;
        delete bp;
        return -1;
    } else if (t->right == NULL) {
        BSTnode *bp = t;
        t = bp->left;
        delete bp;
        return -1;
    } else {

        BSTnode *successor = t->left;
        while (successor->right != NULL) {
            successor = successor->right;
        }
        t->key = successor->key;
        int delta = removeAVL(t->left, successor->key);
        if (delta == 0)
            return 0;
        bool rl = true;
        delta = updateBf(t, rl);
        return delta;
    }
}

int updateBf(BSTnode *& t, bool & removeLeftChild) {
    //        t->bf -= -1;
    //    if (t->bf < BST_LEFT_HEAVY) {
    //        fixLeftImbalance(t);
    //        return -1;
    //    } else if (t->bf > BST_RIGHT_HEAVY) {
    //        fixRightImbalance(t);
    //        return -1;
    //    }

    if (removeLeftChild) {
        switch (t->bf) {
            case -1:
                //                fixLeftImbalance(t);
                t->bf = 0;
                return -1;
            case 0:
                t->bf = +1;
                return 0;
            case +1:
                fixRightImbalance(t);
                return -1;
        }
    } else {
        switch (t->bf) {
            case -1:
                fixLeftImbalance(t);
                return -1;
            case 0:
                t->bf = -1;
                return 0;
            case +1:
                t->bf = 0;
                return -1;
        }
    }

}

void clearTree(BSTnode * & t) {
    if (t != NULL) {
        clearTree(t->left);
        clearTree(t->right);
        delete t;
        t = NULL;
    }
}


int insertAVL(BSTnode * & t, const BSTKEY & key) {
    if (t == NULL) {
        t = new BSTnode;
        t->key = key;
        t->bf = 0;
        t->left = t->right = NULL;
        return +1;
    }
    if (key == t->key) return 0;
    if (key < t->key) {
        int delta = insertAVL(t->left, key);
        if (delta == 0) return 0;
        switch (t->bf) {
            case +1: t->bf = 0;
                return 0;
            case 0: t->bf = -1;
                return +1;
            case -1:
                fixLeftImbalance(t);
                return 0;
        }
    } else {
        int delta = insertAVL(t->right, key);
        if (delta == 0) return 0;
        switch (t->bf) {
            case -1: t->bf = 0;
                return 0;
            case 0: t->bf = +1;
                return +1;
            case +1:
                fixRightImbalance(t);
                return 0;
        }
    }
}


void fixLeftImbalance(BSTnode * & t) {
    BSTnode *child = t->left;
    if (child->bf == +1) {
        int oldBF = child->right->bf;
        rotateLeft(t->left);
        rotateRight(t);
        t->bf = 0;
        switch (oldBF) {
            case -1: t->left->bf = 0;
                t->right->bf = +1;
                break;
            case 0: t->left->bf = t->right->bf = 0;
                break;
            case +1: t->left->bf = -1;
                t->right->bf = 0;
                break;
        }
    } else if (child->bf == 0) {
        rotateRight(t);
        t->bf = +1;
        t->right->bf = -1;
    } else {
        rotateRight(t);
        t->right->bf = t->bf = 0;
    }
}


void rotateLeft(BSTnode * & t) {
    BSTnode *child = t->right;
#ifdef DEBUG_AVLTREE
    std::cout << "rotateLeft(" << t->key << "-" << child->key << ")" << std::endl;
#endif
    t->right = child->left;
    child->left = t;
    t = child;
}


void fixRightImbalance(BSTnode * & t) {
    BSTnode *child = t->right;
    if (child->bf == -1) {
        int oldBF = child->left->bf;
        rotateRight(t->right);
        rotateLeft(t);
        t->bf = 0;
        switch (oldBF) {
            case -1: t->left->bf = 0;
                t->right->bf = +1;
                break;
            case 0: t->left->bf = t->right->bf = 0;
                break;
            case +1: t->left->bf = -1;
                t->right->bf = 0;
                break;
        }
    } else if (child->bf == 0) {
        rotateLeft(t);
        t->bf = -1;
        t->left->bf = +1;
    } else {
        rotateLeft(t);
        t->left->bf = t->bf = 0;
    }
}


void rotateRight(BSTnode * & t) {
    BSTnode *child = t->left;
#ifdef DEBUG_AVLTREE
    std::cout << "rotateRight(" << t->key << "-" << child->key << ")" << std::endl;
#endif
    t->left = child->right;
    child->right = t;
    t = child;
}


std::string bfString(int bf) {
    if (bf == 0) return "0";
    if (bf < 0) return integerToString_W(bf);
    return "+" + integerToString_W(bf);
}

std::string integerToString_W(int n) {
    std::ostringstream os;
    os << n;
    return os.str();
}

bool removeNode_(BSTnode * & t, const BSTKEY & key) {
#ifdef DEBUG_AVLTREE
    std::cout << "removing key: " << key << std::endl;
#endif
    if (t == NULL) return false;
    int sign = compareKeys(key, t->key);
    if (sign == 0) return removeTargetNode(t);
    int bfDelta = BST_IN_BALANCE;
    if (sign < 0) {
        if (removeNode_(t->left, key)) bfDelta = BST_RIGHT_HEAVY;
    } else {
        if (removeNode_(t->right, key)) bfDelta = BST_LEFT_HEAVY;
    }
    updateBF(t, bfDelta);
    return bfDelta != 0 && t->bf == BST_IN_BALANCE;
}

bool removeTargetNode(BSTnode * & t) {
    BSTnode *toDelete = t;
    if (t->left == NULL) {
        t = t->right;
        delete toDelete;
        //        nodeCount--;
        return true;
    } else if (t->right == NULL) {
        t = t->left;
        delete toDelete;
        //        nodeCount--;
        return true;
    } else {
        BSTnode *successor = t->left;
#ifdef DEBUG_AVLTREE
        std::cout << "FOUND KEY! removing successor->key: " << successor->key << std::endl;
#endif
        while (successor->right != NULL) {
            successor = successor->right;
        }
        t->key = successor->key;
        //        t->value = successor->value;
        if (removeNode_(t->left, successor->key)) {
            updateBF(t, BST_RIGHT_HEAVY);
            return (t->bf == BST_IN_BALANCE);
        }
        return false;
    }
}

void updateBF(BSTnode * & t, int bfDelta) {
    t->bf += bfDelta;
    if (t->bf < BST_LEFT_HEAVY) {
        fixLeftImbalance(t);
    } else if (t->bf > BST_RIGHT_HEAVY) {
        fixRightImbalance(t);
    }
}

int compareKeys(const BSTKEY & k1, const BSTKEY & k2) {
    //      if (cmpp->lessThan(k1, k2)) return -1;
    //      if (cmpp->lessThan(k2, k1)) return +1;
    if ((k1 < k2)) return -1;
    if ((k1 > k2)) return +1;
    return 0;
}

void putNode(BSTnode * & t, const BSTKEY & key) {
    bool dummy;
    addNode(t, key, dummy);
}

void addNode(BSTnode * & t, const BSTKEY & key, bool & heightFlag) {
    heightFlag = false;
    if (t == NULL) {
        t = new BSTnode;
        t->key = key;
        t->bf = BST_IN_BALANCE;
        t->left = t->right = NULL;
        heightFlag = true;
        return;
    }
    int sign = compareKeys(key, t->key);
    if (sign == 0) return;
    int bfDelta = BST_IN_BALANCE;
    if (sign < 0) {
        addNode(t->left, key, heightFlag);
        if (heightFlag) bfDelta = BST_LEFT_HEAVY;
#ifdef DEBUG_AVLTREE
        std::cout << "heightFlag: " << heightFlag
                << " key: " << key << " < t->key: " << t->key << std::endl;
#endif
    } else {
        addNode(t->right, key, heightFlag);
        if (heightFlag) bfDelta = BST_RIGHT_HEAVY;
#ifdef DEBUG_AVLTREE
        std::cout << "heightFlag: " << heightFlag
                << " key: " << key << " > t->key: " << t->key << std::endl;
#endif
    }
    updateBF(t, bfDelta);
    heightFlag = (bfDelta != 0 && t->bf != BST_IN_BALANCE);
    return;
}
