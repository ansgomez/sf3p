#include "queues/FIFOQueue.h"

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
FIFOQueue::FIFOQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable at the end of the queue, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
bool FIFOQueue::protectedInsertRunnable(Runnable *newRunnable) {
  //increase the size counter
  size++;
  
  #if _INFO==1
  cout << "FIFOQueue::insertRunnable - size is now: " << size << endl;
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

  struct timespec currentArrivalTime = head->r->getCriteria()->getArrivalTime();
  struct timespec newArrivalTime = newRunnable->getCriteria()->getArrivalTime();

  #if _DEBUG==1
  cout << "Comparing: " << currentArrivalTime.tv_sec << ":" << currentArrivalTime.tv_nsec << " vs " << newArrivalTime.tv_sec << ":" << newArrivalTime.tv_nsec << endl;
  #endif

  //If in a non-empty queue, newRunnable has an arrivalTime earlier than the head, it becomes the new head
  if(currentArrivalTime > newArrivalTime ) {
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

  currentArrivalTime = tail->r->getCriteria()->getArrivalTime();

  #if _DEBUG==1
  cout << "Comparing: " << currentArrivalTime.tv_sec << ":" << currentArrivalTime.tv_nsec << " vs " << newArrivalTime.tv_sec << ":" << newArrivalTime.tv_nsec << endl;
  #endif

  //If in a non-empty queue, the new runnable has an arrival time equal to or later than the tail, it is the tail
  if( currentArrivalTime <= newArrivalTime ) {
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
    currentArrivalTime = aux->r->getCriteria()->getArrivalTime();

    #if _DEBUG==1
    cout << "Comparing: " << currentArrivalTime.tv_sec << ":" << currentArrivalTime.tv_nsec << " vs " << newArrivalTime.tv_sec << ":" << newArrivalTime.tv_nsec << endl;
    #endif

    if( currentArrivalTime > newArrivalTime) {
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

  cout << "FIFOQueue::insertRunnable() error! newRunnable was not inserted...\n";
  return false;
}
