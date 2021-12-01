clc
clear
close all

filename='sb050.wav';
[y,fs]=audioread(filename);
sound(y,fs);    %play signal 'so many overwhelm me and I was move to tears'

ty=0:1/fs:(length(y)-1)*1/fs;
figure
plot(ty,y);
title(['Signal:', filename]);
xlabel('[s]')

tstart=1.11; %1.11 seconds, A vowel of the word mAny
Nstart=tstart*fs;   %
tmax=tstart+0.03;  %30ms
N=tmax*fs;  %sample

x=y(Nstart:N);  %cut signal
sound(x,fs);    %play new signal
t=linspace(tstart,tmax,numel(x));  %time index of new signal


cxx=xcorr(x);   %autocorrelation

figure
subplot(2,1,1);
plot(t,x);
title(['30ms of the vowel A, filename: ',filename]);
xlabel('[s]');

subplot(2,1,2);
c0=round(length(cxx)/2);
plot(cxx(c0:end))
title('Autocorrelation');
%plot important values
str = {['r[0]=', num2str(cxx(c0)),'  r[1]=',num2str(cxx(c0+1)),'  r[lag=86]=',num2str(cxx(c0+86))]};
text(290,1.25,str,'Color','blue')

%% pitch value
lag=86;
pitch1=1/lag*fs

%% find pitch using pitch() function of Audio toolbox
%pitch() default window is round(fs*0.052)=52 ms
tstart=1.109; %1.11 seconds, A vowel of the word MANY
Nstart=tstart*fs;   %
tmax=tstart+0.052;  %30ms
N=tmax*fs;  %sample

x2=y(Nstart:N);  %cut signal
f0 = pitch(x2,fs)