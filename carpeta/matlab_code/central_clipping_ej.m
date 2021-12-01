clc
clear
close all

th_cc=0.03;

filename='sb050.wav';
[y,fs]=audioread(filename);
%sound(y,fs);    %play signal 'so many overwhelm me and I was move to tears'
ty=0:1/fs:(length(y)-1)*1/fs;
%y=sin(2*pi*ty*5);
figure
subplot(2,1,1);
plot(ty,y);
title(['Signal:', filename]);
xlabel('[s]');


y(abs(y(:))<th_cc)=0;    % turn to zero
subplot(2,1,2);
plot(ty,y);
title([filename,'  signal clipped, threshold used:', num2str(th_cc)]);
xlabel('[s]');