#ifndef _THREAD_H
#define _THREAD_H

#include "util/Enumerations.h"

#include <pthread.h>

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Thread {

 protected:

  /*********** VARIABLES ***********/

  ///Thread id - a unique value among all threads in a simulation
  unsigned int id;

  ///Auxiliary variable to determine if thread has been joined
  bool joined;

  ///This is an auxiliary variable to set the thread's priority
  int policy;

  ///The actual pthread variable
  pthread_t thread;

  ///Auxiliary variable to set the thread's attributes
  pthread_attr_t thread_attr;

  ///Auxiliary variable to the set the thread's parameters
  struct sched_param thread_param;

  ///Type indicates wether the thread is a scheduler, worker, dispatcher, server, or idle
  _thread_type thread_type;

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs nothing to create thread (with inactive priotity);
  Thread(unsigned int _id);
  ~Thread();

  /*********** MEMBER FUNCTIONS ***********/
  
  ///Calling this function will block the calling thread until this thread exits
  virtual void join();

  ///Copy of the join() method -> in case join() is redefined by a subclass
  virtual void join2();

  ///This function is called by the static wrapper. In the Worker object, it should point to fire(), and in the Scheduler object, it should point to schedule()
  virtual void wrapper();

  /*********** GETTER AND SETTER FUNCTIONS ***********/

  ///This function returns the thread id
  unsigned int getId();

  ///This function sets the priority of the thread
  void setPriority(int);

 private:
 
  /*********** MEMBER FUNCTIONS ***********/
 
  ///The runnable thread points to this static function. This function recieves a Runnable object, and calls
  ///the wrapper function in that object
  static void * static_wrapper(void * This);

};
#endif
