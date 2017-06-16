function missed_deadlines(name)

%CONSTANTS
SCHED_START=1;
SCHED_END=2;
TASK_START = 3;
TASK_END = 4;

WORKER = 4;

name_traces = strcat(name,'_traces.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

traces = csvread(name_traces);

name_missed = strcat(name,'_missed_deadlines.csv');

if exist(name_missed, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_missed);
  return;
end

[s, err, msg] = lstat(name_missed);

if(s.size == 0)
  return;
end;

missed = csvread(name_missed);

if (size(missed)==0)
  return;
end


name_runtimes = strcat(name,'_runtimes.csv');

if exist(name_runtimes, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_runtimes);
  return;
end

runTime = csvread(name_runtimes);

%find workers ids
worker = sort (runTime(find(runTime(:,1)==WORKER),2));

%find thread ids and sorts them
thread_ids=unique(traces(find(traces(:,2)~=0),2));
sorted_ids = sort(thread_ids);

ids_missed = unique(missed(:,2));
sorted_missed = sort (ids_missed);

total_jobs = 0;
total_missed = 0;
%find the number of jobs and missed deadlines for each worker
for i=1:size(thread_ids,1);
  numJob(i) = size(find(traces(find(traces(:,2)==sorted_ids(i)),3)==TASK_END),1);
  total_jobs = total_jobs+numJob(i);

  numJob_missed(i) = size((find(missed(:,1)==sorted_ids(i))),1);
  if numJob(i) == 0
    missed_percentage(i) = 0;
  else
    missed_percentage(i) = (numJob_missed(i)/numJob(i));
    total_missed = total_missed + numJob_missed(i);
  end
end

for i=1:size(thread_ids,1);
  tardiness(i) =(average(missed(find(missed(:,1)==sorted_ids(i)),4))) - (average(missed(find(missed(:,1)==sorted_ids(i)),3)));
  worker_id(i) = find(sorted_ids(i)==worker);
end

matrix = [worker_id ; missed_percentage ; tardiness];
deadlines_matrix =transpose(matrix);

%multiply to get in percentage
deadlines_matrix(:,2) = deadlines_matrix(:,2)*100;
%divide to get millis
deadlines_matrix(:,3) = deadlines_matrix(:,3)/1000;

name_missed_deadlines = strcat(name,'_deadline_metrics.csv');
csvwrite(name_missed_deadlines, deadlines_matrix , 'precision', '%2.3f');

total_deadlines = (total_missed/total_jobs)*100;
name_total_deadlines = strcat(name,'_deadline_total.csv');
csvwrite(name_total_deadlines, total_deadlines, 'precision', '%2.3f');

clear all;
