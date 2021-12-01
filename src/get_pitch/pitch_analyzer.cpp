/// @file

#include <iostream>
#include <math.h>
#include "pitch_analyzer.h"

using namespace std;

/// Name space of UPC
namespace upc {
  void PitchAnalyzer::autocorrelation(const vector<float> &x, vector<float> &r) const {

    for (unsigned int l = 0; l < r.size(); ++l) {
  		/// \TODO Compute the autocorrelation r[l]
      /// \DONE Albino nos ha chivado el calculo de la autocorrelacion.
      r[l]=0;
      for (unsigned int n = l; n<x.size(); n++){
        r[l] += x[n-l]*x[n];
      }
    }

    if (r[0] == 0.0F) //to avoid log() and divide zero 
      r[0] = 1e-10; 
  }

  void PitchAnalyzer::set_window(Window win_type) {
    if (frameLen == 0)
      return;

    window.resize(frameLen);

    switch (win_type) {
    case HAMMING:{
      /// \TODO Implement the Hamming window
      float a0=0.53836;
      //double pi=3.141592653589793;
      for(unsigned int i = 0; i < frameLen; i++){
        window[i]=a0-(1-a0)*cos((2*M_PI*i)/frameLen);
      }
      /// \DONE Ventana de Hamming implementada
    
      break;
    }
    case RECT:{
      window.assign(frameLen, 1);
      break;
    }
    default:
      window.assign(frameLen, 1);
    }
  }

  void PitchAnalyzer::set_f0_range(float min_F0, float max_F0) {
    npitch_min = (unsigned int) samplingFreq/max_F0;
    if (npitch_min < 2)
      npitch_min = 2;  // samplingFreq/2

    npitch_max = 1 + (unsigned int) samplingFreq/min_F0;

    //frameLen should include at least 2*T0
    if (npitch_max > frameLen/2)
      npitch_max = frameLen/2;
  }

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm, float zcr) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.

    /// \DONE Implementado en clase (15/11/2021)

    //CON CENTER CLIPPING, COTAS AJUSTADAS 90.77% 
    //float threshold1=0.5;
    //float threshold2=0.90;
    //float threshold3=0;
    //normpot=pot/maxPot;
    //if (zcr > threshold4)

    //Unvoice decision
    //if( r1norm < threshold1 || rmaxnorm < threshold2 || pot < threshold3  || zcr > threshold4){
    //  return true;
    if(/*pot < threshold3 ||*/ rmaxnorm < 0.25 || r1norm < 0.60){
      return true;
    }else if(r1norm < 0.89 && rmaxnorm < 0.42){
      return true;
    }else if(zcr > threshold4 && pot < -52){ 
      return true;
    }else{
      //voice decision
      return false;
    }
    
    /*
    if(pot > -28){ 
      if(r1norm > 0.99 && rmaxnorm < 0.55){
        return true;
      }
      else if(r1norm > 0.9 && rmaxnorm < 0.38){ //Ver si es util
        return true;
      }
      else if(r1norm > 0.9 && rmaxnorm > 0.9){
        return false;
      }

    }
    else{
      if(r1norm > 0.975 && rmaxnorm > 0.5){ //con 0.97 -> 90.53%
        return false;
      }
      return true;
    }*/
  }

  //zcr
  float PitchAnalyzer::compute_zcr(vector<float> & x, unsigned int N, unsigned int fm) const {
	  unsigned int i=0;
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

  float PitchAnalyzer::compute_pitch(vector<float> & x,float maxPot) const {
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i < x.size(); ++i)
      x[i] *= window[i];

    vector<float> r(npitch_max);

    //Compute correlation
    autocorrelation(x, r);

    vector<float>::const_iterator iR = r.begin(), iRMax = r.begin() + npitch_min;

    /// \TODO 
	/// Find the lag of the maximum value of the autocorrelation away from the origin.<br>
	/// Choices to set the minimum value of the lag are:
	///    - The first negative value of the autocorrelation.
	///    - The lag corresponding to the maximum value of the pitch.
    ///	   .
	/// In either case, the lag should not exceed that of the minimum value of the pitch.
  /// \DONE Otra chivada de Albino (1st version).
  for(iR = r.begin() + npitch_min; iR < r.begin()+npitch_max; iR++){
    if(*iR > *iRMax){
      iRMax = iR;
    }
  }
  //https://stackoverflow.com/questions/7719978/finding-max-value-in-an-array/43921864

    unsigned int lag = iRMax - r.begin();

    float pot = (10 * log10(r[0]/x.size())); //frame power in dB
    pot = r[0] / maxPot;  //norm power (normalized with the power of the frame that has maxpower in the signal (before clipping))
    //float pot = r[0];

    //ZCR
  float zcr=compute_zcr(x, frameLen, samplingFreq) ;

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 1
    if (r[0] > 0.0F){
      //cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
      cout << maxPot << '\t\t' << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
      cout << '\n' << zcr << '\n';
    }
#endif
    
    if (unvoiced(pot, r[1]/r[0], r[lag]/r[0], zcr))
      return 0;
    else
      return (float) samplingFreq/(float) lag;
  }
}
