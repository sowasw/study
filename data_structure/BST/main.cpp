#include <cstdlib>
#include "String.h"
#include "BSTree.h"
#include "AVLTree.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    srand(time(0));

	//创建包含七个小矮人名字的二叉树，普通方法插入
	cout << "创建包含七个小矮人名字的二叉树，普通方法插入:" << endl;
    BSTree tr;
    String s2("Grumpy");//自定义String类型
    tr.insertNodeOld(tr.getNode(), s2);
    tr.insertNodeOld(tr.getNode(), String("Sleepy"));
    tr.insertNodeOld(tr.getNode(), String("Doc"));
    tr.insertNodeOld(tr.getNode(), String("Bashful"));
    tr.insertNodeOld(tr.getNode(), "Dopey");
    tr.insertNodeOld(tr.getNode(), "Happy");
    tr.insertNodeOld(tr.getNode(), "Sneezy");

    tr.insertNodeOld(tr.getNode(), String("Bashful"));
    tr.insertNodeOld(tr.getNode(), String("Doc"));
    tr.insertNodeOld(tr.getNode(), String("Dopey"));
    tr.insertNodeOld(tr.getNode(), s2);
    tr.insertNodeOld(tr.getNode(), String("Happy"));
    tr.insertNodeOld(tr.getNode(), String("Sleepy"));
    tr.insertNodeOld(tr.getNode(), String("Sneezy"));

	cout << "中序遍历:" << endl;
    tr.displayTree(tr.getNode());//中序遍历
    cout << endl;
    cout << "先序遍历:" << endl;
    tr.preorderTraversal(tr.getNode());//先序遍历
    cout << endl;
    cout << "后序遍历:" << endl;
    tr.postorderTraversal(tr.getNode());//后序遍历
    
    cout << "----验证find函数:" << endl;
    BSTNode* bstn = tr.findNode(tr.getNode(), "Grumpy");
    cout << bstn->key << endl;
    cout << bstn->left->key << endl;
    cout << bstn->right->key << endl;
    cout << bstn->left->left->key << endl;
    cout << bstn->left->right->key << endl;
    cout << bstn->right->left->key << endl;
    cout << bstn->right->right->key << endl;
    cout << "tr.getNode()->key:" << tr.getNode()->key << endl;
    //    delete bstn;
    cout << "tr.getNode()->key:" << tr.getNode()->key << endl;


	//创建包含七个小矮人名字的二叉树，AVL旋转法插入
    cout << "--------------------------AVL旋转法插入:" << endl;
    BSTree btr;

    //以下3种插入顺序得到相同结果

    //    btr.insertNode(btr.getNode(), s2);
    //    btr.insertNode(btr.getNode(), String("Sleepy"));
    //    btr.insertNode(btr.getNode(), String("Doc"));
    //    btr.insertNode(btr.getNode(), String("Bashful"));
    //    btr.insertNode(btr.getNode(), "Dopey");
    //    btr.insertNode(btr.getNode(), "Happy");
    //    btr.insertNode(btr.getNode(), "Sneezy");

    btr.insertNode(btr.getNode(), String("Bashful"));
    btr.insertNode(btr.getNode(), String("Doc"));
    btr.insertNode(btr.getNode(), String("Dopey"));
    btr.insertNode(btr.getNode(), s2);
    btr.insertNode(btr.getNode(), String("Happy"));
    btr.insertNode(btr.getNode(), String("Sleepy"));
    btr.insertNode(btr.getNode(), String("Sneezy"));

    btr.insertNode(btr.getNode(), String("Doc"));
    btr.insertNode(btr.getNode(), String("Happy"));
    btr.insertNode(btr.getNode(), s2);
    btr.insertNode(btr.getNode(), String("Sleepy"));
    btr.insertNode(btr.getNode(), String("Sneezy"));
    btr.insertNode(btr.getNode(), String("Bashful"));
    btr.insertNode(btr.getNode(), String("Dopey"));

   	cout << "中序遍历:" << endl;
    tr.displayTree(tr.getNode());//中序遍历
    cout << endl;
    cout << "先序遍历:" << endl;
    tr.preorderTraversal(tr.getNode());//先序遍历
    cout << endl;
    cout << "后序遍历:" << endl;
    tr.postorderTraversal(tr.getNode());//后序遍历
    cout << "----验证find函数:" << endl;
    BSTNode* bstn1 = btr.findNode(btr.getNode(), "Grumpy");
    cout << bstn1->key << endl;
    cout << bstn1->left->key << endl;
    cout << bstn1->right->key << endl;
    cout << bstn1->left->left->key << endl;
    cout << bstn1->left->right->key << endl;
    cout << bstn1->right->left->key << endl;
    cout << bstn1->right->right->key << endl;

    cout << endl;
    //    btr.removeNode(btr.getNode(), "Happy");
    //    btr.displayTree(btr.getNode());
    cout << endl;

    btr.clear();
    tr.clear();



	/**********************通用AVL树*************/
	cout << "\n----验证通用AVL树:" << endl;
    BSTnode* dwarfTree = NULL;
    //insertNode(dwarfTree, "Grumpy");
    //insertNode(dwarfTree, "Sleepy");
    //insertNode(dwarfTree, "Doc");
    //insertNode(dwarfTree, "Bashful");
    //insertNode(dwarfTree, "Dopey");
    //insertNode(dwarfTree, "Happy");

    //    insertNode(dwarfTree, "Sneezy");
    //
    ////    insertNode(dwarfTree, "Grumpy");
    ////    insertNode(dwarfTree, "Sleepy");
    ////    insertNode(dwarfTree, "Doc");
    ////    insertNode(dwarfTree, "Bashful");
    //////    insertNode(dwarfTree, "Dopey");
    //////    insertNode(dwarfTree, "Happy");
    //
    //    insertNode(dwarfTree, "Happy");
    //    insertNode(dwarfTree, "Bashful");
    //    insertNode(dwarfTree, "Dopey");
    //    insertNode(dwarfTree, "Grumpy");
    //    insertNode(dwarfTree, "Sleepy");
    //    insertNode(dwarfTree, "Doc");
    //
    //    displayTree(dwarfTree);
    //    display(dwarfTree);
    //    std::cout << "++++++++++++++++++++++" << std::endl;

    //
    //    removeNode(dwarfTree, "Grumpy");
    //    removeNode(dwarfTree, "Sleepy");
    //    removeNode(dwarfTree, "Sneezy");
    ////    removeNode(dwarfTree, "Grumpy");
    //    displayTree(dwarfTree);
    //    display(dwarfTree);


    insertNode(dwarfTree, 2);
    insertNode(dwarfTree, 1);
    //    insertNode(dwarfTree, 2);
    insertNode(dwarfTree, 4);
    insertNode(dwarfTree, 7);
    insertNode(dwarfTree, 3);
    insertNode(dwarfTree, 0);
    insertNode(dwarfTree, 5);
    insertNode(dwarfTree, 9);
    insertNode(dwarfTree, 8);
    insertNode(dwarfTree, 6);
    insertNode(dwarfTree, 10);
    insertNode(dwarfTree, 11);
    insertNode(dwarfTree, 12);
    insertNode(dwarfTree, 13);
    insertNode(dwarfTree, 14);
    insertNode(dwarfTree, 15);
    //    for (int i = 0; i < 10; i++)
    //        insertNode(dwarfTree, 16 + i);
    displayTree(dwarfTree);
    display(dwarfTree);
    std::cout << "++++++++++++++++++++++" << std::endl;
    removeNode(dwarfTree, 5);
    removeNode(dwarfTree, 11);
    removeNode(dwarfTree, 2);
    //    removeNode(dwarfTree, 18);
    //    removeNode(dwarfTree, 17);
    //    removeNode(dwarfTree, 8);
    displayTree(dwarfTree);
    display(dwarfTree);

    clearTree(dwarfTree);


	cout << "\n新建一颗AVL树随机顺序插入25个数1-25:" << endl;
    BSTnode *dwarfTree1 = NULL;
    for (int i = 0; i < 25; i++) {
        insertNode(dwarfTree1, rand() % 25 + 1);
        insertNode(dwarfTree1, 1 + i);
//        putNode(dwarfTree1, rand() % 25 + 1);
//                putNode(dwarfTree1, 1 + i);
    }
    //    displayTree(dwarfTree1);
    //    display(dwarfTree1);

	cout << "---删除树中元素12-24:" << endl;
    for (int i = 12; i < 25; i++) {
        //removeNode_(dwarfTree1, rand() % 25 + 1);
        removeNode_(dwarfTree1, i);//正确的删除元素的函数
        //removeNode(dwarfTree1, i);
    }
    //        removeNode(dwarfTree1, 12);
    //        removeNode(dwarfTree1, 13);
    //        removeNode(dwarfTree1, 14);
    //        removeNode(dwarfTree1, 15);
    displayTree(dwarfTree1);
    display(dwarfTree1);

    return 0;
}

