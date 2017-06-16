function show_throughput(name)
              
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

simulationTime = sum (runTime(:,3));

thread_ids=unique(traces(find(traces(:,2)~=0),2));
sorted_ids = sort(thread_ids);
[id_size col]=size(thread_ids);


for i=1:id_size;
[numJob(i) col]= size(find(traces(find(traces(:,2)==sorted_ids(i)),3)==4));
throughput(i) =(( numJob(i)/ simulationTime)*10^6);
end
throughput = throughput';

fprintf('  Throughput(Job/s) : \n\n');
fprintf('\tID    Throughput          \n');
for i=1:size(throughput);
fprintf ('\t%2d     %7.3f \n' ,i , throughput(i) );
end
