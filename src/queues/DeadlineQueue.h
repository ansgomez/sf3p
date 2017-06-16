#ifndef _DEADLINEQUEUE_H
#define _DEADLINEQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class DeadlineQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  DeadlineQueue();

 protected:

  /*********** PROTECTED INHERITED FUNCTIONS ***********/

  /**** FROM RUNNABLEQUEUE ****/   
   
  ///This function inserts the new runnable in the queue depending on the Runnable's deadline, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
  bool protectedInsertRunnable(Runnable *newRunnable);
};

#endif 
