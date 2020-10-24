#ifndef _RB_TREE_H
#define _RB_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_RBT 1

    typedef int RBTKey;

    enum rbt_color {
        RED = 0, BLACK
    };

    struct rbt_node {
        struct rbt_node* parent;
        struct rbt_node* left;
        struct rbt_node* right;
        RBTKey key;
        enum rbt_color color;
    };

    struct rb_tree {
        struct rbt_node* root;
        struct rbt_node* nil;
    };

    typedef struct rbt_node RBTNode;
    typedef struct rb_tree RBTree;

    /*
     * 常用方法
     */
    RBTree* create_rbtree(void); //使用malloc在堆中创建红黑树并初始化
    RBTree* init_rbtree(RBTree* t); //初始化
    RBTree* insert_rbt_node(RBTree* t, const RBTKey key); //插入key
    RBTree* remove_rbt_node(RBTree* t, const RBTKey key); //删除key对应节点
    void clear_rbtree(RBTree* t); //后序遍历，free树中的节点，对于t需要另行处理
    void display_rbt(RBTree* t); //中序遍历
    void display_rbt_prevorder(RBTree* t); //先序遍历
    void display_rbt_postorder(RBTree* t); //后序遍历
    void show_rbt(RBTree* t); //图示




    /*
     * 辅助函数
     * 参数中RBTree* t, RBTNode* r
     * t->root为根节点, r为待操作的节点
     */
    RBTNode* create_rbt_node(const RBTKey key); //使用malloc在堆中创建节点并初始化

    RBTree* rbt_insert_fixup(RBTree* t, RBTNode* r); //插入后平衡

    RBTree* rbt_left_rotate(RBTree* t, RBTNode* r); //左旋
    RBTree* rbt_right_rotate(RBTree* t, RBTNode* r); //右旋

    RBTree* rbt_transplant(RBTree* t, RBTNode* u, RBTNode* v); //互换节点
    RBTree* rbt_delete_fixup(RBTree* t, RBTNode* r); //删除后平衡
    RBTNode* rbt_minimum(RBTree* t, RBTNode* r); //返回将与待删除节点互换的节点 \
    //这里是用待删除节点的右孩子的最左边一个节点与待删除节点互换，使得他们位置变换， \
    //然后执行删除，这样树的整体结构不变，之后执行平衡策略

    void clear_rbt_node_post(RBTree* t, RBTNode* r);

    void display_rbt_node(RBTree* t, RBTNode* r);
    void display_rbt_node_prev(RBTree* t, RBTNode* r);
    void display_rbt_node_post(RBTree* t, RBTNode* r);
    void show_rbt_node(RBTree* t, RBTNode* r, int high, int n); //



#ifdef __cplusplus
}
#endif

#endif /* RB_TREE_H */

