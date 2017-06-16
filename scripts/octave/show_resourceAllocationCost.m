function show_resourceAllocationCost(name)

name_runtimes = strcat(name,'_alloc_cost_us.csv' );

if exist(name_runtimes, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_runtimes);
  return;
end

alloc_cost_us =  csvread(name_runtimes);

fprintf('  Resource Allocation Cost : \n\n');
fprintf ('\tResource Allocation Cost is  %7.3f (us/s)\n' ,alloc_cost_us );
