#ifndef _RUNTIME_H
#define _RUNTIME_H

#include "util/Enumerations.h"

#include <string>
#include <time.h>

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Runtime {

 private:

  /*********** VARIABLES ***********/

  //Variables will be traced in the following order:

  ///This variable stores the type of thread the statistic belongs to
  enum _thread_type type;

  ///This variable stores the id of the thread the statistic belongs to
  unsigned int id;

  ///This variable stores the runtime of the thread
  struct timespec runtime;

 public:

  /*********** MEMBER FUNCTIONS ***********/

  ///This sets the thread type, id, and runtime of trace
  void setRuntime(enum _thread_type _type, unsigned int _id, struct timespec _rt);

  ///This function converts the runtime statistic to a string,
  string toString();

  /*********** GETTER FUNCTIONS ***********/  
  
  ///This function returns the ID of the trace
  unsigned int getId();

  /*********** OPERATORS ***********/

  ///This operator comparest two runtimestatistics
  int operator<(Runtime b);
};

#endif