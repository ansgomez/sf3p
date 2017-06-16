function x = roundn(x, n)
% Round to multiple of n (normally 10, or 5)
x = n*round(x/n);
