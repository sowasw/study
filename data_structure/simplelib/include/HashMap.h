#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <string>
#include <sstream>
#include <iostream>

const int HASH_SEED = 5381;
const int HASH_MULTIPLIER = 33;
const int HASH_MASK = unsigned(-1) >> 1;

//通用哈希函数，KeyType类型必须重载<<操作符以转化为字符串,生成哈希码

template <typename KeyType>
int hashCode(const KeyType & key) {
    unsigned hash = HASH_SEED;
    std::string str;
    std::ostringstream os;
    os << key; //KeyType类型必须重载<<操作符以转化为字符串
    str = os.str();
    int n = str.length();
    for (int i = 0; i < n; i++) {
        hash = HASH_MULTIPLIER * hash + str[i];
    }
    return int(hash & HASH_MASK);
}

bool isPrime(const int& n) {//判断n是否为素数
    int m = 2;
    while (m < n) {
        if (n % m == 0)
            return false;
        m++;
    }
    return true;
}

/**HashMap用法
 * HashMap<KeyType,ValueType> hmp;
 * 
 * KeyType类型必须重载<<操作符以转化为字符串,而且必须是能赋值的，能比较的，重载=，==操作符
 * 为了能打印或者调用toString方法，ValueType也应该重载<<操作符
 * 
 */

template <typename KeyType, typename ValueType>
class HashMap {
public:
    HashMap();
    ~HashMap();

    int size()const; //返回键值对数

    int getNBuckets()const {//返回桶数
        return nBuckets;
    }
    bool isEmpty()const; //hashmap为空返回1
    void clear(); //清空数据和桶,回到初始状态，13桶
    void clearData(); //清空数据保留桶

    void put(const KeyType &key, const ValueType &value); //插入元素

    bool containsKey(const KeyType &key)const; //if(containsKey),get();
    ValueType get(const KeyType &key)const; //返回key对应的value

    ValueType operator[](const KeyType &key)const; //下标取key，返回value

    void remove(const KeyType &key); //删除key对应元素

    void expandAndRehash(const int & newNBuckets);
    //设置桶数为newNBuckets之后第一个素数并rehash,newNBuckets需要大于1.4倍旧桶数

    template<typename Func>
    void mapAll(Func fn)const; //遍历整个hashmap并对每组键值对调用函数fn
    void mapAll(void (*fn)(const KeyType& key, const ValueType& value)); //同上

    typedef void (*FP)(KeyType&, ValueType&);
    FP fp; //辅助mapAll，用法：map.mapAll(map.fp=FUNC);

    void display()const; //打印,为了能打印key和value都要重载<<操作符,也可以调用toString方法实现
    std::string toString()const;

    //重载<<操作符不用声明友元函数
    //    friend std::ostream & operator<<(std::ostream & os, const HashMap & src);
    //    friend std::istream & operator>>(std::istream & is, const HashMap & src);

    double getLoadFactor()const {//返回动态负荷系数
        return double(count) / nBuckets;
    }

    double getLoadFactorThreshold()const {//负荷系数阈值
        return REHASH_THRESHOLD;
        //        return double(REHASH_NUM) / REHASH_DEN;
    }


private:

    struct Cell {
        KeyType key;
        ValueType value;
        Cell* link;
    };

    //    static const int REHASH_NUM = 7; 
    //    static const int REHASH_DEN = 10;
    static const double REHASH_THRESHOLD; //负荷系数阈值//0.7,double型不能在这里初始化
    static const int INTIAL_BUCKET_COUNT = 13; //初始桶数
    Cell** buckets; //桶数组
    int nBuckets; //桶数

    int count; //键值对计数

    void deleteAll(); //销毁
    Cell* findCell(const KeyType &key, const int &bucket)const; //查找key对应节点
    void rehash(const bool & customCapacity, const int & newNBuckets); //重哈希


    //禁止拷贝

    HashMap(const HashMap & src) {
    }

    HashMap& operator=(const HashMap & src) {
        return *this;
    }

public:

    /**迭代器
     *用法：
     *HashMap<KeyType,ValueType> hmp;
     * ...;
     *HashMap<KeyType,ValueType>::iterator it, begin = hmp.begin(), end = hmp.end();
     *for(it = begin; it != end; ++it)
     *    ...;
     * 
     */

    class iterator {
    public:

        iterator() {
            hmp = NULL;
            cp = NULL;
            index = 0;
        }

        ~iterator() {
            hmp = NULL;
            cp = NULL;
            index = 0;
        }

        ValueType getValue()const {
            return cp->value;
        }

        KeyType getKey()const {
            return cp->key;
        }

        void setValue(const ValueType & value) {
            cp->value = value;
        }

        ValueType operator*() {
            if (hmp == NULL)error("Iterator unintialized!");
            if (cp == NULL)error("Iterator out of range OR HashMap unintialized!");
            return cp->value;
        }

        Cell* operator->() {//能通过->操作符改变Cell*指向的内容：key,value,link,慎用
            if (hmp == NULL)error("Iterator unintialized!");
            if (cp == NULL)error("Iterator out of range OR HashMap unintialized!");
            return cp;
        }

        bool operator==(const iterator &rhs) {
            if (hmp != rhs.hmp)error("Iterators are in dfferent QueueLs!");
            return hmp == rhs.hmp && cp == rhs.cp && index == rhs.index;
        }

        bool operator!=(const iterator &rhs) {
            if (hmp != rhs.hmp)error("Iterators are in dfferent QueueLs!");
            return !(*this == rhs);

        }

        iterator & operator++() {
            if (hmp == NULL)error("Iterator unintialized!");
            cp = cp->link;
            while (cp == NULL && index < hmp->nBuckets - 1) {
                cp = hmp->buckets[++index];
            }
            return *this;
        }

        iterator operator++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

    private:
        const HashMap* hmp;
        Cell* cp;
        int index;

        iterator(const HashMap* hmp, int bucket) {
            this->hmp = hmp;
            this->cp = NULL;
            this->index = bucket;

            if (index == 0) {
                if (hmp->count > 0) {
                    int n = hmp->nBuckets;
                    for (int i = 0; i < n; i++) {
                        cp = hmp->buckets[i];
                        if (cp != NULL)
                            break;
                        index++;
                    }
                }
            }
        }

        void error(char* msg) {
            std::cerr << msg << std::endl;
            exit(EXIT_FAILURE);
        }
        friend class HashMap;
    };

    iterator begin()const {//首元素迭代器，如果hashmap为空则为NULL
        return iterator(this, 0);
    }

    iterator end()const {//末尾桶中的NULL
        return iterator(this, this->nBuckets - 1);
    }

};

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::HashMap() {
    nBuckets = INTIAL_BUCKET_COUNT;
    buckets = new Cell*[nBuckets];
    for (int i = 0; i < nBuckets; i++)
        buckets[i] = NULL;
    count = 0;
}

template <typename KeyType, typename ValueType>
HashMap<KeyType, ValueType>::~HashMap() {
    deleteAll();
}

template <typename KeyType, typename ValueType>
inline int HashMap<KeyType, ValueType>::size() const {
    return count;
}

template <typename KeyType, typename ValueType>
inline bool HashMap<KeyType, ValueType>::isEmpty() const {
    return count == 0;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::clear() {
    deleteAll();

    nBuckets = INTIAL_BUCKET_COUNT;
    buckets = new Cell*[nBuckets];
    for (int i = 0; i < nBuckets; i++)
        buckets[i] = NULL;
    count = 0;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::clearData() {
    if (count > 0) {
        for (int i = 0; i < nBuckets; i++) {
            if (buckets[i] != NULL) {
                Cell* next = buckets[i];
                Cell* cp = next;
                buckets[i] = NULL; //
                while (next != NULL) {
                    next = next->link;
                    delete cp;
                    cp = next;
                }
            }
        }
        //        delete[] buckets;
        count = 0;
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    if (getLoadFactor() > REHASH_THRESHOLD)
        rehash(false, 0); //默认方法扩容并rehash
    int bucket = hashCode(key) % nBuckets;
    Cell* cp = findCell(key, bucket);
    if (cp == NULL) {
        cp = new Cell;
        cp->key = key;
        cp->link = buckets[bucket];
        buckets[bucket] = cp;
    }
    cp->value = value;
    count++;
}

template <typename KeyType, typename ValueType>
bool HashMap<KeyType, ValueType>::containsKey(const KeyType& key) const {
    int bucket = hashCode(key) % nBuckets;
    Cell* cp = findCell(key, bucket);
    return cp != NULL;
}

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType, ValueType>::get(const KeyType& key) const {
    int bucket = hashCode(key) % nBuckets;
    Cell* cp = findCell(key, bucket);
    if (cp != NULL)
        return cp->value;

    std::cerr << "Not found key:" << key << std::endl;
    exit(1);
}

template <typename KeyType, typename ValueType>
ValueType HashMap<KeyType, ValueType>::operator[](const KeyType& key) const {
    return get(key);
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::remove(const KeyType& key) {
    int bucket = hashCode(key) % nBuckets;
    Cell* cp = findCell(key, bucket);
    if (cp == NULL) {
        std::cerr << "Not found key:" << key << std::endl; //cerr??
        return;
    }

    if (buckets[bucket] == cp) {//要删除的节点是第bucket桶首元素
        buckets[bucket] = cp->link;
        delete cp;
        count--;
        return;
    }

    Cell* left = buckets[bucket];
    while (left->link != cp)
        left = left->link;
    left->link = cp->link;
    delete cp;
    count--;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::expandAndRehash(const int & newNBuckets) {
    if (newNBuckets > 1.4 * nBuckets)
        rehash(true, newNBuckets);
}

template <typename KeyType, typename ValueType>
template<typename Func>
void HashMap<KeyType, ValueType>::mapAll(Func fn) const {
    for (int i = 0; i < nBuckets; i++) {
        Cell* cp = buckets[i];
        while (cp != NULL) {
            fn(cp->key, cp->value);
            cp = cp->link;
        }
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::mapAll(void(*fn)(const KeyType&, const ValueType&)) {
    for (int i = 0; i < nBuckets; i++) {
        Cell* cp = buckets[i];
        while (cp != NULL) {
            fn(cp->key, cp->value);
            cp = cp->link;
        }
    }
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::display() const {
    std::cout << std::endl;
    for (int i = 0; i < nBuckets; i++) {
        std::cout << "Bucket[" << i << "] ";
        for (Cell* cp = buckets[i]; cp != NULL; cp = cp->link)
            std::cout << " -> [" << cp->key << ":" << cp->value << "]";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename KeyType, typename ValueType>
std::string HashMap<KeyType, ValueType>::toString() const {
    std::ostringstream os;
    os << "\n";
    for (int i = 0; i < nBuckets; i++) {
        os << "Bucket[" << i << "] ";
        for (Cell* cp = buckets[i]; cp != NULL; cp = cp->link)
            os << " -> [" << cp->key << ":" << cp->value << "]";
        os << "\n";
    }
    return os.str();
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::deleteAll() {
    clearData();
    delete[] buckets;
    nBuckets = 0;
    buckets = NULL;
}

template <typename KeyType, typename ValueType>
typename HashMap<KeyType, ValueType>::Cell *
HashMap<KeyType, ValueType>::findCell(const KeyType& key, const int & bucket)const {//特殊要求
    Cell* cp = buckets[bucket];
    while (cp != NULL && cp->key != key)
        cp = cp->link;
    return cp;
}

template <typename KeyType, typename ValueType>
void HashMap<KeyType, ValueType>::rehash(const bool & customCapacity, const int & newNBuckets) {
    std::cout << "rehashing...";
    Cell** old = buckets;
    int oldNBuckets = nBuckets;

    if (customCapacity)
        nBuckets = newNBuckets;
    else
        nBuckets *= 2;
    while (!isPrime(nBuckets))//桶数取素数
        nBuckets++;
    std::cout << "nBuckets = " << nBuckets << std::endl;

    count = 0;
    buckets = new Cell*[nBuckets];
    for (int i = 0; i < nBuckets; i++) {
        buckets[i] = NULL;
    }

    for (int i = 0; i < oldNBuckets; i++) {
        Cell* cp = old[i];
        Cell* tmp = cp;
        while (cp != NULL) {
            tmp = cp;
            put(cp->key, cp->value); //put方法用了new，则拷贝后需要delete释放旧的Cell
            cp = cp->link;
            delete tmp;
        }
    }
    delete[]old;
}

template <typename KeyType, typename ValueType>
const double HashMap<KeyType, ValueType>::REHASH_THRESHOLD = 0.7;

template <typename KeyType, typename ValueType>
std::ostream & operator<<(std::ostream & os, const HashMap<KeyType, ValueType> & src) {
    os << src.toString();
    return os;
}

template <typename KeyType, typename ValueType>
std::istream & operator>>(std::istream & is, const HashMap<KeyType, ValueType> & src) {
    is >> src.toString();
    return is;
}

#endif /* HASHMAP_H */

