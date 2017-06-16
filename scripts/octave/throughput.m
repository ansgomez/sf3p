function throughput(name)

TASK_END=4;

name_traces = strcat(name,'_traces.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

traces = csvread(name_traces);

name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%calculates simulation time
simulationTime = sum (runTime(:,3));

%finds thread ids and sorts them
thread_ids=unique(traces(find(traces(:,2)~=0),2));
sorted_ids = sort(thread_ids);

%finds the number of jobs for each worker
for i=1:size(thread_ids,1);
  numJob(i) = size(find(traces(find(traces(:,2)==sorted_ids(i)),3)==TASK_END),1);
  throughput(i) =(( numJob(i)/ simulationTime)*10^6);
end

name_throughput = strcat(name,'_throughput.csv');
throughput = transpose(throughput);
csvwrite(name_throughput, throughput, 'precision', '%2.3f' );

clear all;




