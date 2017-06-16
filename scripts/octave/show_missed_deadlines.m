function show_missed_deadlines(name)
name_missed = strcat(name,'_deadline_metrics.csv');
missed_file = strcat(name,'_missed_deadlines.csv');

[s, err, msg] = lstat(name_missed);

if(size(s) == 0)
  fprintf('  Missed Deadlines: \n\n');
  fprintf("\tNo Missed Deadlines \n");
  return;
end;

if (size(csvread(missed_file))==0)
  fprintf('  Missed Deadlines: \n\n');
  fprintf("\tNo Missed Deadlines \n");
  return;
end


if exist(name_missed, "file") == 0
  fprintf("\n   File: '%s' was not found!\n", name_missed);
  return;
end

missed = missed = csvread(name_missed);

fprintf('  Missed Deadlines: \n\n');
fprintf('\tID    Missed(%%)      Avg. Tardiness(ms) \n');

for i=1:size(missed,1);
fprintf ('\t%2d     %3.3f             %7.3f \n' ,missed(i,1), missed(i,2), missed(i,3));
end
