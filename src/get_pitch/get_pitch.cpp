/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#include <math.h>

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt> 
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project
    -1=FLOAT, --thr1norm=FLOAT  Threshold for r[1]/r[0] unvoice decision  [default: 0.90]
    -2=FLOAT, --thrmaxnorm=FLOAT  Threshold for r[lag]/r[0] unvoice decision  [default: 0.5]
    -3=FLOAT, --thpower=FLOAT  Threshold for normalized power used on the unvioce decision  [default: -0.0002]
    -4=FLOAT, --thzcr=FLOAT  Threshold zcr for unvoice decision [default: 2000]
    -l=INT, --length_median_w=INT  Length of the window used on the median filter, this value must be an odd number [default: 3]
    -c=FLOAT, --alpha_clipping=FLOAT  Alpha used to determinate the threshold for the central-clipping [default: 0.005]

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  //std::float threshold_maxR = args["<threshold-maxR>"].stof();
  float thr1norm=stof(args["--thr1norm"].asString());
  float thrmaxnorm=stof(args["--thrmaxnorm"].asString());
  float thpower=stof(args["--thpower"].asString());
  unsigned int M=stoi(args["--length_median_w"].asString());
  float alpha_cc=stof(args["--alpha_clipping"].asString());
  float thzcr=stof(args["--thzcr"].asString());

  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  //PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::RECT, 50, 500);
  PitchAnalyzer analyzer(n_len, rate, PitchAnalyzer::HAMMING, 50, 500, thr1norm, thrmaxnorm, thpower, thzcr);



  //Maximum signal power for power normalization
  vector<float>::iterator iX2;
  float maxPot = 1.e-9;
  float power;
  for (iX2 = x.begin(); iX2 + n_len < x.end(); iX2 = iX2 + n_shift) {
    power=0;

    std::vector<float> x2((iX2 + n_len)-iX2); //local copy of input frame, size N
    std::copy(iX2, iX2 + n_len, x2.begin()); //copy input values into local vector x2

    for(int i=0; i < n_len; ++i){
      power+=(x2[i]*x2[i]);
    }
    //power=((float)1/n_len)*power;/*1.0f*/
    power=power/n_len;/*1.0f*/
	//printf("Potencia media de la señal en decibelios: %.2f /n",power);
    if(power>maxPot){
      maxPot=power;
    }
    x2.clear();
  }
  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
  /// \DONE
  /// Central-clipping implementado

  
  //float th_cc = 0.05;
  float th_cc=maxPot*alpha_cc;
  for (unsigned int i=0; i < x.size(); ++i)
    if(abs(x[i]) < th_cc){
      x[i] = 0;
  }
  
  maxPot=10*log10(maxPot);/*1.0f*/ // in dB
  /* Emi solution
  vector<float>::iterator it;
  vector<float> cc;
  float cota=0.0055;

  for(it = x.begin(); it != x.end(); ++it){
    if(*it > cota)
      cc.push_back(*it-cota);
    else if(*it < -cota)
      cc.push_back(*it+cota);
    else
      cc.push_back(0);
  }
  x=cc;
  */

  // Iterate for each frame and save values in f0 vector
  vector<float>::iterator iX;
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len, maxPot);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  /// \DONE 
  /// Median filter, this method of medianfilter uses a moving window with a length of L. The value at
  /// point n is determined by the data from point n-(L-1)/2 to point n+(L-1)/2.
  /// Then the median value in these L points is chooses as the value the point. */
  /// colab info:https://stackoverflow.com/questions/2114797/compute-median-of-values-stored-in-vector-c
  if(M%2==1){ //if M is odd
    vector<float> f0filtered = f0;

    unsigned int L_median_W = M;
    vector<float> window(L_median_W);

    unsigned int offset = (L_median_W-1)/2;
    for (unsigned int n=offset; n<f0.size()-offset; ++n){ //for start at ceil(N_window/2-1)
      //vector<float> window {f0[n-1] , f0[n], f0[i+n]};
      for (unsigned int j=0; j<L_median_W; ++j)
        window[j] = f0[n - offset + j];

    //window.insert(window.begin(), f0[i-n] , f0[n], f0[n+1]);
      sort(window.begin(), window.end());
      //Get result, middle element
      f0filtered[n]=window[offset];
    //window.clear();
    }
    f0=f0filtered;
  }
  /* EMI
  */



  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
