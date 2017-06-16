#ifndef _PERIODQUEUE_H
#define _PERIODQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class PeriodQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  PeriodQueue();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function inserts the new runnable in the queue depending on the Runnable's period, returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
  bool protectedInsertRunnable(Runnable *newRunnable);
};

#endif 
