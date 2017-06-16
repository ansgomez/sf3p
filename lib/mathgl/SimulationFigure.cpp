#include "SimulationFigure.h"

#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

/********************************************************************************
 * VARIABLES
 ********************************************************************************
 */

const char *plot_color[] = {"k", "b", "g", "r", "q", "c", "p", "h", "u", "P", "M"};

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs the prefix to the '_traces.csv' file
SimulationFigure::SimulationFigure(string _prefix) {

  prefix = _prefix;

  //setting default values
  height = 0.75;
  width = 0.5;
  n_plots = 1;
  base = 0.05;
  plot_offset = 0;
  arrow_length = 1.3;

  gr = new mglGraph;
  x1=0; x2=100; y1=0; y2=1.5;


  importCSV();
}

//This function draws an arrival arrow at time t, with an option label
void SimulationFigure::drawArrival(int plot, double time, int color) {
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
          
    //Draw Arrow
    gr->Line(mglPoint(time,base), mglPoint(time,base+height*arrow_length), "A1");
          
    //Draw Label
    //float xpt[] = {time,NaN}; float ypt[] = {height*arrow_length+0.05, NaN};
    //gr->Label(mglData(2, xpt), mglData(2, ypt), label);
  }
}

//This function draws a rectangle of a defined color starting at time t and a relative width
void SimulationFigure::drawJob(int plot, double start, double end, int clr) {
  //Assign plot color, unless it's overwritten
  int color = (clr!=-1)? clr:plot+1;
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    //Draw Face
    gr->Face(mglPoint(start,base), mglPoint(end,base), mglPoint(start,base+height), mglPoint(end,base+height), plot_color[color]);
  }
}

//This function draw a green downward arrow at the specified time to indicate a met deadline
void SimulationFigure::drawMetDeadline(int plot, double time) {
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
          
    //Draw Arrow
    gr->Line(mglPoint(time,base), mglPoint(time,base+height*arrow_length), "_A1");
  }
}

//This function draw a green downward arrow at the specified time to indicate a met deadline
void SimulationFigure::drawMissedDeadline(int plot, double time) {
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
          
    //Draw Arrow
    gr->Line(mglPoint(time,base), mglPoint(time,base+height*arrow_length), "_Ar1");
  }
}

//This function draws an arrow indicating that the scheduler activated plot_i during this time
void SimulationFigure::drawSched(int plot, double sched_start, double sched_end, int clr) {
  //Assign plot color, unless it's overwritten
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    gr->Line(mglPoint(sched_start,base+height*1.1), mglPoint(sched_end,base+height*1.1), "II");
  }
}

//This function exports the mgl graph to an eps file
void SimulationFigure::exportEPS() {
  string path = prefix + "_figure.eps";
  if(gr!=NULL) {
    gr->WriteEPS(path.data(),"");
    //gr->WriteTEX("blah.tex","");
    printf("***   Saved EPS Figure\t\t***\n");
  }
  else {
    printf("ERROR: Graph was null\n");
  }
}

//This function exports the mgl graph to a png file
void SimulationFigure::exportPNG() {
  string path = prefix + "_figure.png";
  if(gr!=NULL) {
    gr->WritePNG(path.data(),"",false);
    printf("***   Saved PNG Figure\t\t***\n");
  }
  else {
    printf("ERROR: Graph was null\n");
  }
}

//This function exports the mgl graph to an svg file
void SimulationFigure::exportSVG() {
  string path = prefix + "_figure.svg";
  if(gr!=NULL) {
    gr->WriteSVG(path.data(),"");
    printf("***   Saved SVG Figure\t\t***\n");
  }
  else {
    printf("ERROR: Graph was null\n");
  }
}

///This function generates a figure from the trace vector
void SimulationFigure::genFig() {
  char title[20];
  unsigned int c;
 
  vector<unsigned int>::iterator it;
  int id;

  unsigned int n_workers = worker_id.size();

  vector<unsigned int> start_sched_us, start_time;
  start_sched_us.reserve(n_workers);
  start_time.reserve(n_workers);
  
  gr->SetArrowSize(1.5); //set default arrow size


  struct timespec range = TimeUtil::Millis(300);
  
  //Define values
  setNPlots(n_workers);
  setTimeRange(TimeUtil::convert_ms(range));

  //Initialize each worker plot
  for(c=0;c<n_workers;c++) {
    start_time[c] = 0;
    start_sched_us[c] = 0;

    sprintf(title, "Worker %d ", c+1);
    initPlot(c, title);
  }

  //Iterate through traces and build job trace
  for(c=0; c<traces.size() ;c++) {
    it = find(worker_id.begin(), worker_id.end(), traces[c].getId());
    id = (int) (it-worker_id.begin()); //get the index where the ID was found

    if(traces[c].getTimestamp() > range)
      break;

    switch(traces[c].getAction()) {
    case task_arrival:
      {
      drawArrival(id, TimeUtil::convert_ms(traces[c].getTimestamp()));
      }
      break;   

    case task_start:
      {
      start_time[id] = TimeUtil::convert_us(traces[c].getTimestamp());
      }
      break;

    case sched_start:
      {
      start_sched_us[id] = TimeUtil::convert_us(traces[c].getTimestamp());

      if(start_time[id] != 0) {
	start_time[id] = start_sched_us[id];
      }
      }
      break;

    case sched_end:
      {
      if(true) { 
	float x = start_sched_us[id]/1000;
	drawSched(id, x,TimeUtil::convert_ms(traces[c].getTimestamp()) );
      }

      if(start_time[id] != 0) {
	float x = start_time[id]/1000;
	drawJob(id, x, TimeUtil::convert_ms(traces[c].getTimestamp()) );
      }
      }
      break;

    case task_end:
      {
      float x = start_time[id]/1000;
      drawJob(id, x, TimeUtil::convert_ms(traces[c].getTimestamp()) );
      start_time[id] = 0;
      }
      break;

    case deadline_missed:
      {
      drawMissedDeadline(id, TimeUtil::convert_ms(traces[c].getTimestamp()));
      }
      break;

    case deadline_met:
      {
      drawMetDeadline(id, TimeUtil::convert_ms(traces[c].getTimestamp()));
      }
      break;
    }
  }
}

///This function first generates a figure in memory (by calling genFig()) and automatically exports them to EPS and SVG file formats
void SimulationFigure::generateFigures() {
  //first generate the mgl graph
  genFig();

  //then export graph to figures
  exportEPS();
  exportSVG();
}


///This function imports data from a CSV file with the previously set prefix
void SimulationFigure::importCSV() {

#if _INFO==1
  cout << "Trying to read from file...\n";
#endif
  
  string name = prefix + "_traces.csv";

  //Tranforms name to c string
  char *cstr = new char[name.length() + 1];
  strcpy(cstr, name.c_str());

  ifstream infile;
  infile.open(cstr);

  string   line;
  while(getline(infile,line)) {
    parseLine(line);
  }
  //sort worker_id vector
  sort_worker_id();
}

//Initialize a plot with y-label and default settings (ticks, ranges, etc)
void SimulationFigure::initPlot(int plot, char *name) { 
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    //gr->Label('y', name, 0); 
    gr->Puts(mglPoint(-15,0.5),mglPoint(-10,0.5),name,"",8);
    gr->SetTicks('x', 5, 0);
    gr->Axis("xT");
  }
}

///This function parses one line from a CSV file
void SimulationFigure::parseLine(string line) {
  struct timespec ts;
  enum _task_action task_action;
  unsigned int id;
  char *str;
  Trace aux;

  //Tranforms line to c string
  char *cstr = new char[line.length() + 1];
  strcpy(cstr, line.c_str());

  //Tokenize c string
  str = strtok(cstr, ",");
 
  //Extract and parse the timestamp
  ts = parseTimespec(str);
  
  //Extract and parse the thread id
  str = strtok(NULL, ",");
  id = (unsigned int) atoi(str);

  //Add ID to worker_id vector
  workerID_vector(id);

#if _INFO==1
  printWorkerID();
#endif

  //Extract and parse the enum task_action
  str = strtok(NULL, ",");
  task_action = static_cast<_task_action> (atoi(str));

  //Set the auxiliary Trace
  aux.setTrace(ts, idle, id, task_action);

  //Add to the JobTrace to the vector
  traces.push_back(aux);

  delete []cstr;
  
}
//This Function parses a struct timespec 
struct timespec SimulationFigure::parseTimespec(char *str){
  struct timespec aux;
  unsigned long long int ns, ns2;
  unsigned long int nsec=0;
  unsigned long int sec;
  unsigned long long int t;
 
  if (strlen(str)> 12){
    cout << "Timespec Parser ERROR , number is too big"<< endl;
    return TimeUtil::Millis(0);
  }
  else
  {
    t = atoi(str);
    ns =  t * (unsigned long long int) 1000;
    sec = static_cast <unsigned long int> (ns / 1000000000);
    ns2 = (sec*(unsigned long long int)1000000000);
    nsec = (unsigned long int)(ns - ns2);
    aux.tv_sec = sec;
    aux.tv_nsec = nsec;
  
    return aux;
  }
}

//This auxiliary function prints the imported worker IDs
void SimulationFigure::printWorkerID(){
 unsigned  int i; 
 for (i=0 ; i < worker_id.size();i++){
   cout << "WORKER ID "  <<i <<" is equal to " << worker_id[i]<< endl; 
   }
}

//Set the number of plots for the figure
void SimulationFigure::setNPlots(int n) {
  n_plots = n;
}

//Set the ranges of the figure
void SimulationFigure::setRanges(double x1, double x2, double y1, double y2) {
  this->x1 = x1;
  this->x2 = x2;
  this->y1 = y1;
  this->y2 = y2;
}

//Sets the size of the figure, as well as the title and x label
void SimulationFigure::setSize(int x, int y) {
  if (gr!=NULL) {
    gr->SetSize(x,y);
    gr->Label('x',"Time [ms]",0);
  }
}

//Set the x domain of the current plot and the corresponding ranges of the figure
void SimulationFigure::setTimeRange(double t) {
  this->x2 = t;
  setSize((int)ceil(t)*UNIT_WIDTH, PLOT_HEIGHT);
}

//This function sorts the worker_id vector
void SimulationFigure::sort_worker_id(){
	sort (worker_id.begin(), worker_id.end());

}

// This function produces a vector of worked IDs
void SimulationFigure::workerID_vector(unsigned int id){
 if (worker_id.size()==0)
       worker_id.push_back(id);
  else
  {
    if (find (worker_id.begin(), worker_id.end(), id)!=worker_id.end())
      return ;
    else
      worker_id.push_back(id);
  }
}


