function systemCost(name)

IDLE=0;
SCHED=1;
DISP=2;
SERV=3;

name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%finds the indeces of the disp, sched and serv threads
dispatcher =  find(runTime(:,1)==DISP);
scheduler  =  find(runTime(:,1)==SCHED);
server     =  find(runTime(:,1)==SERV);

%finds the total runtimes of disp, sched and serv threads
runT_dispatcher = sum(runTime(dispatcher,3));
runT_scheduler  = sum(runTime(scheduler,3));
runT_server     = sum(runTime(server,3));
 

%total simulation time
total = sum(runTime(:,3));

%calculates the resource Allocation Cost
sys_cost_us = ((runT_dispatcher + runT_scheduler + runT_server)*(10^6) /total );

name_sys_cost = strcat(name,'_sys_cost_us.csv');
csvwrite(name_sys_cost, sys_cost_us, 'precision', '%2.3f');
