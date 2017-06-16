function min = minimum(r)
    
    if length(r) == 0 
       min = -1;
       return;
   end
    
    min = r(1);
    
    if length(r) > 1	
        for i1=2:length(r)
            if r(i1) < min && r(i1) ~= 0
                 min = r(i1);	
            end 
            
            if r(i1) == 0
            	break;
            end
        end
     end
end