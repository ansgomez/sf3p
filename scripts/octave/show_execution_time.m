function show_execution_time(name)

name_exec = strcat(name,'_exec_ms.csv');

if exist(name_exec, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_exec);
  return;
end

exec_ms = csvread(name_exec);

[num_job , num_worker] = size(exec_ms);

fprintf('  Execution Times (ms): \n\n');
fprintf('\tID    N_Jobs      MIN        AVG        MAX      TOTAL\n');

%for each worker
for i=1:num_worker;

  total=sum(exec_ms(:,i));

  if total==0
    min_i = 0;
    avg = 0;
    max_i = 0;
    num_job = 0;
  else
    min_i = minimum(exec_ms(:,i));
    avg = average(exec_ms(:,i));
    max_i  =  max(exec_ms(:,i));
    num_job = count(exec_ms(:,i));
  end

  %output
  fprintf('\t%2d    %3d      %7.3f    %7.3f    %7.3f    %7.3f \n', i, num_job,  min_i , avg, max_i, total);
end
