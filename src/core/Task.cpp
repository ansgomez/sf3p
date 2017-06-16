#include "core/Task.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Task::Task(_task_load load) {
  task_load = load;
}

/*********** MEMBER FUNCTIONS ***********/

///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
void Task::fire() {
  cout << "Task::fire - This should not print!\n";
}
