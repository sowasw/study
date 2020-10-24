#include <stdio.h>
#include <stdlib.h>
#include "linux_list.h"

struct fox{
	int tail_length;
	int weight;
	struct list_head list;
};

// 通过链表节点打印数据
void print_fox(struct list_head *head) {
	struct fox *fptr;
	fptr = list_entry(head, struct fox, list);//使用宏获取父结构地址
	printf("tail_length: %d, weight: %d\n", fptr->tail_length, fptr->weight);
}

int main (int argc, char** argv){
	struct list_head lh;
	lh.prev = NULL;
	lh.next = NULL;

	struct fox red_fox = {
		.tail_length = 40,
		.weight = 6,
		.list = lh,
	};
	
	//两个结构的迭代器
	struct fox *f;
	struct list_head *p;
	
	p = &(red_fox.list);
	f = list_entry(p, struct fox, list);
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);

/*	container_of(p, struct fox, list);*/
/*	//typeof(&lh) l = lh;//error*/
/*	typeof(&lh) l = &lh;*/
/*	typeof(&lh) l1 = p;*/
	
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
	
	printf("offsetof(struct fox, list): %ld\n", offsetof(struct fox, list));
	printf("&lh: %p\n", &lh);
	printf("f: %p\n", f);
	printf("&red_fox.list: %p\n", &red_fox.list);
	
	//添加一个节点
	struct list_head lh1;
	lh1.prev = &lh;
	lh1.next = NULL;
	lh.next = &lh1;

	struct fox blue_fox = {
		.tail_length = 38,
		.weight = 5,
		.list = lh1,
	};
	
	p = &(blue_fox.list);
	f = list_entry(p, struct fox, list);
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
	
	
	//遍历
	//struct list_head *head = &lh;//error
	struct list_head *head = &red_fox.list;//头节点
	p = head;
	while(p != NULL){
		f = list_entry(p, struct fox, list);
		printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
		p = p->next;
	}
	
	////////////
	//1
	//动态分配的链表
	struct list_head *tail = NULL;//尾节点
	head = NULL;
	for(int i = 0; i < 10;i ++){
		struct fox *newfox = malloc(sizeof(struct fox));
		newfox->tail_length = i + 1;
		newfox->weight = 50 + i + 1;
		newfox->list.prev = NULL;
		newfox->list.next = NULL;
		
		if(i == 0){
			head = &newfox->list;
			tail = head;
		} else {
			newfox->list.prev = tail;
			tail->next = &newfox->list;
			tail = &newfox->list;
		}
	}
	
	//正向遍历
	p = head;
	while(p != NULL){
		f = list_entry(p, struct fox, list);
		printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
		p = p->next;
	}
	
	//反向遍历
	p = tail;
	while(p != NULL){
		f = list_entry(p, struct fox, list);
		printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
		p = p->prev;
	}
	
	//此时f在链表首节点
	//f赋予下一个节点
	f = list_entry(f->list.next, typeof(*f), list);
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
	//f再后移
	f = list_next_entry(f, list);
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
	//f向前移
	f = list_prev_entry(f, list);
	printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);
	
	////////////
	//2
	//环形链表
	printf("\n----环形链表-----\n");
	
	//初始化链表
	struct list_head tmp;
	head = &tmp;
	INIT_LIST_HEAD(head);
	//注意头节点不包含数据，
	//如果头节点包含了数据则不能用foreach宏遍历，开始的那个节点遍历不到
	
	//插入
	for(int i = 0; i < 10;i ++){
		struct fox *newfox = malloc(sizeof(struct fox));
		newfox->tail_length = i + 1;
		newfox->weight = 50 + i + 1;
		
		list_add(&newfox->list, head->prev);//在头节点与最后一个元素之间插入
		//list_add(&newfox->list, head);//在头节点与第一个元素之间插入
	}
	
	//遍历，用函数指针打印
	printf("\n ----环形链表list_all-----\n");
	list_all(head, print_fox);//用函数指针遍历打印
	
	//通过指针遍历
	p = head->next;
	printf("\n main----环形链表遍历-----\n");
	while(p != head){
/*		f = list_entry(p, struct fox, list);*/
/*		printf("tail_length: %d, weight: %d\n", f->tail_length, f->weight);*/
		
		print_fox(p);
		p = p->next;
	}
	
	//通过foreach宏遍历，向后遍历
	printf("\n ----环形链表foreach-----\n");
	list_for_each(p, head){
		print_fox(p);
	}
	
	//通过foreach宏向前遍历
	printf("\n ----环形链表foreach_prev-----\n");
		list_for_each_prev(p, head){
		print_fox(p);
	}
	
	//删除一个节点后遍历
	printf("\n ----环形链表删除后foreach  -----\n");
	p = head->next;//删除第一个节点
	list_del(p);
	f = list_entry(p, struct fox, list);
	free(f);
	
	list_for_each(p, head){
		print_fox(p);
	}
	
	exit(0);
}

