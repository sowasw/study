
#ifndef __WXL_TYPE__
#define __WXL_TYPE__

template<typename T>
struct LIST_NODE {
    T data;
    LIST_NODE<T>* next;

    LIST_NODE(): next(nullptr) { };
};

template<typename T>
struct DUAL_LIST_NODE {
    T data;
    DUAL_LIST_NODE<T>* prec;
    DUAL_LIST_NODE<T>* next;

    DUAL_LIST_NODE(): prec(nullptr), next(nullptr) { };
};

// template<typename T>
// typedef _LIST_NODE<T> LIST_NODE<T>;
// template<typename T>
// typedef _DUAL_LIST_NODE DUAL_LIST_NODE;

#endif