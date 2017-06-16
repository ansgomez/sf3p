function response_time ( name )

%CONSTANTS
TASK_ARRIVAL = 0;
TASK_END = 4;

name_traces = strcat(name,'_traces.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

traces = csvread(name_traces);

thread_ids=unique(traces(find(traces(:,2)~=0),2));
num_threads = length(thread_ids);

actions=traces(:,3);

for i1=1:num_threads
	
    c=1;
    
    t_id = thread_ids(i1);
    
    %find finish and start times for thread i1
    end_times = intersect(find(traces(:,2)==t_id), find(actions==TASK_END));
    arrival_times = intersect(find(traces(:,2)==t_id), find(actions==TASK_ARRIVAL));
    
    %for each ith end time, subtract the ith start time and /1000 to get millis
    for i2=1:length(end_times)
        %R=end-start
        aux = traces(end_times(i2), 1)-traces(arrival_times(i2), 1);
        resp_ms(c, i1) = aux/1000;
        c=c+1;
    end
    
    if c==1
      resp_ms(1,i1)=0;
      N=0;
      continue;
    end

end

name_resp = strcat(name,'_resp_ms.csv');
csvwrite(name_resp, resp_ms, 'precision', '%2.3f');

clear all;
