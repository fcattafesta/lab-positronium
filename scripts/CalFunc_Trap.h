#include "spectrum.h"
#include "plot.h"

std::string treename = "tree;2", branchname = "EnergyTrap";

Bool_t reject0 = kTRUE;
Bool_t reject1 = kTRUE;

double bkg_Cs(double *x, double *p) {
  if (reject0 && x[0]>=25e3 && x[0]<=31.5e3) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double bkg_Co1(double *x, double *p) {
  /*if (reject1 && x[0] >= 46e3 && x[0] <= 60e3) {
    TF1::RejectPoint();
    return 0;
  } */
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double bkg_Co2(double *x, double *p) {
  return p[0] + p[1] * x[0];
}


double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}


double f_Cs(double *x, double *p) {
  return signal(x, p) + bkg_Cs(x, &p[3]);
}

double f_Co1(double *x, double *p) {
  return signal(x, p) + bkg_Co1(x, &p[3]);
}

double f_Co2(double *x, double *p) {
  return signal(x, p) + bkg_Co2(x, &p[3]);
}


TFitResultPtr Caesium() {

  std::string treepath = "data/137Cs.root",
              figpath = "figures/calibrationTrap/137Cs.pdf",
              elementname = "{}^{137}Cs";

  double LowLim = 22e3, UpLim = 32e3;
  double peakMin = 25e3, peakMax = 31.5e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Cs, LowLim, UpLim, 3);
  bkg->SetParameters(100, .03, 1.e-7);

  h->Fit(bkg, "SRLNQI");

  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Cs, LowLim, UpLim, 6);
  fitFunc->SetParameters(2.5e3, 28e3, .8e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  //cout << results->Prob() << endl;
  return results;

}


TFitResultPtr Cobalt1() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/calibrationTrap/60Co_1.pdf",
              elementname = "{}^{60}Co";

  double LowLim = 40e3, UpLim = 53e3;
  double bkgMin = 40e3, bkgMax = 46e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Co1, bkgMin, bkgMax, 3);
  bkg->SetParameters(40, -.2, 1e-5);

  h->Fit(bkg, "SRLNQI");

  reject1 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Co1, LowLim, UpLim, 6);
  fitFunc->SetParameters(.2e3, 49e3, 1.1e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  return results;
}


TFitResultPtr Cobalt2() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/calibrationTrap/60Co_2.pdf",
              elementname = "{}^{60}Co";

  double LowLim = 53e3, UpLim = 67e3;
  double bkgMin = 60e3, bkgMax = 66e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Co2, bkgMin, bkgMax, 2);
  bkg->SetParameters(10., -1e-5);

  h->Fit(bkg, "SRLNQI");

  TF1 *fitFunc = new TF1("fitFunc", f_Co2, LowLim, UpLim, 5);
  fitFunc->SetParameters(.2e3, 56e3, 1.1e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  return results;

}
