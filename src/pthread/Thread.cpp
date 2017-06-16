#include "pthread/Thread.h"

#include "pthread/Priorities.h"
#include "results/Statistics.h"

#include <iostream>
#include <stdlib.h>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs nothing to create thread (with inactive priotity). Note that there should never be a simple Thread object because its wrapper function is null. (It should be called from a subclass)
Thread::Thread(unsigned int _id) {

  id = _id;
  joined = false;

  #if _INFO==1
  cout << "Thread " << _id << " created\n";
  #endif

  pthread_attr_init(&thread_attr);

  //creating the pthread
  if (pthread_create(&thread,&thread_attr, static_wrapper, this)) {
    cout << "Error creating thread\n";
    exit(-1);
  }
                
  //Setting its priority
  setPriority(Priorities::get_inactive_pr());	  
}

Thread::~Thread() {
  cout << "Thread Object " << id << " is being destructed\n";
}

/*********** MEMBER FUNCTIONS ***********/

///This function blocks the calling thread until this thread exits
void Thread::join() {
  if(joined) {
    return;
  }
  pthread_join(thread, NULL);
  joined = true;
}

///Copy of the join() method -> in case join() is redefined by a subclass
void Thread::join2() {
  if(joined) {
    return;
  }
  pthread_join(thread, NULL);
  joined = true;
}

///The runnable thread points to this static function. This function recieves a Thread object, and calls the wrapper function in that object
void * Thread::static_wrapper(void * This)
{
  Thread *t = ((Thread*)This);
  t->wrapper(); 

  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
 
  //Save the runtime statistic
  Statistics::addRuntime(t->thread_type, t->id, ts);

  pthread_exit(NULL);
  return NULL;
}

///Should be overwritten by all subclasses
void Thread::wrapper() {
  cout << "Thread::wrapper (" << id << ") - This should not print!\n";
}

/*********** GETTER AND SETTER FUNCTIONS ***********/

///This function return the thread id
unsigned int Thread::getId() {
  return id;
}

///This function sets the priority of the thread
void Thread::setPriority(int priority) {
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = priority;
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  
}
