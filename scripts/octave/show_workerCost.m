function show_workerCost(name)

name_workerCost = strcat(name,'_workerCost.csv');

if exist(name_workerCost, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_workerCost);
  return;
end

workerCost = csvread(name_workerCost);

fprintf('  Worker Cost : \n\n');
fprintf('\tID     RATIO     \n');
for i=1:size(workerCost);
fprintf ('\t%2d   %7.3f \n' ,i , workerCost(i) );
end
