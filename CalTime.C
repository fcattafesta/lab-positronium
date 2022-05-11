#include ""

void CalTime() {

  std::string figpath = "";
  const int npoints = 2;

  TFitResultPtr results[npoints];

  for (int i=0; i<npoints; i++) {
    results[i] = Calibration();
  }



}
