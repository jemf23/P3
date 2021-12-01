#include <math.h>
#include "pav_analysis.h"
#include <stdio.h>

float compute_power(const float *x, unsigned int N) {
    int i=0;
	float power=1.e-9;
	for(i=0;i<N;i++) {
		power+=(x[i]*x[i]);
/*
		power=power+pow(x[i],2);
*/
	}
	power=10*log10(((float)1/N)*power);/*1.0f*/
	//printf("Potencia media de la señal en decibelios: %.2f /n",power);
	return power;
}

float compute_am(const float *x, unsigned int N) {
	float am=0;
	int i=0;
	for(i=0;i<N;i++) {
		am+=fabs(x[i]); /*fabs abs for float*/
	}
	am=((float)1/N)*am;
	//printf("Amplitud media de la señal: %.2f /n",am);
	return am;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
	int i=0;
	float zcr=0;
	for(i=1;i<N;i++){
		if((x[i]*x[i-1])<0){
			zcr+=1;
		}
	}
	zcr=((float)1/(N-1))*zcr;
	zcr=fm/((float)2)*zcr;
	//printf("Tasa de cruzes por cero de la señal: %.2f /n",zcr);
	return zcr;
}

float compute_powerhamm(const float *x, unsigned int N) {
	float p=0;
	float s1=0;
	float s2=0;
	float w=0;
	for (int i = 0; i < N; i++) {
		//ventana hamming: https://en.wikipedia.org/wiki/Window_function#Hamming_window
        w = 0.53836-0.46164*cos(2*M_PI*i/N);
        s1+=pow(x[i]*w,2);
		s2+=pow(w,2);
    }
	p=s1/s2;
    return p=10*log10(p);
}