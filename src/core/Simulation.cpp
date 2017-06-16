#include "core/Simulation.h"

#include "core/Dispatcher.h"
#include "core/Worker.h"
#include "pthread/Idle.h"
#include "pthread/Priorities.h"
#include "pthread/Thread.h"
#include "results/Statistics.h"
#include "results/Trace.h"
#include "schedulers/Scheduler.h"
#include "util/Operators.h"
#include "util/Parser.h"
#include "util/TimeUtil.h"

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** STATIC VARIABLES ***********/

///This attribute holds how long the simulation will last
struct timespec Simulation::simTime;

///This attribute indicate if the simulation is initialized
bool Simulation::initialized=false;

///This attribute indicates if there is currently a simulation
bool Simulation::simulating=false;  

/*********** CONSTRUCTOR ***********/
  ///Constructor needs the path to xml and the cpu_set
Simulation::Simulation(string _xml_path, int cpu) {
  simTime = TimeUtil::Seconds(1); 
  initialized = false;
  simulating = false;
  xml_path = _xml_path;

  //Set main process parameters
  param.sched_priority = Priorities::get_main_pr(); //highest priority
  if ((sched_setscheduler(0, SCHED_FIFO, &param) != 0)) {
    cout << "Run with root\n";
    pthread_exit(0);
  }

  //Check if file exist...
  struct stat buf;
  if (stat(xml_path.c_str(), &buf) == -1) {
    cout << "\nSimulation::Simulation() error! File '" << xml_path << "' not found...\n";
    xml_path = "error";
    return;
  }

  int n_cpus = sysconf( _SC_NPROCESSORS_ONLN );

  //if chosen cpu value is invalid, move it to the last core
  if( cpu < 0 && cpu >= n_cpus)
  {
    cpu = n_cpus-1;
  }

  //Set CPU affinity
  CPU_ZERO (&set);
  
  for(int i=0;i<n_cpus;i++) {
    //Set only the chosen cpu
    if(i== cpu) {
      CPU_SET (i, &set);
    }
    //By default, clear CPU from the set
    else {
      CPU_CLR (i, &set);
    }
  }


  if (pthread_setaffinity_np(pthread_self(), sizeof (cpu_set_t), &set) <0) {
    perror("Error setting CPU affinity\n");
  }

  initialize();
}

/*********** MEMBER FUNCTIONS ***********/

///This function sets the dispatchers to their 'active' priority.
void Simulation::activate_dispatchers() {
  for (unsigned int c=0;c<disp.size();c++) {
    disp[c]->activate();
  }
}

///This function adds dispatchers to the simulation object
void Simulation::addDispatcher(Dispatcher *d) {
  disp.push_back(d);
}

///This function adds thread pointers to the simulation object
void Simulation::addThread(Thread* t) {
  threads.push_back(t);
}

///This function initializes all of the objects
void Simulation::initialize() {
  //Reserve some memory for vectors
  disp.reserve(50);
  Statistics::initialize();

  //Idle should be the first thread to be created
  idle = new Idle();
  threads.push_back((Thread*)idle);

  Parser *parser = new Parser(this);

  parser->parseFile(xml_path);

  free(parser);
}

///This function waits for all other threads to join
void Simulation::join_all() {
  Thread* t;

#if _DEBUG==1
  cout << "Joining all threads...\n";
#endif

  for(unsigned int i=0;i<threads.size();i++) {
    t = threads[i];

    if(t!=NULL) {
      t->join();
    }
  }

#if _INFO==1
  cout << "Joined all!\n";
#endif
}


///This function begins the simulation
void Simulation::simulate() {

  if(xml_path.compare("error") == 0) {
    cout << "\nSimulation::simulate() error! Simulation was not loaded correctly!\n";
    return;
  }

  struct timespec rem;
  cout << "***   Simulating\t\t***\n" ;

  //Set simulation variables
  Statistics::enable();
  initialized=true;
  simulating=true;
  TimeUtil::setOffset();

  //Activate threads
  activate_dispatchers();
  top_sched->activate();

  //Sleep for the duration of the simulation
  nanosleep(&simTime, &rem);

  cout << "***   Done\t\t\t***\n";

  Statistics::addTrace((_thread_type)0,0,sim_end);

  //Disable statistics
  Statistics::disable();

  //Set flags
  simulating=false;

  //Join all other threads
  join_all();

#if _INFO==1
  cout << "Saving results...\n";
#endif

  //Save results to file
  Statistics::toFile(name);

  cout << "***   Results Saved!\t\t***\n\n";
}

/*********** STATIC FUNCTIONS ***********/

///This function returns the simulation time
struct timespec Simulation::getSimTime() {
  return simTime;
}

///This function tells if the simulation is initialized
bool Simulation::isInitialized() {
  return initialized;
}

///This function tells if there is currently a simulation
bool Simulation::isSimulating() {
  return simulating;
}

/*********** GETTER AND SETTER FUNCTIONS ***********/

///This function returns the name of the simulation
string Simulation::getName() {
  return name;
}

///This function sets the duration of the simulation
void Simulation::setDuration(struct timespec d) {
  simTime = d;
}

///This function sets the name of the simulation
void Simulation::setName(string s) {
  name = s;
}

///This function sets the top schedulerxs
void Simulation::setTopScheduler(Scheduler* sched) {
  top_sched = sched;
}
