#include "util/Parser.h"

#include "core/Criteria.h"
#include "core/Dispatcher.h"
#include "core/ResourceAllocator.h"
#include "core/Runnable.h"
#include "core/Simulation.h"
#include "core/Worker.h"
#include "criteria/InclusiveCriteria.h"
#include "dispatchers/Aperiodic.h"
#include "dispatchers/Periodic.h"
#include "dispatchers/PeriodicJitter.h"
#include "schedulers/EDF.h"
#include "schedulers/EventBased.h"
#include "schedulers/FIFO.h"
#include "schedulers/FixedPriority.h"
#include "schedulers/RateMonotonic.h"
#include "schedulers/Scheduler.h"
#include "schedulers/TDMA.h"
#include "tasks/BusyWait.h"
#include "tasks/Video.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

#define _DEBUG 0
#define _INFO 0

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

//Constructor needs simulation pointer
Parser::Parser(Simulation *_sim) {
  sim = _sim;
}

/*********** PUBLIC MEMBER FUNCTIONS ***********/

///This function indicates whether the string parameter is an EventBased algorithm
bool Parser::isEventBased(string alg) {
  if(alg == "EDF") {
    return true;
  }
  if(alg == "FixedPriority") {
    return true;
  }
  if(alg == "RateMonotonic") {
    return true;
  }
  if(alg == "FIFO") {
    return true;
  }

  return false;
}

///This function receives the filepath and initiates the entire simulation
void Parser::parseFile(string filePath) {
  xml_document doc;
  unsigned int id = 1;

  //Load file
  if (!doc.load_file(filePath.data()) ) {
    cout << "Could not find file...\n";
    return;
  }

  //Create parent xml_node
  xml_node sim_node = doc.child("simulation");

  //Set parent (simulation) attributes
  sim->setName(sim_node.attribute("name").value());
  sim->setDuration(parseTime(sim_node.child("duration")));

  //Create top_sched node
  xml_node top_sched = sim_node.child("runnable");

  Scheduler *top = parseScheduler(NULL, top_sched, &id, 0); 
  sim->setTopScheduler(top);

  cout << "\n***   Loaded '" << sim_node.attribute("name").value() << "'\t***\n";
}

/*********** PRIVATE MEMBER FUNCTIONS ***********/

///This function parses a Criteria node, and it returns an initialized Criteria object
Criteria* Parser::parseCriteria(xml_node criteria_node) {
  Criteria* c;

  string type = criteria_node.attribute("type").as_string();

  if(type == "inclusive" || type=="inclusive_criteria") {
    c = (Criteria*) new InclusiveCriteria();
  }
  else {
    cout << "Parser::parseCriteria() error! Criteria type not recognized!\n";
    c = (Criteria*) new InclusiveCriteria();
  }

  xml_node relativeDeadline = criteria_node.child("relative_deadline");
  if( !relativeDeadline.empty() ) {
    c->setRelativeDeadline(parseTime(relativeDeadline));
  }
  else {
    c->setRelativeDeadline(TimeUtil::Millis(0));
  }

  xml_node period = criteria_node.child("period");
  if( !period.empty() ) {
    c->setPeriod(parseTime(period));
  }
  else {
    c->setPeriod(TimeUtil::Millis(100));
  }

  xml_node priority = criteria_node.child("priority");
  if( !priority.empty() ) {
    string aux = priority.attribute("value").as_string();
    c->setPriority((unsigned int) atoi(aux.c_str()) );
  }
  else {
    c->setPriority(100);
  }

  return c;
}

//This function extracts information from an XML "worker" and returns its corresponding disp.
Dispatcher* Parser::parseDispatcher(xml_node disp_node, unsigned int *id) {

  string periodicity = disp_node.attribute("periodicity").as_string();
  Dispatcher *disp = NULL;

  #if _INFO==1
  cout << "Creating Dispatcher " << *id << endl;
  #endif

  /**** CREATE DISPATCHER ****/
  if(periodicity == "periodic") {
    Periodic* p = new Periodic(*id);
    p->setPeriod(parseTime(disp_node.child("period")));
    disp = (Dispatcher*) p;
  }
  else if(periodicity == "periodic_jitter") {
    PeriodicJitter* p = new PeriodicJitter(*id);
    p->setPeriod(parseTime(disp_node.child("period")));
    p->setJitter(parseTime(disp_node.child("jitter")));
    disp = (Dispatcher*) p;
  }
  else if(periodicity == "aperiodic") {
    Aperiodic* a = new Aperiodic(*id);
    a->setReleaseTime(parseTime(disp_node.child("release_time")));
    disp = (Dispatcher*) a;
  }
  else {
    cout << "Parser Error: Runnable " << *id << "'s periodicity was not recognized\n";
  }

  /**** SET OFFSET ****/
  xml_node offset = disp_node.child("offset");
  if( !offset.empty() ) {
    disp->setOffset(parseTime(offset));
  }

  if(disp!=NULL) {
    sim->addDispatcher(disp); 
    sim->addThread((Thread*)disp);
  }

  return disp;
}

///This function receives an EDF node and it parses the entire node to return the full object
EventBased* Parser::parseEventBased(xml_node eb_node, unsigned int *id, int level) {
  string alg = eb_node.attribute("algorithm").as_string();

  EventBased* eb;

  if(alg == "EDF") {
    eb = (EventBased*) new EDF(*id, level);
  }
  else if(alg == "FixedPriority") {
    eb = (EventBased*) new FixedPriority(*id, level);
  }
  else if(alg == "RateMonotonic") {
    eb = (EventBased*) new RateMonotonic(*id, level);
  }
  else if(alg == "FIFO") {
    eb = (EventBased*) new FIFO(*id, level);
  }
  else {
    cout << "Parser::parseEventBased() error - algorithm not identified!\n";
    exit(0);
  }

  #if _INFO==1
  cout << "Creating EventBased " << *id << endl;
  #endif

  //iterate through all of the children nodes
  for (xml_node load = eb_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    *id = *id + 1;

    //If child is worker, parse a worker
    if( type == "worker" ) {
      parseWorker((ResourceAllocator*)eb, load, id);
    }
    //If child is scheduler, parse the correct scheduler
    else if( type == "scheduler" ) {
      parseScheduler((ResourceAllocator*)eb, load, id, level+1);
    }//end of scheduler
    else {
      cout << "Parser::parseEventBased() error - subrunnable type not identified!\n";
      exit(0);
    }
  }  

  return eb;
}

///This function receives a Scheduler and it call on the appropiate parsing function to return the full object
Scheduler* Parser::parseScheduler(ResourceAllocator* parent, xml_node sched_node, unsigned int *id, int level) {

  Scheduler *sched = NULL;
  string alg = sched_node.attribute("algorithm").as_string();

  //Call the appropiate parsing function
  if(alg == "TDMA") {
    sched = (Scheduler*) parseTDMA(sched_node, id, level);
  }
  else if(isEventBased(alg)) {
    sched = (Scheduler*) parseEventBased(sched_node, id, level);
  }
  else {
    cout << "Parser error: '" << alg << "' algorithm was not recognized\n";
  }

  //set the parent
  if(sched != NULL) {
    sched->setParent(parent);
    sim->addThread((Thread*)sched);
  }

  return sched; 
}

///This function receives and TDMA node, and parses its load
TDMA* Parser::parseTDMA(xml_node tdma_node, unsigned int *id, int level) {

  TDMA *tdma = new TDMA(*id, level);

  #if _INFO==1
  cout << "Creating TDMA " << *id << endl;
  #endif

  //iterate through all of the children nodes
  for (xml_node load = tdma_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    *id = *id + 1;

    //If child is worker, parse a worker
    if( type == "worker" ) {
      Worker *w = parseWorker((ResourceAllocator*)tdma, load, id);

      tdma->add_load((Runnable*) w);
    }
    //If child is scheduler, parse the correct scheduler
    else if( type == "scheduler" ) {
      Scheduler* aux = parseScheduler((ResourceAllocator*)tdma, load, id, level+1);
      tdma->add_load(aux);
    }//end of scheduler

    //TODO: only iterate through children of type runnable
  }
  
  //TIME SLOTS
  xml_node time_slots = tdma_node.child("time_slots");
  for (xml_node slot = time_slots.first_child(); slot; slot = slot.next_sibling()) {
    tdma->add_slot(parseTime(slot));
  }

  return tdma;
}

//This function converts an XML "time" node to a timespec
struct timespec Parser::parseTime(xml_node n) {
  int time = n.attribute("value").as_int();
  string units = n.attribute("units").value();
  struct timespec ret;

  if(units == "sec") {
    ret = TimeUtil::Seconds(time);
  }
  else if(units == "ms") {
    ret = TimeUtil::Millis(time);
  }
  else if(units == "us") {
    ret = TimeUtil::Micros(time);
  }
  else {
    cout << "Parser error: could not recognize time unit!\n";
  }

  return ret;
}


///This function receives a Worker node, its parent, and it returns the initialized worker object
Worker* Parser::parseWorker(ResourceAllocator* parent, xml_node worker_node, unsigned int *id) {
  string task = worker_node.attribute("task").as_string();
  Worker* worker = NULL;
  Criteria* c = NULL;

  #if _INFO==1
  cout << "Creating Worker " << *id << endl;
  #endif

  /**** SETTING THE DISPATCHER  ****/
  Dispatcher *d = parseDispatcher(worker_node, id);
  *id = *id + 1;  

  /**** SETTING THE TASK ****/
  if(task == "busy_wait") {
    BusyWait *bw = new BusyWait(parseTime(worker_node.child("wcet")));

    worker = new Worker(parent, *id, busy_wait);
    worker->setTask(bw);
    d->setWorker(worker);
  }
  else if(task == "video") {
    Video *vid = new Video();
    worker = new Worker(parent, *id, video);
    worker->setTask(vid);
    d->setWorker(worker);
  }
  else {
    cout << "Parser error: Worker " << *id << "'s task was not recognized\n";
  }

  /**** SETTING THE CRITERIA ****/
  xml_node criteria = worker_node.child("criteria");
  if(criteria!=NULL) {
    c = parseCriteria(criteria);
    worker->setCriteria(c);
  }
  else {
    c = (Criteria*) new InclusiveCriteria();
    c->setRelativeDeadline(TimeUtil::Millis(0));
    c->setPeriod(TimeUtil::Millis(100));
    c->setPriority(100);
    worker->setCriteria(c);    
  }

  /**** SETTING THE RELATIVE DEADLINE ****/
  xml_node relativeDeadline = worker_node.child("relative_deadline");
  if( !relativeDeadline.empty() ) {
    struct timespec aux = parseTime(relativeDeadline);
    worker->setRelativeDeadline(aux);
    //If runnable has relDel but criteria has none, set that as the default
    if(c->getRelativeDeadline() == TimeUtil::Millis(0) ) {
      c->setRelativeDeadline(aux);
    }
  }
  else {
    //By default, if no relative deadline is specified, default is 100ms
    worker->setRelativeDeadline(TimeUtil::Millis(100));
    if(c->getRelativeDeadline() == TimeUtil::Millis(0) ) {
      c->setRelativeDeadline(TimeUtil::Millis(100));
    }    
  }

  //Register thread with simulation object
  sim->addThread((Thread*)worker);

  return worker;
}
