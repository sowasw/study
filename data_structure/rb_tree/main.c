#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rb_tree.h"

/*
 * 
 */
#define N_NODES 20//插入红黑树的次数，插入数值在0～N_NODES之间随机，\
					 //可能重复插入
#define ARRAYSIZE 20//用于保存插入的数字的数组大小，应该与N_NODES相等

void display_array(int array[], int size);//打印数组

int main(int argc, char** argv) {
    int i, n = 0;//统计插入元素的个数
    srand(time(0));
    int array[ARRAYSIZE];//保存插入的数字
    printf("sizeof(array): %ld\n", sizeof(array));
    printf("sizeof(RBTNode): %ld\n", sizeof(RBTNode));
    //memset(array, -1, sizeof (array));
    memset(array, -1, ARRAYSIZE * sizeof(int));//初始化数组为-1
    //memset只能初始化为0或-1，因为每字节只取第二个参数的后八位
    
    display_array(array, ARRAYSIZE);

    RBTree* t = create_rbtree();
    
    //随机插入
    for (i = 0; i < N_NODES; i++) {
        int num = rand() % N_NODES;
        if (num != array[num]) {
            printf("\n插入： %d (main)\n", num);
            array[num] = num;//把插入过的元素保存到数组中
            n++;
        } else {
            printf("重复插入 %d (main)\n", num);
        }
        
        insert_rbt_node(t, num);

        /*
        		//填满数组，并插入到红黑树，操作完之后总数为N_NODES
                num = i;
                insert_rbt_node(t, num);
         */
    }
    
    //打印
    printf("array: \n");
    display_array(array, ARRAYSIZE);
    printf("\n实际插入了多少节点: %d\n", n);
    printf("中序遍历display_rbt(t): \n");
    display_rbt(t);
    printf("先序遍历display_rbt_prevorder(t): \n");
    display_rbt_prevorder(t);
    printf("后序遍历display_rbt_postorder(t): \n");
    display_rbt_postorder(t);
    printf("视图show_rbt(t): \n");
    show_rbt(t);
     
	//随机删除
    for (i = 0; i < N_NODES; i++) {
        int num = rand() % N_NODES;
        if (num == array[num]) {
            printf("\n删除： %d (main)\n", num);
            array[num] = -1;//把树中删除的元素也从数组中移除
            n--;
        } else {
            printf("重复删除或不存在 %d (main)\n", num);
        }
        
        remove_rbt_node(t, num);

        /*
                num = i;
                remove_rbt_node(t, num);
         */
    }
    
    //打印
    printf("array: \n");
    display_array(array, ARRAYSIZE);
    printf("\n删除后还剩多少节点: %d\n", n);
    printf("中序遍历display_rbt(t): \n");
    display_rbt(t);
    printf("先序遍历display_rbt_prevorder(t): \n");
    display_rbt_prevorder(t);
    printf("后序遍历display_rbt_postorder(t): \n");
    display_rbt_postorder(t);
    printf("视图show_rbt(t): \n");
    show_rbt(t);

	//释放内存
	printf("释放内存clear_rbtree(t): \n");
    clear_rbtree(t);
    //printf("t->root->key: %d\n", t->root->key);
    free(t);
    //printf("t->root->key: %d\n", t->root->key);

    return (EXIT_SUCCESS);
}

void display_array(int array[], int size){
	for(int i = 0; i < size; i++)
		printf("%d ", array[i]);
}


