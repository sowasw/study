#include "rb_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RBTree* create_rbtree(void) {
    RBTree* t = (RBTree*) malloc(sizeof (RBTree));
    /*
        t->nil = create_rbt_node(0);
        t->nil->color = BLACK;
        t->root = t->nil;
        return t;
     */
    init_rbtree(t);
    return t;
}

RBTree* init_rbtree(RBTree* t) {
    t->nil = create_rbt_node(0);
    t->nil->color = BLACK;
    t->root = t->nil;
    return t;
}

RBTree* insert_rbt_node(RBTree* t, const RBTKey key) {
    RBTNode* rp = t->root; //插入路径上的迭代器//算法导论书上记作x
    RBTNode* parent = t->nil; //迭代器的父节点//y
    /*
        RBTNode* r = create_rbt_node(key); //待插入的节点//z
     */
    while (rp != t->nil) {
        parent = rp;
        if (key < rp->key)
            rp = rp->left;
        else if (key > rp->key)
            rp = rp->right;
        else
            return t; //树中已有key，返回
    }

    RBTNode* r = create_rbt_node(key); //待插入的节点//z
    r->parent = parent;
    r->left = t->nil;
    r->right = t->nil;

    if (parent == t->nil)
        t->root = r;
    else if (key < parent->key)
        parent->left = r;
    else
        parent->right = r;

    t = rbt_insert_fixup(t, r);
    return t;
}

RBTree* remove_rbt_node(RBTree* t, const RBTKey key) {
    RBTNode* rp = t->root; //删除路径上的迭代器//y
    while (rp != t->nil) {
        if (key < rp->key)
            rp = rp->left;
        else if (key > rp->key)
            rp = rp->right;
        else
            break; //找到key
    }
    if (rp == t->nil)
        return t; //未找到

    RBTNode* z = rp; //z 待删除key的节点
    enum rbt_color y_o_c = rp->color; //记录颜色
    RBTNode* x; //记录z的孩子节点

    if (z->left == t->nil) {
        x = z->right;
        rbt_transplant(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        rbt_transplant(t, z, z->left);
    } else {
        rp = rbt_minimum(t, z->right);

        /*
                //以下4行为tree_minimum方法
                rp = z->right;
                while (rp->left != t->nil) {
                    rp = rp->left;
                }
         */

        y_o_c = rp->color;
        x = rp->right;
        if (rp->parent == z)
            x->parent = rp;
        else {
            rbt_transplant(t, rp, rp->right);
            rp->right = z->right;
            rp->right->parent = rp;
        }
        rbt_transplant(t, z, rp);
        rp->left = z->left;
        rp->left->parent = rp;
        rp->color = z->color;
    }

#ifdef DEBUG_RBT
    printf("删除节点 %d\n", z->key);
    memset(z, 0, sizeof(RBTNode));//
    printf("memset后: %d\n", z->key);//
#endif
    free(z);

    if (y_o_c == BLACK)
        rbt_delete_fixup(t, x);
}

void clear_rbtree(RBTree* t) {
    if (t == NULL)return;
    clear_rbt_node_post(t, t->root);
    if (t->nil != NULL)
        free(t->nil);
    
    //free(t);//这里注释掉，在外部释放t的内存
}

void display_rbt(RBTree* t) {
    if (t == NULL)return;
    display_rbt_node(t, t->root);
    printf("\n");
}

void display_rbt_prevorder(RBTree* t) {
    if (t == NULL)return;
    display_rbt_node_prev(t, t->root);
    printf("\n");
}

void display_rbt_postorder(RBTree* t) {
    if (t == NULL)return;
    display_rbt_node_post(t, t->root);
    printf("\n");
}

void show_rbt(RBTree* t) {
    if (t == NULL)return;
    int h = 0;
    int n = 0;
    show_rbt_node(t, t->root, h, n);
}

RBTNode* create_rbt_node(const RBTKey key) {
    RBTNode* r = (RBTNode*) malloc(sizeof (RBTNode));
    r->left = r->right = r->parent = NULL;
    r->color = RED;
    r->key = key;
    return r;
}

/*
 * 参数中RBTree* t, RBTNode* r
 * t->root为根节点, r为待操作的节点
 */
RBTree* rbt_insert_fixup(RBTree* t, RBTNode* r) {
    while (r->parent->color == RED) {
        if (r->parent == r->parent->parent->left) {
            RBTNode* u = r->parent->parent->right; //r的叔叔uncle;//y
            if (u->color == RED) {//case 1
                r->parent->color = BLACK;
                u->color = BLACK;
                r->parent->parent->color = RED;
                r = r->parent->parent;
            } else if (r == r->parent->right) {//case 2
                r = r->parent;
                rbt_left_rotate(t, r);
            } else {//case 3//第三版书上这里没有else，加上
                r->parent->color = BLACK;
                r->parent->parent->color = RED;
                rbt_right_rotate(t, r->parent->parent);
            }
        } else {
            RBTNode* u = r->parent->parent->left; //r的叔叔uncle;//y
            if (u->color == RED) {
                r->parent->color = BLACK;
                u->color = BLACK;
                r->parent->parent->color = RED;
                r = r->parent->parent;
            } else if (r == r->parent->left) {
                r = r->parent;
                rbt_right_rotate(t, r);
            } else {//
                r->parent->color = BLACK;
                r->parent->parent->color = RED;
                rbt_left_rotate(t, r->parent->parent);
            }
        }
    }//while

    t->root->color = BLACK;
    return t;
}

RBTree* rbt_left_rotate(RBTree* t, RBTNode* r) {
    if (r->right != t->nil) {
        RBTNode* child = r->right; //y
#ifdef DEBUG_RBT
        printf("left_rolate %d<->%d\n", r->key, child->key);
#endif
        r->right = child->left;
        if (child->left != t->nil)
            child->left->parent = r;
        child->parent = r->parent;
        if (r->parent == t->nil)
            t->root = child;
        else if (r == r->parent->left)
            r->parent->left = child;
        else
            r->parent->right = child;
        child->left = r;
        r->parent = child;
    }

    return t;
}

RBTree* rbt_right_rotate(RBTree* t, RBTNode* r) {
    if (r->left != t->nil) {//书上伪代码之前假设这个条件，
        //根节点的父节点为nil已经在init_rbtree()方法中初始化
        RBTNode* child = r->left; //y
#ifdef DEBUG_RBT
        printf("right_rolate %d<->%d\n", r->key, child->key);
#endif
        r->left = child->right;
        if (child->right != t->nil)
            child->right->parent = r;
        child->parent = r->parent;
        if (r->parent == t->nil)
            t->root = child;
        else if (r == r->parent->right)
            r->parent->right = child;
        else
            r->parent->left = child;
        child->right = r;
        r->parent = child;
    }

    return t;
}

RBTree* rbt_transplant(RBTree* t, RBTNode* u, RBTNode* v) {
    if (u->parent == t->nil)
        t->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
#ifdef DEBUG_RBT
    printf("transplant %d %d\n", u->key, v->key);
#endif
    return t;
}

RBTree* rbt_delete_fixup(RBTree* t, RBTNode* x) {//r=x
    while (x != t->root && x->color == BLACK) {
        RBTNode* w; //记录x的兄弟节点
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {//case 1
                w->color = BLACK;
                x->parent->color = RED;
                rbt_left_rotate(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
            												//case 2
                w->color = RED;
                x = x->parent;
            } else if (w->right->color == BLACK) {//case 3
                w->left->color = BLACK;
                w->color = RED;
                rbt_right_rotate(t, w);
                w = x->parent->right;
            } else {//else一定要加上//case 4
                w->color = x->parent->color; //case 4
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rbt_left_rotate(t, x->parent);
                x = t->root;
            }
        } else {//镜像
            w = x->parent->left;
            if (w->color == RED) {//case 1
                w->color = BLACK;
                x->parent->color = RED;
                rbt_right_rotate(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
            												//case 2
                w->color = RED;
                x = x->parent;
            } else if (w->left->color == BLACK) {//case 3
                w->right->color = BLACK;
                w->color = RED;
                rbt_left_rotate(t, w);
                w = x->parent->left;
            } else {
                w->color = x->parent->color; //case 4
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rbt_right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }//while

    x->color = BLACK;
}

RBTNode* rbt_minimum(RBTree* t, RBTNode* r) {
    while (r->left != t->nil) {
        r = r->left;
    }
    return r;
}

void clear_rbt_node_post(RBTree* t, RBTNode* r) {
    if (r != t->nil) {
        clear_rbt_node_post(t, r->left);
        clear_rbt_node_post(t, r->right);
        if (r != NULL){
#ifdef DEBUG_RBT
        	printf("before free: %d, ", r->key);//
        	memset(r, 0, sizeof(RBTNode));//
        	printf("after memset: %d\n", r->key);//
#endif
            free(r);
        }
    }
}

void display_rbt_node(RBTree* t, RBTNode* r) {
    if (r != t->nil) {
        display_rbt_node(t, r->left);
        /*
                printf("%d\n", r->key);
         */
        printf("%d ", r->key);
        display_rbt_node(t, r->right);
    }
}

void display_rbt_node_prev(RBTree* t, RBTNode* r) {
    if (r != t->nil) {
        /*
                printf("%d\n", r->key);
         */
        printf("%d ", r->key);
        display_rbt_node_prev(t, r->left);
        display_rbt_node_prev(t, r->right);
    }
}

void display_rbt_node_post(RBTree* t, RBTNode* r) {
    if (r != t->nil) {
        display_rbt_node_post(t, r->left);
        display_rbt_node_post(t, r->right);
        /*
                printf("%d\n", r->key);
         */
        printf("%d ", r->key);
    }
}

void show_rbt_node(RBTree* t, RBTNode* r, int high, int n) {
    if (r != t->nil) {
        show_rbt_node(t, r->right, high + 7, n + 1);
        int i = 0;
        while (i != high) {
            if (i % 7 == 0) {
                if (i >= 7 * n)
                    printf("|");
                else
                    printf(" ");
            } else
                printf(" ");
            i++;
        }
        if (r->color == RED)
            printf("|-<R>");
        else printf("|-<B>");
        printf("%2d-\n", r->key);

        show_rbt_node(t, r->left, high + 7, n + 1);
    } else {
        int i = 0;
        while (i != high) {
            if (i % 7 == 0) {
                if (i >= 7 * n)
                    printf("|");
                else
                    printf(" ");
            } else
                printf(" ");
            i++;
        }
        printf("|-NIL\n");
    }
}
