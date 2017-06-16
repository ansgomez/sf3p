#ifndef _DBS_H
#define _DBS_H

#include "queues/RunnableQueue.h"
#include "servers/Server.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class DBS : public Server {

  private:

    RunnableQueue queue;

};
#endif
