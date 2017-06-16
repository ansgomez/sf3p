#ifndef _SERVER_H
#define _SERVER_H

#include "core/ResourceAllocator.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Server : public ResourceAllocator {
 public:

  /*********** CONSTRUCTOR ***********/
   
  ///Constructor needs pointer to simulation as well as the scheduler's id and hierarchical level
  Server( unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM THREAD ****/

  ///This is the pthread's wrapper function
  void wrapper();
  
  /**** FROM RUNNABLE ****/
  
  ///This function rewrites the activate method to activate both the scheduler as well as its load
  void activate();
  
  ///This function rewrites the deactivate method both the scheduler as well as its load
  void deactivate();

  /**** FROM RESOURCEALLOCATOR ****/

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void finishedJob(unsigned int runnable_id);

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void newJob(Runnable *r);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  virtual void updateRunnable(Runnable* r);
  
  /*********** MEMBER FUNCTIONS ***********/

  virtual void serve();
};

#endif
