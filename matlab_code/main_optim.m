    clc
    clear
    close all
%options = optimset('PlotFcns',@optimplotfval);
%fun = @(x)100*(x(2) - x(1)^2)^2 + (1 - x(1))^2; 
%x0 = [-1.2,1]; 
%x = fminsearch(fun,x0,options);

%xo input parameters 
x0 = [0.25,-0.25];


x = fminsearch(@black_box_function,x0);