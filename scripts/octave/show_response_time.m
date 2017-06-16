function show_response_time ( name )

name_resp = strcat(name,'_resp_ms.csv');

if exist(name_resp, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_resp);
  return;
end

resp_ms = csvread(name_resp);

[num_job , num_worker] = size(resp_ms);

fprintf('  Response Times (ms): \n\n');
fprintf('\tID    N_Jobs      MIN        AVG        MAX      TOTAL\n');

%for each worker
for i=1:num_worker;

  total = sum(resp_ms(:,i));

  if total==0
    min_i = 0;
    avg = 0;
    max_i = 0;
    num_job = 0;
  else
    min_i = minimum(resp_ms(:,i));
    avg = average(resp_ms(:,i));
    max_i  =  max(resp_ms(:,i));
    num_job = count(resp_ms(:,i));
  end

  %output
  fprintf('\t%2d    %3d      %7.3f    %7.3f    %7.3f    %7.3f \n', i, num_job,  min_i , avg, max_i, total);
end
