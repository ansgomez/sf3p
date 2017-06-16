#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include "pthread/Thread.h"
#include "util/Enumerations.h"

class Criteria;
class ResourceAllocator;
class Task;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Runnable : public Thread {

 protected:

  /*********** VARIABLES ***********/

  ///Auxiliary variable to hold the state of runnable
  _runnable_state state;

  ///Runnable's schedulable criteria (this is the criteria used to schedule it)
  Criteria* criteria;

  ///Pointer to the scheduler that this runnable belongs to
  ResourceAllocator* parent;

  ///Pointer to Runnable currently being 'executed' by the runnable
  Runnable* current_runnable;
  
  ///Pointer to Task currently being 'executed' by the runnable
  Task* current_task;

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs simulation pointer and an id
  Runnable(unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM THREAD ****/

  ///Calling this function will block the calling thread until this thread exits
  virtual void join();

  ///This function will be defined by subclasses
  virtual void wrapper();

  /**** FROM RUNNABLE ****/
  
  ///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
  virtual void activate();

  ///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
  virtual void deactivate();


  /*********** GETTERS AND SETTERS ***********/

  ///This function return the runnable's schedulable criteria
  Criteria* getCriteria();

  ///This function sets the runnable's parent
  void setParent(ResourceAllocator* p);
};
#endif
