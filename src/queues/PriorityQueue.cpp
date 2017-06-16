#include "queues/PriorityQueue.h"

#include "core/Criteria.h"
#include "core/Runnable.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Contructor needs nothing
PriorityQueue::PriorityQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue depending on the Runnable's priority, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
bool PriorityQueue::protectedInsertRunnable(Runnable *newRunnable) {
  //increase the size counter only when current runnable was not already in the queue
  size++;

  if(newRunnable->getCriteria() == NULL) {
    cout << "PriorityQueue::insertRunnable() error - newRunnable's criteria is null!\n";
    return false;
  }

  #if _INFO==1
  cout << "PriorityQueue::insertRunnable() - size is now: " << size << endl;
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

  unsigned int currentPriority = head->r->getCriteria()->getPriority();
  unsigned int newPriority = newRunnable->getCriteria()->getPriority();

  #if _DEBUG==1
  cout << "Comparing: " << currentPriority << " vs " << newPriority << endl;
  #endif

  //If in a non-empty queue, newRunnable has a priority higher than the head, it becomes the new head
  if(currentPriority < newPriority ) {
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

  currentPriority = tail->r->getCriteria()->getPriority();

  #if _DEBUG==1
  cout << "Comparing: " << currentPriority << " vs " << newPriority << endl;
  #endif

  //If in a non-empty queue, the new runnable has an equal or lower priority than the tail, it is the new tail
  if( currentPriority >= newPriority ) {
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
    if (aux->r->getCriteria() != NULL) {
      currentPriority = aux->r->getCriteria()->getPriority();
    }
    else {
      cout << "PriorityQueue::insertRunnable() error - runnable criteria is null!\n";
    }

    #if _DEBUG==1
    cout << "Comparing: " << currentPriority << " vs " << newPriority << endl;
    #endif

    if( currentPriority < newPriority) {
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

  cout << "PriorityQueue::insertRunnable() error! newRunnable was not inserted...\n";
  return false;
}
