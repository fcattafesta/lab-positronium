#include "scripts/calibration.h"

void provacalibr() {

  std::string treepath[4] = {"data/241Am.root"},
              figpath[4] = {"figures/241Am2604cal.pdf"},
              branchname = "Energy";
  double LowLim[4] = {20e3, 20e3, 20e3, 20e3},
         UpLim[4] = {100e3, 100e3, 100e3, 100e3},
         fitMin[4] = {40e3, 40e3, 40e3, 40e3},
         fitMax[4] = {43e3, 43e3, 43e3, 43e3};
  int nbins[4] = {200, 200, 200, 200};
  TFitResultPtr results[4];

  auto h = new TH1D();
  auto fitFunc = new TF1("fitFunc", "gaus");

  for (int i=0; i<4; i++) {

    h = MakeSpectrum(treepath[i], branchname, nbins[i], LowLim[i], UpLim[i]);
    fitFunc->SetRange(fitMin, fitMax);
    result[i] = h->Fit(fitFunc, "SRLNQ");
    
  }

}
