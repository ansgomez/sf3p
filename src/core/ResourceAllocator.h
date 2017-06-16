#ifndef _RESOURCE_ALLOCATOR_H
#define _RESOURCE_ALLOCATOR_H

#include "core/Runnable.h"
#include "util/Enumerations.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class ResourceAllocator : public Runnable {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs runnable's id
  ResourceAllocator(unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD ****/

  ///Calling this function will block the calling thread until this thread exits
  virtual void join();

  ///This is the pthread's wrapper function
  void wrapper();
  
  /**** FROM RUNNABLE ****/

  ///This function rewrites the activate method to activate both the scheduler as well as its load
  virtual void activate();

  ///This function rewrites the deactivate method both the scheduler as well as its load
  virtual void deactivate();

  /*********** MEMBER FUNCTIONS ***********/
  
  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void finishedJob(unsigned int id);

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void newJob(Runnable *r);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  virtual void updateRunnable(Runnable* r);
};
#endif
