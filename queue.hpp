//
//  queue.hpp
//  dijkstraC++
//
//  Created by Aleksandra Jażdżewska on 24/04/2023.
//

#ifndef queue_hpp
#define queue_hpp

#include <stdio.h>

#endif /* queue_hpp */
#include <iostream>
//#include "queue.hpp"
using namespace std;

class myQueue{
public:
    typedef struct point{
        int x = -1;
        int y = -1;
    }point;
    
    
    struct node {
        point data;
        struct node *next = NULL;
    };
    
   
    int size=0;
    struct node* head = NULL;
    struct node* tail = NULL;
    struct node* tmp = NULL; // = NULL?

   // point point2;
    myQueue();
    ~myQueue();
    void enqueue(point value); //add item to the queue (at the end)
    void dequeue(); //remove item from the queue (first element)
    void popBack();
    point getHead(); //get front of queue
    point getTail(); //get back of gueue
    bool empty();
    //void display();
};

myQueue::myQueue(){ //: size(0){};
        this->head = NULL;
        this->tail = NULL;
        this->tmp = NULL;
        this->size = 0;
};

myQueue::~myQueue(){
    node* current = tail;
    while( current != NULL ){
        node* next = current->next;
        delete current;
        current = next;
    }
    tail = NULL;
}

void myQueue::enqueue(point value){
    if(tail == NULL){ //then we insert 1 node which is both front and back
        tail = new node;
        tail -> next = NULL; //cause it has size 1
        tail -> data = value;
        head = tail; //only 1 node so it is both head and tail
        size = 1;
    }
    else{
        tmp = new node;
        tail -> next = tmp;
        tmp -> data = value;
        tmp -> next = NULL;
        tail = tmp;
        size++;
    }
};

void myQueue::dequeue(){
    tmp = head;
    if(head == NULL){ //queue is empty
        cout << "Queue is empty" << endl;
    }
    else if(tmp -> next != NULL){ //jesli za headem jest jakis node
        tmp = tmp -> next; //to ustaw tmp na ten node bo to on bedzie pierwszy po usunieciiu head
      //  cout << "deleted " << head->data << endl;
        free(head);
        head = tmp;
        size--;
    }
    else{
     //   cout << "deleted " << head->data << endl;
        free(head);
        head = NULL;
        tail=NULL;
        size--;
    }
    
};

void myQueue::popBack(){
   // tmp -> next = tail;
    if(tail == NULL){
        cout << "Queue is empty" << endl;
    }
    else{
        if(tail == head){
            free(tail);
            head = NULL;
            tail = NULL;
        }
        else{
            tmp = head;
            while(tmp -> next -> next != NULL){
                tmp = tmp->next;
            }
            free(tmp-> next);
            tmp->next = NULL;
            tail = tmp;
            
//           // tmp -> next -> data = NULL;
//            tail = NULL;
//            free(tail);
//           // tmp -> next = NULL;
//        //    tmp -> next -> data = NULL; //???
//            tail = tmp;
        }
        size--;
    }
};

bool myQueue::empty(){
    if(head == NULL){
        return true;
    }
    else
        return false;
}

myQueue::point myQueue::getHead(){
    if(head == NULL){
       // cout << "is empty";
        point value;
        value.x = -1;
        value.y = -1;
        return value;
    }
    else{
        return head->data;
    }
};
myQueue::point myQueue::getTail(){
    if(tail == NULL){
        //cout << "is empty";
        point value;
        value.x = -1;
        value.y = -1;
        return value;
    }
    else
        return tail->data;
};


