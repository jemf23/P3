    %Read first column from the text file
    % Assigning the path
    %path = '\\wsl$\ubuntu-20.04\home\rookie\PAV\P3\matlab_code\'; %abc is your folder which contains the txt file
    % Assigning a variable to access the file
    %fileName = [path,'result_an_input_parameters.txt'];
    %fileName = 'result_an_input_parameters.txt'
    % Importing all the data from the txt file
    %fileEntireDataSet = importdata(FileName);
    % Assigning the first column only
    %dataFirstColumn = fileEntireDataSet.data(:,1)
    
    %%
     fileName = '/home/rookie/PAV/P3/results/result_an_input_parameters.txt';

    % Importing all the data from the txt file
    %load data from text file
    data=load(fileName); % assuming you only have data values(numbers)
    %result1=data(:,1); % and just extract the first column
    result1=data(1,1); % and just extract the first element
    
    
    
    
    clc
    clear
    close all
    data=load('result_an_input_parameters.txt'); % assuming you only have data values(numbers)
    result1=data(:,1) % and just extract the first column
    % or
    data=readtable('result_an_input_parameters.txt'); % requires 2013a or later
    result2=data(:,1) % table with one column which is the first column
    %data{:,1} % just numbers in the first column