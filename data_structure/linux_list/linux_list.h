#ifndef _linux_list_h_
#define _linux_list_h_

/*
 * /usr/src/linux-headers-4.15.0-65/include/linux/list.h
 */

struct list_head {
	struct list_head* prev;
	struct list_head* next;
};

//初始化链表
void INIT_LIST_HEAD(struct list_head *list) {
	list->next = list;
	list->prev = list;
}

//添加节点
void list_add(struct list_head *new, struct list_head *head) {
	new->next = head->next;
	new->prev = head;
	head->next->prev = new;
	head->next = new;
}

//删除节点
void list_del(struct list_head *entry) {
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
}

//判断链表是否为空
int list_empty(const struct list_head *head) {
	return head->next == head;
}

//遍历链表，对每个节点调用回调函数
void list_all(struct list_head *head, 
			   void (*list_callback)(struct list_head *)) {
	
	if(list_empty(head))
		return;
	struct list_head *lptr = head->next;
	while(lptr != head) {
		list_callback(lptr);
		lptr = lptr->next;
	}
}

//在内核定义
//返回在type中member的偏移量
#define offsetof(type, member) ((size_t)&((type *)0)->member)

//在内核定义
//通过member取得父结构地址
//第二行保证ptr与((type *)0)->member是同一类型，如不是则编译不通过
#define container_of(ptr, type, member) ({ \
			const typeof( ((type *)0)->member ) *__mptr = (ptr); \
			(type *)( (char *)__mptr - offsetof(type, member) );})
		
//通过链表节点取得数据结构体的地址
#define list_entry(ptr, type, member) \
		container_of(ptr, type, member)

//通过一个数据结构体获得下一个\上一个结构的地址	
#define list_next_entry(pos, member) \
		list_entry((pos)->member.next, typeof(* (pos)), member)
		
#define list_prev_entry(pos, member) \
		list_entry((pos)->member.prev, typeof(* (pos)), member)

//数据结构中的第一个元素，ptr应该为链表头节点		
/**
 * list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

//最后一个元素
/**
 * list_last_entry - get the last element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)


//遍历链表节点，head是一个没有数据的头节点
//向后遍历		
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

//向前遍历
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)
	
/**
 * list_first_entry_or_null - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 *
 * Note that if the list is empty, it returns NULL.
 */
#define list_first_entry_or_null(ptr, type, member) ({ \
	struct list_head *head__ = (ptr); \
	struct list_head *pos__ = READ_ONCE(head__->next); \
	pos__ != head__ ? list_entry(pos__, type, member) : NULL; \
})





#endif
