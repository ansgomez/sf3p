#ifndef _CBS_QUEUE_H
#define _CBS_QUEUE_H

#include <time.h>

class CBS;
class Task;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

///This structure defines the nodes for the SchedulerCBS queue
struct cbs_job {
  CBS *server;
  Task *task;
};

class QueueCBS {

  private:

    ///This is the head of the queue that holds all the ready jobs
    struct cbs_job* ready_queue;
   
    ///This is the head of the queue that holds all the pending jobs
    struct cbs_job* pending_queue;

    ///This holds the size of the pending queue
    int size;

  public:
  
    ///This function enqueues a new job in the scheduler. It could end up in either the ready_queue or the pending_queue
    void enqueue(unsigned int id, struct timespec arrival_time);

    ///This function dequeues a job from the ready_queue. This is called when the job has finished execution
    void dequeue(unsigned int id);
};
#endif
