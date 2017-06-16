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

///This variable holds the prefix for the input files (_traces, _runtimes, _missedDeadlines).
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

const char *missedVector[] = {"missed_deadline", "missed","deadline"};
vector <string> missed(missedVector, (missedVector)+sizeof(missedVector)/sizeof(missedVector[0]));



/*********** FUNCTIONS ***********/

///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void calculateExecutionTimes();

///This function calls the missed deadllines script to produce "$prefix_missed_deadlines.csv"
void calculateMissedDeadlines();

///This function calls the Resource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void calculateResourceAllocationCost();

///This function calls the response_times script to produce "$prefix_response_times.csv"
void calculateResponseTimes();

///This function calls the SystemCost script to produce "$prefix_System Cost"
void calculateSystemCost();

///This function calls the throughput script to produce "$prefix_Throughput"
void calculateThroughput();

///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void calculateUtilization();

////This function calls the WorkerCost script to produce "$prefix_Worker Cost"
void calculateWorkerCost();

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


/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  string* aux;

#if _INFO==1
  cout << "Starting SF3P's calculate tool\n";
#endif

  ///If there are input parameters, interpret them
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
  ///otherwise calculate all by default, 
  else {
    cout << "\n***   Calculating all metrics!\t***\n\n";
    calculateExecutionTimes();
    calculateResponseTimes();
    calculateThroughput();
    calculateUtilization();
    calculateResourceAllocationCost();
    calculateSystemCost();
    calculateWorkerCost();
    calculateMissedDeadlines();
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}


///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void calculateExecutionTimes() {
  cout << "Calculating Execution Times...\n";

  string cmd = "octave --no-window-system -q --eval \"execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the missed deadllines script to produce "$prefix_missed_deadlines.csv"
void calculateMissedDeadlines(){
  cout << "Calculating Missed Deadlines...\n";

  string cmd = "octave --no-window-system -q --eval \"missed_deadlines('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the ResourceAllocationCost script to produce "$prefix_alloc_cost_us.csv"
void  calculateResourceAllocationCost(){
  cout << "Calculating Resource Allocation Cost...\n";

  string cmd = "octave --no-window-system -q --eval \"resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the response_times script to produce "$prefix_response_times.csv"
void calculateResponseTimes() {
  cout << "Calculating Response Times...\n";

  string cmd = "octave --no-window-system -q --eval \"response_time('" + prefix + "')\"";
  
  system(cmd.c_str());
}
///This function calls the SystemCost script to show "$prefix_System Cost"
void  calculateSystemCost(){
  cout << "Calculating System Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the throughput script to produce "$prefix_Throughput"
void calculateThroughput(){
cout << "Calculating Throughput...\n";
  string cmd = "octave --no-window-system -q --eval \"throughput('" + prefix + "')\"";

  system(cmd.c_str());

}
///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void calculateUtilization(){
  cout << "Calculating Utilization...\n";

  string cmd = "octave --no-window-system -q --eval \"utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

////This function calls the WorkerCost script to show "$prefix_Worker Cost"
void calculateWorkerCost(){
  cout << "Calculating Worker Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"workerCost('" + prefix + "')\"";

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
	 calculateExecutionTimes();
       if ( isResponseTimes(inputMetric[j]) )
	 calculateResponseTimes();
       if ( isUtilization(inputMetric[j]) )
	 calculateUtilization();
       if ( isResourceAllocationCost(inputMetric[j]) )
	 calculateResourceAllocationCost();
       if ( isSystemCost(inputMetric[j]) )
	 calculateSystemCost();
       if ( isWorkerCost(inputMetric[j]) )
         calculateWorkerCost();
       if ( isThroughput(inputMetric[j]) )
	 calculateThroughput();
       if ( isMissedDeadlines(inputMetric[j]) )
	 calculateMissedDeadlines();
       if ( isAll(inputMetric[j]) ){
	 cout << "***   Calculating all metrics!\t***\n\n";
	 calculateExecutionTimes();
	 calculateResponseTimes();
	 calculateThroughput();
	 calculateUtilization();
	 calculateResourceAllocationCost();
	 calculateSystemCost();
	 calculateWorkerCost();
	 calculateMissedDeadlines();
       }
     }
   }
}


