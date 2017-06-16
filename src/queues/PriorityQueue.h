#ifndef _PRIORITYQUEUE_H
#define _PRIORITYQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class PriorityQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  PriorityQueue();

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM RUNNABLEQUEUE ****/

  ///This function inserts the new runnable in the queue depending on the Runnable's priority, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
  bool protectedInsertRunnable(Runnable *newRunnable);
};

#endif 
