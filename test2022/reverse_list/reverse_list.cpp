#include "../head.h"

template<typename T>
void printList(LIST_NODE<T>* list, std::string str = "")
{
    if (str != "") {
        std::cout << str << "\n";
    }

    while (list) {
        std::cout << list->data << "\n";
        list = list->next;
    }
}

int main()
{
    LIST_NODE<int>* head = new LIST_NODE<int>;
    if (head == nullptr) {
        return 0;
    }
    head->data = 0;
    LIST_NODE<int>* tmp = head;
    for (int i = 1; i < 10; i++) {
        tmp->next = new LIST_NODE<int>;
        if (tmp->next == nullptr) {
            return 0;
        }
        tmp->next->data = i;
        tmp = tmp->next;
    }

    tmp = head;
    printList(tmp, "construct list:");

    // reverse 
    LIST_NODE<int>* next = head->next;
    head->next = nullptr;
    while (head && next) {
        LIST_NODE<int>* tmp = next->next;
        next->next = head;
        head = next;
        next = tmp;
    }
    printList(head, "after reverse: ");

    return 0;
}