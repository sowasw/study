
#ifndef _AVLTREE_H
#define _AVLTREE_H

#define DEBUG_AVLTREE 1

//typedef std::string BSTKEY; //自定义KEY类型
typedef int BSTKEY;

static const int BST_LEFT_HEAVY = -1;
static const int BST_IN_BALANCE = 0;
static const int BST_RIGHT_HEAVY = +1;

struct BSTnode {
    BSTKEY key;
    BSTnode *left, *right;
    int bf;
};


void displayTree(BSTnode *t, const std::string & prefix = "");
void display(BSTnode *t);


void insertNode(BSTnode * & t, const BSTKEY & key);//OK

int removeNode(BSTnode * & t, const BSTKEY & key);//error


/*
 *
 * OK
 */
//删除元素应使用此函数
bool removeNode_(BSTnode * & t, const BSTKEY & key);

bool removeTargetNode(BSTnode * & t);
void updateBF(BSTnode * & t, int bfDelta);
int compareKeys(const BSTKEY & k1, const BSTKEY & k2);

void putNode(BSTnode * & t, const BSTKEY & key); 
void addNode(BSTnode * & t, const BSTKEY & key, bool & heightFlag);

void clearTree(BSTnode * & t);

#endif
