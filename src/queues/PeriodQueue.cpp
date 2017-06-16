#include "queues/PeriodQueue.h"

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
PeriodQueue::PeriodQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue depending on the Runnable's period, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
bool PeriodQueue::protectedInsertRunnable(Runnable *newRunnable) {
  //increase the size counter
  size++;
  
  #if _INFO==1
  cout << "PeriodQueue::insertRunnable - size is now: " << size << endl;
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

  struct timespec currentPeriod = head->r->getCriteria()->getPeriod();
  struct timespec newPeriod = newRunnable->getCriteria()->getPeriod();

  #if _DEBUG==1
  cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
  #endif

  //If in a non-empty queue, newRunnable has a period smaller than the head, it becomes the new head
  if(currentPeriod > newPeriod ) {
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

  currentPeriod = tail->r->getCriteria()->getPeriod();

  #if _DEBUG==1
  cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
  #endif

  //If in a non-empty queue, the new runnable has a period larger than or equal to the tail, it is the new tail
  if( currentPeriod <= newPeriod ) {
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
    currentPeriod = aux->r->getCriteria()->getPeriod();

    #if _DEBUG==1
    cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
    #endif

    if( currentPeriod > newPeriod) {
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

  cout << "PeriodQueue::insertRunnable() error! newRunnable was not inserted...\n";
  return false;
}
