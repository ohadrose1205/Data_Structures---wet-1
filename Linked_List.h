//
// Created by ohadr on 11/28/2022.
//

#ifndef WORLDCUP23A1_CPP_LINKED_LIST_H
#define WORLDCUP23A1_CPP_LINKED_LIST_H

#include "Player.h"

//class linked_list{
//public:
//    linked_list();
//    ~linked_list();
//    linked_list(const linked_list& list);
//    linked_list& operator=(const linked_list& list);
//
//    void insert(Player& newItem);
//    void delete(Player& playerToDelete);
//    void pushBack(Player& newItem);
//
//
//private:
//    class Node{
//    public:
//        explicit Node(Player& p) : data(p){
//            next = nullptr;
//            prev = nullptr;
//        }
//        Node(){
//            data = nullptr;
//        }
//        ~Node()=default;
//        Node(const Node& node) = default;
//        Node& operator=(const Node& node) = default;
//    private:
//    Player data;
//    Node* next;
//    Node* prev;
//};
//private:
//    Node* head;
//};

#include "iostream"
#include <new>

///-----------------CONSTS--------------------//
const int EMPTY_LIST = 0;

///-----------------INTERFACE OF QUEUE CLASS--------------------//
template <class T>
class Linked_List{
private:
///-----------------IMPLEMENT OF NODE CLASS--------------------//
//c`tor of Node class
    class Node{
        T& m_nodeData;
        Node* m_next;
        friend class Linked_List<T>;

    public:
        Node(){
            m_nodeData = NULL;
            m_next = nullptr;
        }
        explicit Node(T data) : m_nodeData(data),m_next(nullptr){}
        ~Node() = default; //base on Player/Card
        Node(const Node &node) = default;
        Node& operator=(const Node &node) = default;
    };
    Node* m_first;
    Node* m_last;
    int m_size;

public:
///-----------------IMPLEMENT OF ITERATOR CLASS--------------------//
//interface and implementing of class Iterator
//function to move iterator to the head of queue
//operators of Iterator class
//operator == of Iterator class
    class Iterator{
        const Linked_List<T>* m_list;
        Node* m_node;
        Iterator(const Linked_List<T>& list, Node* indexNode) : m_list(list), m_node(indexNode){
//            Node* ptr = queue->m_first;
//            for(int i = INIT_ITERATOR; i < index; i++){
//                ptr = ptr->m_next;
//            }
//            m_node = ptr;
        }
        friend class Linked_List<T>;
        friend class Node;
    public:
        ~Iterator() = default;
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;
        T& operator*(){
            if(!m_node){
                throw InvalidOperation();
            }
            return m_node -> m_nodeData;
        }

        //operator prefix ++ of Iterator class
        Iterator& operator++(){
            if(m_node == nullptr){
                throw InvalidOperation();
            }
            m_node = m_node -> m_next;
            return *this;
        }

        //operator postfix ++ of Iterator class
        Iterator operator++(int){
            Iterator result = *this;
            ++*this; ///should be pointer here?
            return result;
        }

        //operator != of Iterator class
        friend bool operator!=(const Iterator& i1,const Iterator& i2) {
            return (i1.m_list != i2.m_list || i1.m_node != i2.m_node);
        }
        class InvalidOperation{};
    };

    class ConstIterator {
        const Linked_List<T> * m_queue;
        Linked_List<T>::Node* m_node;
        ConstIterator(const Linked_List<T>* list, Node* indexNode) : m_list(list), m_node(indexNode){
//            Node* ptr = queue->m_first;
//            for(int i = INIT_ITERATOR; i < index; i++){
//                ptr = ptr->m_next;
//            }
//            m_node = ptr;
        }
        friend class Linked_List<T>;
        friend class Node;

    public:
        ~ConstIterator() = default;
        ConstIterator(const ConstIterator &) = default;
        ConstIterator &operator=(const ConstIterator &) = default;
        const T& operator*()const{
//            Node* ptrElement = m_node;
            // bad alloc

            return m_node->m_nodeData;
        }
        ConstIterator& operator++(){
            if(m_node == nullptr){
                throw InvalidOperation();
            }
            m_node = m_node -> m_next;
            return *this;
        }

        //operator postfix ++ of ConstIterator class
        ConstIterator operator++(int){
            ConstIterator result = *this;
            ++*this;///should be pointer here?
            return result;
        }

        //operator != of ConstIterator class
        friend bool operator!=(const ConstIterator &i1,const ConstIterator &i2) {
            return (i1.m_queue != i2.m_queue || i1.m_node != i2 .m_node);
        }


        class InvalidOperation {};
    };
///-----------------METHOEDS & C`tors OF LINKED_LIST CLASS--------------------//
//c`tor of Queue
    Linked_List(){
        m_first = nullptr;
        m_last = nullptr;
        m_size = EMPTY_LIST;
    }
    //destructor of Queue
    ~Linked_List(){
        Node* current;
        while (m_first!= nullptr){
            current=m_first;
            m_first = m_first->m_next;
            delete current;
        }
        m_last = nullptr;
    }

//copy c`tor of List
    Linked_List<T>(const Linked_List<T>& list) : m_first(nullptr),m_last(nullptr), m_size(EMPTY_QUEUE){
        Queue<element>::ConstIterator it = list.begin();
        while (it != list.end()) {
            try {pushBack(*it);}
            catch (std::bad_alloc&){
                while (m_first){
                    popFront();
                }
                throw;
            }
            ++it;
        }
    }
    //assign operator of Queue
    Linked_List<T>& operator=(const Linked_List<T>& list){
        if(this == &list){
            return *this;
        }
        if(list.m_first == nullptr){
            while (m_first!= nullptr){
                popFront();
            }
            return *this;
        }
        Node* node= new Node(*list.m_first);
        Node* first = node;
        for(const element& elem : queue){
            try {
                node->m_next = new Node(elem);
            }
            catch(std::bad_alloc& e){
                while (first){
                    Node* toDelete = first;
                    first=first->m_next;
                    delete(toDelete);
                }
                throw e;
            }
            node = node->m_next;
        }
//
        while (m_first != nullptr){
            popFront();
        }
        m_first=first->m_next;
        delete(first);
        m_last=node;
        m_size = list.m_size;
        return *this;
    }

//function to push new object to the queue
    void pushBack(const T elementToAdd){
        Node* newElement= new Node(elementToAdd);

        if(m_first== nullptr){
            m_first=newElement;
            m_last= newElement;
        }
        else{
            m_last->m_next = newElement;
            m_last=m_last->m_next;
        }
        m_size++;
    }

//function to show (return) the first object of the queue FOR CONST
    const T& front() const{
        if(!m_first){
            throw EmptyQueue();
        }
        return (m_first -> m_nodeData);
    }
//function to show (return) the first object of the queue
    T& front() {
        if(!m_first){
            throw EmptyQueue();
        }
        return (m_first -> m_nodeData);
    }

//function to remove the first object of the queue
    void popFront(){
        if(!m_first){
            throw EmptyQueue();
        }
        Node* toDelete = m_first;
        m_first = m_first->m_next;
        delete(toDelete); //should be destructor?
        m_size--;

    }

//function that returned the size of the queue
    int size()const{
        return (m_size);
    }

    Iterator begin(){
        return Iterator(this,m_first);
    }

    //function to make iterator to the end of queue - TO NULL PLACE ONE AFTER THE LAST "REAL" OBJECT
    Iterator end(){
        return Iterator(this, nullptr);
    }

    class EmptyQueue{};
};


#endif //WORLDCUP23A1_CPP_LINKED_LIST_H
