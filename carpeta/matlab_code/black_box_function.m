function result1 = black_box_function(inputParameters)
    %FUNCTION Summary of this function goes here
    %   Detailed explanation goes here

    %% Call main c++ program with inputParameters, run_get_pitch
    
    %https://es.mathworks.com/matlabcentral/answers/157199-run-a-bash-script-from-matlab
    %system('sh /home/rookie/PAV/P3/run_get_pitch.sh')
    
    %fullfile('myfolder','mysubfolder','myfile.m')
    %pwd = '/home/rookie/PAV/P3/scripts';
    !bash 'U:\home\rookie\PAV\P3\scripts\run_get_pitch.sh'
    pwd = '\\wsl$\Ubuntu-20.04\home\rookie\PAV\P3\scripts';
    %pwd = 'U:\home\rookie\PAV\P3\scripts';
    
    pathToScript = fullfile(pwd,'run_get_pitch.sh');   
    %inputParameters=[0.25,-0.25];
    %cmd= num2str(inputParameters);
    
    %cmdStr = sprintf('%s %.2f %.2f', pathToScript, inputParameters(1), inputParameters(2) );
    cmdStr = sprintf('%s', pathToScript);
    
    !bash -c run_get_pitch.sh
    status=system(cmdStr);
    startcommand =system ('wsl U:\home\rookie\PAV\P3\scripts\run_get_pitch.sh')
    
    %run without inputParameters
    %status=system('/home/rookie/PAV/P3/run_get_pitch.sh')
    
    %check for errors
    if (status ~= 0)
        error(['Run bash cmd error: ' num2str(status)]);
    end


    %% Call shell script, pitch_evaluate, that does the evaluation and save total result on a text file
    
    % now pitch_evaluate is called at the end of run_get_pitch and the
    % result is saved at ./results/result_an_input_parameters.txt
    
    %% Read first column from the text file
    % Assigning the path
    %path = '\\wsl$\ubuntu-20.04\home\rookie\PAV\P3\matlab_code'; %abc is your folder which contains the txt file
    % Assigning a variable to access the file
    %fileName = [path,'result_an_input_parameters.txt'];
    fileName = './results/result_an_input_parameters.txt';
    
    % Importing all the data from the txt file
    %load data from text file
    data=load(fileName); % assuming you only have data values(numbers)
    %result1=data(:,1); % and just extract the first column
    result1=data(1,1); % and just extract the first element
    
    %result1=100-data(1,1); %min search
    end

