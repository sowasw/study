#include "String.h"
#include <iostream>

#ifndef _BSTREE_H
#define _BSTREE_H

struct BSTNode {
    String key;
    BSTNode* left;
    BSTNode* right;
    int bf; //平衡因子
};

class BSTree {
public:
    BSTree();

    ~BSTree();
    void clear(); 
    void clearTree(BSTNode *&t); //清空数据//后序遍历

    void insertNodeOld(BSTNode*& t, const String &key); //普通方法插入节点，但根据特定插入顺序才是平衡树
    BSTNode* findNode(BSTNode* t, const String &key)const; //find函数返回的节点不能调用delete

    void displayTree(BSTNode *t)const; //中序遍历
    void preorderTraversal(BSTNode *t)const; //先序遍历
    void postorderTraversal(BSTNode *t)const; //后序遍历

    BSTNode*& getNode() {
        return tree;
    }

    void insertNode(BSTNode*& t, const String &key);//使用AVL插入算法,保证平衡性
    int insertAVL(BSTNode*& t, const String &key);
    void fixLeftImbalance(BSTNode*& t);
    void rotateLeft(BSTNode*& t);
    void fixRightImbalance(BSTNode*& t);
    void rotateRight(BSTNode*& t);
    
    void removeNode(BSTNode*& t, const String &key);//使用AVL插入算法,保证平衡性
    int removeAVL(BSTNode*& t, const String &key);

private:
    //禁止拷贝

    BSTree(const BSTree& orig) {
    }

    BSTree& operator=(const BSTree &rhs) {
        return *this;
    }
    //    void clearTree(BSTNode *t);//清空数据

    BSTNode* tree; //根节点
};

#endif /* BSTREE_H */

