#include "spectrum.h"
#include "plot.h"

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

Bool_t reject = kTRUE;


TFitResultPtr FitPeak(std::string treepath, std::string figpath, std::string elementname,
         double histMin, double histMax, double peakMin, double peakMax,
         double initBkg[3], double initPeak[3], int nbins=100,
         std::string treename="tree;2", std::string branchname="EnergyTrap") {

  cout << reject << endl;
  auto h = MakeSpectrum(treepath, treename, branchname, nbins, histMin, histMax);


  auto f_bkg = [=](double *x, double *p) {
      if (reject && x[0]>=peakMin && x[0]<=peakMax) {
      TF1::RejectPoint();
      return 0.;
    }
    return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
  };

  auto f_tot = [&](double *x, double *p) {
       return signal(x, p) + f_bkg(x, &p[3]);
    };

  TF1 *bkg = new TF1("bkg", f_bkg, histMin, histMax, 3);
  bkg->SetParameters(initBkg[0], initBkg[1], initBkg[2]);

  h->Fit(bkg, "SRLNI");

  reject = kFALSE;
  cout << reject << endl;

  TF1 *fitFunc = new TF1("fitFunc", f_tot, histMin, histMax, 6);
  fitFunc->SetParameters(initPeak[0], initPeak[1], initPeak[2],
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, histMin, histMax, results);

  reject = kTRUE;
  return results;

}
