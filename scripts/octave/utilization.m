function utilization(name)

name_traces = strcat(name,'_runtimes.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

runTime = csvread(name_traces);


%finds the idle thread
idle = find(runTime(:,1)==0);

% finds the runtime of idle thread
runT = runTime(idle,3);

%total simulation time
total = sum(runTime(:,3));

%calculates the utilization
utilization = (1-(runT /total))*100;

name_util = strcat(name,'_utilization.csv');
csvwrite(name_util, utilization, 'precision', '%3.6f');




