function c = count(r)
    c=0;    
    
    if length(r) >= 1	
        for i1=1:length(r)
            if r(i1) ~= 0
                 c = c+1;	
            end 
            
            if r(i1) == 0
            	break;
            end
        end
     end
end