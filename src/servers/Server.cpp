#include "servers/Server.h"

#include "core/Simulation.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation as well as the scheduler's id and hierarchical level
Server::Server(unsigned int _id) : ResourceAllocator(_id){
  id = _id;
  thread_type = server;
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This is the pthread's wrapper function
void Server::wrapper() {

  //Wait until the simulation is initialized
  while( !Simulation::isInitialized() );

  serve();
}

/**** FROM RUNNABLE ****/

///This function rewrites the activate method to activate both the scheduler as well as its load
void Server::activate() {
  cout << "Server::activate() - This should not print!\n";
}

///This function rewrites the deactivate method to deactivate both the scheduler as well as its load
void Server::deactivate() {
  cout << "Server::deactivate() - This should not print!\n";
}

/**** FROM RESOURCEALLOCATOR ****/

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void Server::newJob(Runnable *r) {
  //empty
  cout << "Server::newJob - This should not print!\n";
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void Server::finishedJob(unsigned int id) {
  //empty
  cout << "Server::finishedJob - This should not print!\n";
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void Server::updateRunnable(Runnable* r) {
  //empty
  cout << "Server::updateRunnable - This should not print!\n";
}

/********************* MEMBER FUNCTIONS *********************/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void Server::serve() {
  //empty
  cout << "Server::serve - This should not print!\n";
}
