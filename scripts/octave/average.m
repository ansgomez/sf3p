function avg = average(r)
    
    if length(r) == 0
       avg = -1;
       return;
    end 
            
     if length(r) > 1	
        for i1=1:length(r)
            if r(i1) == 0.0
	break;
            end 
        end
     else
        i1=2;
     end
     
     avg = mean(r(1:i1-1));
end