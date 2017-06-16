#ifndef _SIMULATIONFIGURE_H
#define _SIMULATIONFIGURE_H

#include "mgl2/eval.h"
#include "mgl2/mgl.h"

#include "util/Enumerations.h"
#include "results/Trace.h"

#include <locale.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>


#if !defined(_MSC_VER) && !defined(__BORLANDC__)
#include <getopt.h>
#endif

using namespace std;

#define MAX_PLOTS    10  //Max number of column plot in figure
#define PLOT_HEIGHT  500 //Vert. Pixels per plot in picture
#define UNIT_WIDTH   5 //Hor. Pixels per time unit in picture

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class SimulationFigure {
     
 private:

  /*********** VARIABLES ***********/
  
  //Relative length of arrow (w.r.t. height)
  double      arrow_length;    

  //Vertical offset from x-axis
  double      base;            

  //Height of all job traces   
  double      height;          
  
  mglGraph   *gr;

  //Number of column plots in the figure
  int         n_plots;         

  //Holds the prefix to the _traces.csv file
  string      prefix;

  //Distance b/w colplots. in % of height
  double      plot_offset;    
  
  // Vector of Job trace
  vector<Trace> traces;
  
  //Width of one time unit
  double      width;          

  // voctor of work ids
  vector <unsigned int> worker_id;

  //Figure ranges
  double      x1, x2, y1, y2;  

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs the prefix to the '_traces.csv' file
  SimulationFigure(string _prefix);

  /*********** MEMBER FUNCTIONS ***********/

  //This function draws an arrival arrow at time t, with an option label
  void drawArrival(int plot, double time, int color=0);  

  //This function draws a rectangle of a defined color starting at time t and a relative width
  void drawJob(int plot, double start, double end, int clr=-1);

  //This function draw a green downward arrow at the specified time to indicate a met deadline
  void drawMetDeadline(int plot, double time);

  //This function draw a green downward arrow at the specified time to indicate a met deadline
  void drawMissedDeadline(int plot, double time);

  //This function draws an arrow indicating that the scheduler activated plot_i during this time
  void drawSched(int plot, double sched_start, double sched_end, int clr=0);

  //This function exports the mgl graph to an eps file
  void exportEPS();

  //This function exports the mgl graph to a png file
  void exportPNG();

  //This function exports the mgl graph to an svg file
  void exportSVG();

  ///This function generates a figure from the trace vector
  void genFig();

  ///This function first generates a figure in memory (by calling genFig()) and automatically exports them to EPS and SVG file formats
  void generateFigures();

  ///This function imports data from a CSV file with the previously set prefix
  void importCSV();

  //Initialize a plot with y-label and default settings (ticks, ranges, etc)
  void initPlot(int plot, char *name);

  ///This function parses one line from a CSV file
  void parseLine(string line);

  //This Function parses a struct timespec
  struct timespec parseTimespec(char *str);

  //This auxiliary function prints the imported worker IDs
  void printWorkerID();

  //Set the number of plots for the figure
  void setNPlots(int n);

  //Set the ranges of the figure
  void setRanges(double x1, double x2, double y1, double y2);

  //Sets the size of the figure, as well as the title and x labelx
  void setSize(int x, int y);

  //Set the x domain of the current plot and the corresponding ranges of the figure
  void setTimeRange(double t); 

  //This function sorts the worker_id vector
  void sort_worker_id();

  // This function produces a vector of worked IDs 
  void workerID_vector(unsigned int id);
};

#endif
