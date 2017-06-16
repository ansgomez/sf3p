#include "queues/RunnableQueue.h"

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
RunnableQueue::RunnableQueue() {
  head = NULL;
  tail = NULL;
  size = 0;

  sem_init(&queue_sem, 0, 1); //mutex semaphore
}

/*********** PROTECTED MEMBER FUNCTIONS ***********/

///This function removes the queue's pointer to the Runnable with the specified id. returns true if the id was found, false otherwise
bool RunnableQueue::protectedDeleteRunnable(unsigned int id) {
  if(head!=NULL) {
    //If runnable is the head, delete the head
    if(head->r->getId()==id) {
      Node *aux = head;
      head = head->next;
      delete(aux);

      size--;
      return true;
    }
  }
  //if head is NULL, there is nothing to do
  else {
    return false;
  }

  Node *aux = head->next, *prev=head;

  //iterate to find runnable and delete it
  while(aux!=NULL) {
    if(aux->r->getId() == id) {
      if(aux == tail) {
	tail = prev;
      }
      prev->next = aux->next;
      delete(aux);
      size--;
      return true ;
    }
    prev = aux;
    aux = aux->next;
  }

  return false;  
}

///This function inserts the new runnable in the queue depending on the derived class' criteria
bool RunnableQueue::protectedInsertRunnable(Runnable* newRunnable) {
  cout << "RunnableQueue::insert - this should not print!\n";
  return false;
}

/*********** PUBLIC MEMBER FUNCTIONS ***********/

///This function is a public wrapper to the protectedDeleteRunnable function
bool RunnableQueue::deleteRunnable(unsigned int id) {
  bool ret;

  sem_wait(&queue_sem);
    ret = protectedDeleteRunnable(id);
  sem_post(&queue_sem);

  return ret;
}

///This function is a public wrapper to the protectedInsertRunnable function
bool RunnableQueue::insertRunnable(Runnable* newRunnable) {
  bool ret;

  if(newRunnable == NULL) {
    cout << "RunnableQueue::insertRunnable() - newRunnable is null!\n";
    return false;
  }

  sem_wait(&queue_sem);
    //Clear the queue of any nodes with this runnable id
    protectedDeleteRunnable(newRunnable->getId());
    //Insert into the queue
    ret = protectedInsertRunnable(newRunnable);
    //cout << size << endl;
  sem_post(&queue_sem);
  return ret;  
}

///This function reads the head of the queue (destructive read), and returns the pointer to the Runnable
Runnable* RunnableQueue::peek_back() {
  if(tail==NULL)
    return NULL;

  return tail->r;
}

///This function reads the head of the queue (non-destructive read) and returns a pointer to the runnable
Runnable* RunnableQueue::peek_front() {
  if(head==NULL)
    return NULL;

  return head->r;
}

///This function reads the head of the queue, erases it from the RunnableQueue, and returns the head pointer
Runnable* RunnableQueue::pop_front() {
  if(head==NULL)
    return NULL;

  Runnable* aux = head->r;

  deleteRunnable(aux->getId());

  return aux;
}

/*********** GETTER FUNCTIONS ***********/

///This function returns the size of the queue
int RunnableQueue::getSize() {
  return size;
}
