#ifndef _FIFOQUEUE_H
#define _FIFOQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class FIFOQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  FIFOQueue();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function inserts the new runnable in the queue depending on Runnable's arrivalTime, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
  bool protectedInsertRunnable(Runnable *newRunnable);
};

#endif
