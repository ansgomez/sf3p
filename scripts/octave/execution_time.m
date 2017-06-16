function execution_time(name)

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

    min_i = minimum(exec_ms(:,i1));
    avg = average(exec_ms(:,i1));
    max_i  =  max(exec_ms(:,i1)) ;
    total =  sum(exec_ms(:,i1));
    
end

name_exec = strcat(name,'_exec_ms.csv');
csvwrite(name_exec, exec_ms, 'precision', '%2.3f');

clear all;
