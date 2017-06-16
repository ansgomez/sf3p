function workerCost(name)

%CONSTANTS
SCHED_START=1;
SCHED_END=2;
TASK_START = 3;
TASK_END = 4;

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

aux=0;

thread_ids=unique(traces(find(traces(:,2)~=0),2));
num_threads = length(thread_ids);

%for each thread
for i1=1:num_threads;
	
    c=1;
    
    t_id = find(traces(:,2)==thread_ids(i1));
    
    aux_count = 0;
    aux_began=0;
    aux_start=-1;

    %foreach trace
    for i2=1:length(t_id)
        if traces(t_id(i2), 3) == TASK_END
             exec_ms(c, i1) = aux_count + (traces(t_id(i2), 1)-aux_start)/1000;
             aux_start = -1;	
             aux_count = 0;
             aux_began = 0;
             c=c+1;
        elseif traces(t_id(i2), 3) == SCHED_START

             if aux_began == 1
                 aux_start = traces(t_id(i2), 1);	
             end

        elseif traces(t_id(i2), 3) == TASK_START

             aux_start = traces(t_id(i2), 1);
             aux_began = 1;

        elseif traces(t_id(i2), 3) == SCHED_END

             if  aux_start ~= -1
                  aux_count =   aux_count  + (traces(t_id(i2), 1)-aux_start)/1000;
             end

        end

	if i2 == length(t_id) && (traces(t_id(i2), 3) == SCHED_START || traces(t_id(i2), 3) == TASK_START)
            exec_ms(c, i1) = aux_count + (traces(end, 1)-aux_start)/1000;
            aux_start = -1;	
            aux_count = 0;
            aux_began = 0;
            c=c+1;
	end

	if i2 == length(t_id) && traces(t_id(i2), 3) == SCHED_END
            exec_ms(c, i1) = aux_count;
            aux_start = -1;	
            aux_count = 0;
            aux_began = 0;
            c=c+1;
	end
    end

    if c==1
      exec_ms(1,i1)=0;
      N=0;
      continue;
    end
end

[num_job , num_worker] = size(exec_ms);

%calculates the matrix of workers execution time from the trace file
for i=1:num_worker;
  sum_exec_traces(i) = sum(exec_ms(:,i));
end

%finds the indecis of workers
worker = find(runTime(:,1)==4);

%finds the workers ID
w_id = runTime(worker,2);

%sorts the workers ID
sorted_id = sort(w_id);

%calculates the sum of execution runtime for each worker
for i=1:size(w_id)
  sum_exec_runtime(i)= runTime(find(runTime(:,2)==sorted_id(i)),3);
end

%calculates the ratio of 2 execution times
ratio = sum_exec_traces ./(sum_exec_runtime*0.001);
ratio = ratio'; #'

name_workerCost = strcat(name,'_workerCost.csv');
csvwrite(name_workerCost, ratio, 'precision', '%2.3f' );

%calculates the difference of 2 execution times
difference = sum_exec_traces - (sum_exec_runtime*0.001);
difference = difference';

if exist("slot.txt", "file") ~= 0
    slot = csvread('slot.txt');
    n = csvread('n.txt');
    %save the normalized cs approximation
    name_workerCSApprox = strcat(name,'_workerCSApprox.csv');
    % formula is really sum(diff)*(slot/(n*WCET*2*repetitions))
    csvwrite(name_workerCSApprox, sum(difference)*(slot/(n*50*20)), 'precision', '%2.3f' );

    name_workerCSTotal = strcat(name,'_workerCSTotal.csv');
    csvwrite(name_workerCSTotal, sum(difference), 'precision', '%2.3f' );
end
