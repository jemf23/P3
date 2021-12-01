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
      /// \DONE Albino nos ha chivado el cálculo de la autocorrelación
      r[l]=0;
      for (unsigned int n = l; n < x.size(); n++){
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
      /// \DONE Ventana de Hamming implementada
      float a0=0.53836;
      double pi=3.141592653589793;
      for(unsigned int i = 0; i < frameLen; i++){
        window[i]=a0-(1-a0)*cos((2*pi*i)/frameLen);
      }
    }
    break;
    case RECT:{
       window.assign(frameLen, 1);
    }
    break;
    default:{
      window.assign(frameLen, 1);
    }
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

  bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm) const {
    /// \TODO Implement a rule to decide whether the sound is voiced or not.
    /// * You can use the standard features (pot, r1norm, rmaxnorm),
    ///   or compute and use other ones.
    /// \DONE Implementado en clase (15/11/2021)

    //CON CENTER CLIPPING, COTAS AJUSTADAS 90.77% 
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
      else 
        return true;
    }


    //CON CENTER CLIPPING, COTAS POCO AJUSTADAS 90.54%
    /*
    if(r1norm > 0.9 && rmaxnorm < 0.3)
      return true;
    else if(pot > -29 && r1norm > 0.57 && rmaxnorm > 0.4) //SONORO
      return false; 
    else//SORDO
      return true; */

    //ANTES DE CENTER CLIPPING
    /*if(pot > -70)//&& r1norm > 0 && rmaxnorm > 0.5) //SONORO
      return false; //OJO!, el 15/11/21 lo pusimos a false!!
    else if(r1norm > 0.97)
      return false;
    else//SORDO
      return true; //ANTES DE CENTER CLIPPING*/ 
  }

  float PitchAnalyzer::compute_pitch(vector<float> & x) const {
    if (x.size() != frameLen)
      return -1.0F;

    //Window input frame
    for (unsigned int i=0; i<x.size(); ++i)
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
  /// \DONE Implementado en clase (15/11/2021)
    for(iR = r.begin() + npitch_min; iR < r.begin()+npitch_max; iR++){
      if(*iR > *iRMax){
        iRMax = iR;
      }
    }
    unsigned int lag = iRMax - r.begin();

    float pot = 10 * log10(r[0]);

    //You can print these (and other) features, look at them using wavesurfer
    //Based on that, implement a rule for unvoiced
    //change to #if 1 and compile
#if 0
    if (r[0] > 0.0F){
      cout << pot << '\t' << r[1]/r[0] << '\t' << r[lag]/r[0] << endl;
      /*for(int i=0; i < r.size(); i++)
        cout << x[i] << endl;*/
    }
#endif
    
      if (unvoiced(pot, r[1]/r[0], r[lag]/r[0]))
        return 0;
      else{
        //cout << (float) samplingFreq/(float) lag << endl;
        return (float) samplingFreq/(float) lag;
      }
  }
}
