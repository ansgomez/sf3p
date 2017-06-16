#ifndef _RUNNABLEQUEUE_H
#define _RUNNABLEQUEUE_H

#include <semaphore.h>

class Runnable;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

///This structure is the basis for the linked list that is the RunnableQueue. It holds a pointer to the Runnable, and a pointer to the next Node.
typedef struct _Node {
  Runnable *r;
  struct _Node *next;
} Node;

class RunnableQueue {

 protected:

 /*********** VARIABLES ***********/

  ///This variable holds the size of the queue
  int size;

  ///The head of the queue points the current job. The rest of the queue is ordered depending of the method used to insert into the queue
  Node* head;

  ///The tail of the queue points to the last job on the active queue.
  Node* tail;

  ///This semaphore protects access to the linked list (for insertion and removal)
  sem_t queue_sem;
  
 public:

  /*********** CONSTRUCTOR ***********/
  
  ///Contructor needs nothing
  RunnableQueue();

 protected:

  /*********** PROTECTED MEMBER FUNCTIONS ***********/

  ///This function removes the queue's pointer to the Runnable with the specified id. returns true if the id was found, false otherwise
  bool protectedDeleteRunnable(unsigned int id);

  ///This function inserts the new runnable in the queue depending on the derived class' criteria
  virtual bool protectedInsertRunnable(Runnable* newRunnable);

 public:

  /*********** PUBLIC MEMBER FUNCTIONS ***********/
  
  ///This function is a public wrapper to the protectedDeleteRunnable function
  bool deleteRunnable(unsigned int id);

  ///This function is a public wrapper to the protectedInsertRunnable function
  bool insertRunnable(Runnable* newRunnable);

  ///This function reads the head of the queue and returns a pointer to the runnable
  Runnable* peek_front();

  ///This function reads the tail of the queue and returns the pointer to the Runnable
  Runnable* peek_back();

  ///This function reads the head of the queue, erases it from the RunnableQueue, and returns the head pointer
  Runnable* pop_front();

  /*********** GETTER FUNCTIONS ***********/
  
  ///This function returns the size of the RunnableQueue
  int getSize();
};

#endif 
