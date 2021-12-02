/// @file

#ifndef PITCH_ANALYZER_H
#define PITCH_ANALYZER_H

#include <vector>
#include <algorithm>

namespace upc {
  const float MIN_F0 = 20.0F;    ///< Minimum value of pitch in Hertzs
  const float MAX_F0 = 10000.0F; ///< Maximum value of pitch in Hertzs
  const float THR1NORM = 0.9;
  const float THRMAXNORM = 0.275; 
  const float THPOWER = -55.5;
  const float THZCR = 1825;

  ///
  /// PitchAnalyzer: class that computes the pitch (in Hz) from a signal frame.
  /// No pre-processing or post-processing has been included
  ///
  class PitchAnalyzer {
  public:
	/// Wndow type
    enum Window {
		RECT, 						///< Rectangular window
		HAMMING						///< Hamming window
	};

    void set_window(Window type); ///< pre-compute window

  private:
    std::vector<float> window; ///< precomputed window
    unsigned int frameLen, ///< length of frame (in samples). Has to be set in the constructor call
      samplingFreq, ///< sampling rate (in samples per second). Has to be set in the constructor call
      npitch_min, ///< minimum value of pitch period, in samples
      npitch_max; ///< maximum value of pitch period, in samples
    float threshold1,
      threshold2,
      threshold3,
      threshold4;
 
	///
	/// Computes correlation from lag=0 to r.size()
	///
    void autocorrelation(const std::vector<float> &x, std::vector<float> &r) const;

	///
	/// Returns the pitch (in Hz) of input frame x
	///
    float compute_pitch(std::vector<float> & x, float maxPot) const;
	
  ///
  /// Returns the zcr of input frame x
  ///
    float compute_zcr(std::vector<float> & x, unsigned int N, unsigned int fm) const;

	///
	/// Returns true is the frame is unvoiced
	///
    bool unvoiced(float pot, float r1norm, float rmaxnorm, float zcr) const;


  public:
    PitchAnalyzer(	unsigned int fLen,			///< Frame length in samples
					unsigned int sFreq,			///< Sampling rate in Hertzs
					Window w=PitchAnalyzer::HAMMING,	///< Window type
					float min_F0 = MIN_F0,		///< Pitch range should be restricted to be above this value
					float max_F0 = MAX_F0,  	///< Pitch range should be restricted to be below this value
          float thr1norm = THR1NORM,
          float thrmaxnorm = THRMAXNORM, 
          float thpower = THPOWER,
          float thzcr = THZCR
				 )
	{
      frameLen = fLen;
      samplingFreq = sFreq;
      set_f0_range(min_F0, max_F0);
      set_window(w);
      threshold1=thr1norm;
      threshold2=thrmaxnorm;
      threshold3=thpower;
      threshold4=thzcr;
    }

	///
    /// Operator (): computes the pitch for the given vector x
	///
    float operator()(const std::vector<float> & _x, float maxPot) const {
      if (_x.size() != frameLen)
        return -1.0F;

      std::vector<float> x(_x); //local copy of input frame
      return compute_pitch(x,maxPot);
    }

	///
    /// Operator (): computes the pitch for the given "C" vector (float *).
    /// N is the size of the vector pointer by pt.
	///
    float operator()(const float * pt, unsigned int N, float maxPot) const {
      if (N != frameLen)
        return -1.0F;

      std::vector<float> x(N); //local copy of input frame, size N
      std::copy(pt, pt+N, x.begin()); ///copy input values into local vector x
      return compute_pitch(x,maxPot);
    }

	///
    /// Operator (): computes the pitch for the given vector, expressed by the begin and end iterators
	///
    float operator()(std::vector<float>::const_iterator begin, std::vector<float>::const_iterator end, float maxPot) const {

      if (end-begin != frameLen)
        return -1.0F;

      std::vector<float> x(end-begin); //local copy of input frame, size N
      std::copy(begin, end, x.begin()); //copy input values into local vector x
      return compute_pitch(x,maxPot);
    }
    
	///
    /// Sets pitch range: takes min_F0 and max_F0 in Hz, sets npitch_min and npitch_max in samples
	///
    void set_f0_range(float min_F0, float max_F0);
  };
}
#endif
