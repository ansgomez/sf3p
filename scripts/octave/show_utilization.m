function show_utilization(name)

name_traces = strcat(name,'_utilization.csv');

if exist(name_traces, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_traces);
  return;
end

utilization = csvread(name_traces);

fprintf('  Utilization : \n\n');
fprintf ('\tUtilization is %7.3f%% \n' ,utilization );
