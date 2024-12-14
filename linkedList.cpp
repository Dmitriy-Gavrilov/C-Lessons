#include <stdio.h>
#include <cstdio>
#include <locale.h>


struct Node {
    int number;
    Node* next;
};


void addNode(Node** head, int a) {

    Node* newElem = new Node();
    newElem->next = nullptr;
    newElem->number = a;

    if (*head == nullptr) {
        *head = newElem;
        return;
    }

    Node* current = *head;
    while (current->next != nullptr) {
        current = current->next;
    }
    //current = node3: 
    // number = 3
    //next = nullptr
    current->next = newElem;
}


void delNode(Node** head, int num) {
    if (*head == nullptr) {
        return;
    }

    Node* current = *head;
    Node* previous = nullptr;
    while (current != nullptr && current->number != num) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        return;
    }

    // current = node3
    // current.next = node4
    // previous = node2
    // previous.next = node3

    if (previous != nullptr){
        previous->next = current->next;
    }
    else {
        *head = current->next;
    }
    delete current;
    
}

void print(Node* head) {
    Node* current = head;
    while (current != nullptr) {
        printf("%d -> ", current->number);
        // current = node1
        // Node1:
        current = current->next;
    }

}

int main()
{
    setlocale(LC_ALL, "RUS");

    Node* head = nullptr;

    addNode(&head, 1);
    addNode(&head, 2);
    addNode(&head, 3);
    
    print(head);
    printf("\n");

    addNode(&head, 4);

    print(head);
    printf("\n");

    delNode(&head, 5);

    print(head);
    printf("\n");

    //head -> node1 -> node2 -> node3 -> nullptr
    // delNum = 3;
    // 
    // 1 -> 2 -> 3 -> 4
    // 1 -> 2 -> 4
    

    return 0;
}
