#ifndef _CBS_H
#define _CBS_H

#include "queues/QueueCBS.h"
#include "servers/Server.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class CBS : public Server {

  private:

    QueueCBS queue;

  public:

    void replenish();

    void serve();

};
#endif
