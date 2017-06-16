/*********************************
 *
 * Input parameter 1    - file prefix
 * Input parameter 2..N - metric to compute
 *
 * Metrics include:
 *
 * execution_times              (exec/exe)
 * response_times               (resp)
 * utilization                  (util)
 * resource_allocation_cost     (alloc)
 * system_implementation_cost   (sys)
 * worker_cost                  (worker)
 * throughput                   (throughput)
 * missed_deadlines             (missed)
 * -----------------------------------
 * All of the above             (all)
 *
 *********************************/

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

/*********** VARIABLES ***********/

///This variable holds the prefix for the input files (_traces, _runtimes, _missed_deadlines).
string prefix="simulation";

vector <string> inputMetric, inputPrefix;

const char *execVector[] = {"execution_times","exec","exe"};
vector <string> exec(execVector, (execVector)+sizeof(execVector)/sizeof(execVector[0]));

const char *respVector[] = {"response_times","resp"};
vector <string> resp(respVector, (respVector)+sizeof(respVector)/sizeof(respVector[0]));

const char *utilVector[] = {"utilization","util"};
vector <string> util(utilVector, (utilVector)+sizeof(utilVector)/sizeof(utilVector[0]));

const char *allocVector[] = {"resource_allocation_cost","alloc"};
vector <string> alloc(allocVector, (allocVector)+sizeof(allocVector)/sizeof(allocVector[0]));

const char *sysVector[] = {"system_cost","sys"};
vector <string> sys(sysVector, (sysVector)+sizeof(sysVector)/sizeof(sysVector[0]));

const char *workerVector[] = {"worker_cost","worker"};
vector <string> worker(workerVector, (workerVector)+sizeof(workerVector)/sizeof(workerVector[0]));

const char *allVector[] = {"all"};
vector <string> all(allVector, (allVector)+sizeof(allVector)/sizeof(allVector[0]));

const char *throughputVector[] = {"throughput"};
vector <string> throughput(throughputVector, (throughputVector)+sizeof(throughputVector)/sizeof(throughputVector[0]));

const char *missedVector[] = {"missed_deadlines", "missed","deadline"};
vector <string> missed(missedVector, (missedVector)+sizeof(missedVector)/sizeof(missedVector[0]));

/*********** FUNCTIONS ***********/

///This function interprets all input parameters
void interpret(string str);

///checks if the input string calls for all functions
bool isAll(string str);

///checks if the input string calls for execution function
bool isExecution(string str);

///checks if the input string calls for missedDeadlinesn function
bool isMissedDeadlines(string str);

////checks if the input string calls for resource allocation function
bool isResourceAllocationCost(string str);

////checks if the input string calls for ResponseTimes function
bool isResponseTimes(string str);

///checks if the input string calls for system cost function
bool isSystemCost(string str);

///checks if the input string calls for throughput function
bool isThroughput(string str);

///checks if the input string calls for utilization function
bool isUtilization(string str);

///checks if the input string calls for worker cost functio
bool isWorkerCost(string str);

///This function process all the imput metrices with all prefixes
void process();

///This function calls the show_execution_times script to produce "$prefix_execution_times.csv"
void showExecutionTimes();

///This function calls the showMissed deadllines script to produce "$prefix_missed_deadlines.csv"
void showMissedDeadlines();

///This function calls the showResource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void showResourceAllocationCost();

///This function calls the showResponse_times script to produce "$prefix_response_times.csv"
void showResponseTimes();

///This function calls the showSystemCost script to show "$prefix_System Cost"
void showSystemCost();

///This function calls the showThroughput script to show "$prefix_Throughput"
void showThroughput();

///This function calls the show_Utilization script to produce "$prefix_Utilization.csv"
void showUtilization();

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void showWorkerCost();


/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  string* aux;
#if _INFO==1
  cout << "Starting SF3P's show tool\n";
#endif

  ///If there are input pArameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      if(argv[i] == NULL)
	continue;

      aux = new string(argv[i]);
      interpret(*aux);
    }

    if(inputMetric.size() == 0)
      inputMetric.push_back("all");
    if(inputPrefix.size() == 0)
      inputPrefix.push_back("simulation");
   
    cout << endl;

    process ();
  }   
  ///otherwise analyze all by default, 
  else {
    cout << "\n***   Showing all metrics!\t***\n\n";
    showExecutionTimes();
    showResponseTimes();
    showThroughput();
    showUtilization();
    showResourceAllocationCost();
    showSystemCost();
    showWorkerCost();
    showMissedDeadlines();
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}


///This function calls the show_execution_times script to show "$prefix_execution_times"
void showExecutionTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the show_missed_deadllines script to produce "$prefix_missed_deadlines.csv"
void showMissedDeadlines(){
  string cmd = "octave --no-window-system -q --eval \"show_missed_deadlines('" + prefix + "')\"";

  system(cmd.c_str());
}
///This function calls the showResourceAllocationCost script to show "$prefix_Resource Allocation Cost"
void  showResourceAllocationCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the show_response_times sCript to show "$prefix_response_times"
void showResponseTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_response_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showSystemCost script to show "$prefix_System Cost"
void  showSystemCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showThroughput script to show "$prefix_Throughput"
void showThroughput(){

  string cmd = "octave --no-window-system -q --eval \"show_throughput('" + prefix + "')\"";

  system(cmd.c_str());

}

///This function calls the show_utilization script to show "$prefix_utilization"
void showUtilization() {
  string cmd = "octave --no-window-system -q --eval \"show_utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void  showWorkerCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_workerCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function interprets all input parameters
void interpret(string str) {

  transform(str.begin(),str.end(),str.begin(),::tolower);
  if (isExecution( str)) 
    inputMetric.push_back(str);
  else if(isResponseTimes( str))
    inputMetric.push_back(str);
  else if(isUtilization( str))
    inputMetric.push_back(str);
  else if(isResourceAllocationCost( str))
    inputMetric.push_back(str);
  else if(isSystemCost( str))
    inputMetric.push_back(str);
  else if(isWorkerCost( str))
    inputMetric.push_back(str);
  else if(isAll( str))
    inputMetric.push_back(str);
  else if(isThroughput( str))
    inputMetric.push_back(str);
  else if(isMissedDeadlines(str))
    inputMetric.push_back(str);
  else
    inputPrefix.push_back(str);
}
///checks if the input string calls for all functions
bool isAll(string str){
   if (find (all.begin(), all.end(), str)!=all.end())
      return true;
  else
      return false;
}

///checks if the input string calls for execution function
bool isExecution(string str){
  if (find (exec.begin(), exec.end(), str)!=exec.end())
      return true;
  else
      return false;
}

///checks if the input string calls for missedDeadlinesn function
bool isMissedDeadlines(string str){
  if (find (missed.begin(), missed.end(), str)!=missed.end())
      return true;
  else
      return false;
}

///checks if the input string calls for resource allocation function
bool isResourceAllocationCost(string str){
  if (find (alloc.begin(), alloc.end(), str)!=alloc.end())
      return true;
  else
      return false;
}

///checks if the input string calls for ResponseTimes function
bool isResponseTimes(string str){
  //cout << "RESPONSE \n";
  if (find (resp.begin(), resp.end(), str)!=resp.end())
      return true;
  else
      return false;
}

///checks if the input string calls for system cost function
bool isSystemCost(string str){
   if (find (sys.begin(), sys.end(), str)!=sys.end())
      return true;
  else
      return false;
}

///checks if the input string calls for throughput function
bool isThroughput(string str){
   if (find (throughput.begin(), throughput.end(), str)!=throughput.end())
      return true;
  else
      return false;
}

///checks if the input string calls for utilization function
bool isUtilization(string str){
  if (find (util.begin(), util.end(), str)!=util.end())
      return true;
  else
      return false;
}

///checks if the input string calls for worker cost function
bool isWorkerCost(string str){
   if (find (worker.begin(), worker.end(), str)!=worker.end())
      return true;
  else
      return false;
}

///This function process 
void process (){
  unsigned int i,j;
 
   for (i=0; i< inputPrefix.size(); i++){
     prefix = inputPrefix[i];
     cout << "+++   Simulation: " << prefix << "\t+++\n\n";
     for (j=0; j< inputMetric.size() ; j++){

       if( isExecution(inputMetric[j]) )
	 showExecutionTimes();
       if ( isResponseTimes(inputMetric[j]) )
	 showResponseTimes();
       if ( isUtilization(inputMetric[j]) )
	 showUtilization();
       if ( isResourceAllocationCost(inputMetric[j]) )
	 showResourceAllocationCost();
       if ( isSystemCost(inputMetric[j]) )
         showSystemCost();
       if ( isWorkerCost(inputMetric[j]) )
	 showWorkerCost();
       if ( isThroughput(inputMetric[j]) )
	 showThroughput();
       if ( isMissedDeadlines(inputMetric[j]) )
	 showMissedDeadlines();
       if ( isAll(inputMetric[j]) ){
	 cout << "***   Showing all metrics!\t***\n\n";
	 showExecutionTimes();
	 showResponseTimes();
	 showThroughput();
	 showUtilization();
	 showResourceAllocationCost();
	 showSystemCost();
	 showWorkerCost();
	 showMissedDeadlines();
	 
       }
     }
   }
}
