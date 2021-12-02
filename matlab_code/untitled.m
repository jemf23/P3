% bashcmd = 'D:\cygwin64\bin\bash.exe -c ';
% bashcmd = 'D:\MinGW\msys\1.0\bin\bash.exe -c ';
%system('wsl')

system('U:\home\rookie\PAV\P3\scripts\run_get_pitch.sh')
system('U:\home\rookie\PAV\P3\matlab_code\first_script.ps1 &')
system('wsl run_get_pitch.sh &')
system('cd &')
system('cd PAV &')
system('cd P3 &')
system('ls &')



bashcmd = 'bash -c '; % WSL
cmd = 'run_get_pitch.sh';
pwd='U:\home\rookie\PAV\P3\matlab_code';
runcmd = ['"' cmd '"'];
status = system([bashcmd runcmd]);
if (status ~= 0)
    error(['Run bash cmd error: ' num2str(status)]);
end




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