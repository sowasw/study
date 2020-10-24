#ifndef _QUEUEL_H_
#define _QUEUEL_H_

#include <iostream>
#include <cstdlib>


//用链表实现

template<typename ValueType>
class QueueL {
public:
    QueueL();
    ~QueueL();

    int size()const;
    bool isEmpty()const;
    void clear();
    void enqueue(ValueType value); //入队
    ValueType dequeue(); //弹出队首元素
    ValueType peek()const; //返回队首元素

    //深复制
    QueueL(const QueueL<ValueType>& src);
    QueueL<ValueType>& operator=(const QueueL<ValueType>& src);

private:
    void deepcopy(const QueueL<ValueType>& src); //深复制

    struct Cell {
        ValueType data;
        Cell* link;
    };

    Cell* head;
    Cell* tail;
    int count;



public:
    //迭代器
    //重载++，==，!=,*,->运算符

    class iterator {
    public:

        iterator() {
            qp = NULL;
            cp = NULL;
            //            index = 0;
        }

        ~iterator() {
            qp = NULL;
            cp = NULL;
            //            index = 0;
        }

        ValueType& operator*() {
            if (qp == NULL)error("Iterator unintialized!");
            if (cp == NULL)error("Iterator out of range OR QueueL unintialized!");
            return cp->data;
        }

        Cell* operator->() {//能通过->操作符改变Cell*指向的内容
            if (qp == NULL)error("Iterator unintialized!");
            if (cp == NULL)error("Iterator out of range OR QueueL unintialized!");
            return cp;
        }

        //        ValueType operator[](int k) {
        //            if (cp == NULL)error("Iterator unintialized!");
        //            if (qp == NULL)error("Iterator unintialized!");
        //            if (index + k < 0 || index + k >= qp->count)error("Iterator out of range!");
        //            return *(qp->head + index + k);
        //        }

        //逻辑运算

        bool operator==(const iterator &rhs) {
            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
            return qp == rhs.qp && cp == rhs.cp;
        }

        bool operator!=(const iterator &rhs) {
            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
            return !(*this == rhs);

        }

        //        bool operator>(const iterator &rhs) {
        //            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
        //            return index > rhs.index;
        //        }
        //
        //        bool operator>=(const iterator &rhs) {
        //            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
        //            return index >= rhs.index;
        //        }
        //
        //        bool operator<(const iterator &rhs) {
        //            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
        //            return index < rhs.index;
        //        }
        //
        //        bool operator<=(const iterator &rhs) {
        //            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
        //            return index <= rhs.index;
        //        }

        //前缀后缀迭代

        iterator & operator++() {
            if (qp == NULL)error("Iterator unintialized!");
            if (cp == NULL)error("Iterator out of range!");
            cp = cp->link;
            return *this;
        }

        //        iterator & operator--() {
        //            if (qp == NULL)error("Iterator unintialized!");
        //            index--;
        //            return *this;
        //        }

        iterator operator++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        //        iterator operator--(int) {
        //            iterator copy(*this);
        //            operator--();
        //            return copy;
        //        }

        //算数运算

        //        iterator operator+(int k) {
        //            if (qp == NULL)error("Iterator unintialized!");
        //            return iterator(qp, index + k);
        //        }
        //
        //        iterator operator-(int k) {
        //            if (qp == NULL)error("Iterator unintialized!");
        //            return iterator(qp, index - k);
        //        }
        //
        //        int operator-(const iterator&rhs) {
        //            if (qp == NULL)error("Iterator unintialized!");
        //            if (qp != rhs.qp)error("Iterators are in dfferent QueueLs!");
        //            return index - rhs.index;
        //        }

    private:
        const QueueL* qp;
        Cell* cp;
        //        int index;//不支持下标操作，强行实现每次取下标都是O（N）复杂度

        iterator(const QueueL* qlp1, Cell* cp1) {
            qp = qlp1;
            cp = cp1;
        }

        void error(char* msg) {
            std::cerr << msg << std::endl;
            exit(EXIT_FAILURE);
        }
        friend class QueueL;
    };

    iterator begin()const {
        return iterator(this, this->head);
    }

    iterator end()const {
        if (this->tail == NULL)
            return iterator(this, this->tail);
        return iterator(this, this->tail->link);
    }
};

template<typename ValueType>
QueueL<ValueType>::QueueL() {
    head = tail = NULL;
    count = 0;
}

template<typename ValueType>
QueueL<ValueType>::~QueueL() {
    clear();
}

template<typename ValueType>
int QueueL<ValueType>::size() const {
    return count;
}

template<typename ValueType>
bool QueueL<ValueType>::isEmpty()const {
    return count == 0;
}

template<typename ValueType>
void QueueL<ValueType>::clear() {
    while (count > 0) {
        dequeue();
    }
}

template<typename ValueType>
void QueueL<ValueType>::enqueue(ValueType value) {
    Cell* cp = new Cell;
    cp->data = value;
    cp->link = NULL;
    if (head == NULL)
        head = cp;
    else
        tail->link = cp;
    tail = cp;
    count++;
}

template<typename ValueType>
ValueType QueueL<ValueType>::dequeue() {
    if (isEmpty()) {
        std::cerr << "dequeue错误：QueueL为空，程序退出！\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    ValueType result = head->data;
    Cell* cp = head;
    head = cp->link;
    delete cp;
    if (head == NULL)
        tail = NULL;
    count--;
    return result;
}

template<typename ValueType>
ValueType QueueL<ValueType>::peek()const {
    if (isEmpty()) {
        std::cerr << "peek错误：QueueL为空，程序退出！\n" << std::endl;
        exit(EXIT_FAILURE);
    }
    return head->data;
}

template<typename ValueType>
QueueL<ValueType>::QueueL(const QueueL<ValueType>& src) {
    deepcopy(src);
}

template<typename ValueType>
QueueL<ValueType>& QueueL<ValueType>::operator=(const QueueL<ValueType>& src) {
    if (this != &src) {
        clear();
        deepcopy(src);
    }
    return *this;
}

template<typename ValueType>
void QueueL<ValueType>::deepcopy(const QueueL<ValueType>& src) {
    head = tail = NULL;
    count = 0;
    for (Cell* cp = src.head; cp != NULL; cp = cp->link)
        enqueue(cp->data);
}

#endif /* QUEUEL_H */

