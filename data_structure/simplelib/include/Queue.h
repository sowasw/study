
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <iostream>
#include <cstdlib>


//用数组实现
template<typename ValueType>
class Queue {
public:
    Queue();
    ~Queue();

    int size()const;
    bool isEmpty()const;
    void clear();
    void enqueue(ValueType value);//入队
    ValueType dequeue();//弹出队首元素
    ValueType peek()const;//返回队首元素

    //深复制
    Queue(const Queue<ValueType>& src);
    Queue<ValueType>& operator=(const Queue<ValueType>& src);
private:
    static const int INITIAL_CAPACITY = 10;
    void expandCapacity();//扩容

    ValueType *array;
    int capacity;
    int head;
    int tail;
};

template<typename ValueType>
Queue<ValueType>::Queue() {
    capacity = INITIAL_CAPACITY;
    array = new ValueType[capacity];
    head = 0;
    tail = 0;
}

template<typename ValueType>
Queue<ValueType>::~Queue() {
    delete[]array;
}

template<typename ValueType>
int Queue<ValueType>::size() const {
    return (tail + capacity - head) % capacity;
}

template<typename ValueType>
bool Queue<ValueType>::isEmpty() const {
    return head == tail;
}

template<typename ValueType>
void Queue<ValueType>::clear() {
    head = tail = 0;
}

template<typename ValueType>
void Queue<ValueType>::enqueue(ValueType value) {
    if (size() == capacity - 1)expandCapacity();
    array[tail] = value;
    tail = (tail + 1) % capacity;
}

template<typename ValueType>
ValueType Queue<ValueType>::dequeue() {
    if (isEmpty()) {
        std::cout << "dequeue错误：Queue为空，程序退出！\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    ValueType result = array[head];
    head = (head + 1) % capacity;
    return result;
}

template<typename ValueType>
ValueType Queue<ValueType>::peek() const {
    if (isEmpty()) {
        std::cout << "peek错误：Queue为空，程序退出！\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    return array[head];
}

template<typename ValueType>
Queue<ValueType>::Queue(const Queue<ValueType>& src) {
    int count = src.size();
    capacity = count + INITIAL_CAPACITY;
    array = new ValueType[capacity];
    for (int i = 0; i < count; i++)
        array[i] = src.array[(src.head + i) % src.capacity];
    head = 0;
    tail = count;
}

template<typename ValueType>
Queue<ValueType>& Queue<ValueType>::operator=(const Queue<ValueType>& src) {
    if (this != &src) {
        delete[] array;

        int count = src.size();//deepcopy
        capacity = count + INITIAL_CAPACITY;
        array = new ValueType[capacity];
        for (int i = 0; i < count; i++)
            array[i] = src.array[(src.head + i) % src.capacity];
        head = 0;
        tail = count;
    }
    
    return *this;
}

template<typename ValueType>
void Queue<ValueType>::expandCapacity() {
    std::cout << "扩容中..." << std::endl;
    
    int count = size();//deepcopy
    ValueType* oldarray = array;
    array = new ValueType[2 * capacity];
    for (int i = 0; i < count; i++)
        array[i] = oldarray[(head + i) % capacity];
    capacity *= 2;
    head = 0;
    tail = count;
    delete[] oldarray;
}


#endif /* QUEQUE_H */

