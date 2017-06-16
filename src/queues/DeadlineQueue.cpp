#include "queues/DeadlineQueue.h"

#include "core/Criteria.h"
#include "core/Runnable.h"
#include "util/Operators.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Contructor needs nothing
DeadlineQueue::DeadlineQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue depending on the Runnable's deadline
bool DeadlineQueue::protectedInsertRunnable(Runnable *newRunnable) {
  //increase the size counter
  size++;
  
  #if _INFO==1
  cout << "DeadlineQueue::protectedInsertRunnable() - size is now: " << size << endl;
  #endif

  //Base case, the list was empty. The Runnable is now head and tail of queue
  if (head == NULL) {
    head = (Node*) malloc(sizeof(Node));
    head->r = newRunnable;
    tail = head;
    tail->next = NULL;

    #if _DEBUG==1
    cout << "New Head1: " << newRunnable->getId() << endl;
    #endif
    return true;
  }

  struct timespec currentDeadline = head->r->getCriteria()->getDeadline();
  struct timespec newDeadline = newRunnable->getCriteria()->getDeadline();

  #if _DEBUG==1
  cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;
  #endif

  //If in a non-empty queue, newRunnable has a deadline earlier than the head, it becomes the new head
  if(currentDeadline > newDeadline ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    //link new node to old head
    newNode->next = head;
    //move the head 
    head = newNode;

    #if _DEBUG==1
    cout << "New Head2: " << newRunnable->getId() << endl;
    #endif
    return true;
  }

  currentDeadline = tail->r->getCriteria()->getDeadline();

  #if _DEBUG==1
  cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;
  #endif

  //If in a non-empty queue, the new runnable has a deadline equal to or later than the tail, it is the new tail
  if( currentDeadline <= newDeadline ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    newNode->next = NULL;
    
    //link the old tail to the new node
    tail->next = newNode;
    //move the tail
    tail = newNode;

    #if _DEBUG==1
    cout << "New tail: " << newRunnable->getId() << endl;
    #endif
    return false;
  }

  Node* aux = head->next, *prev=head;

  //This loop will insert the Runnable in any position except first or last
  while(aux != NULL) {
    currentDeadline = aux->r->getCriteria()->getDeadline();

    #if _DEBUG==1
    cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;
    #endif

    if( currentDeadline > newDeadline) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux;
      prev->next = newNode;

      #if _DEBUG==1
      cout << "New node!" << newRunnable->getId() << endl;
      #endif
      return false;
    }
    
    prev=aux;
    aux = aux->next;
  }

  cout << "DeadlineQueue::protectedInsertRunnable() error! newRunnable was not inserted...\n";
  return false;
}
