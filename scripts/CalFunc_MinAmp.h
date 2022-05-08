#include "spectrum.h"
#include "plot.h"

std::string treename = "tree;3", branchname = "EnergyMinAmp";

Bool_t reject0 = kTRUE;
Bool_t reject1 = kTRUE;

double bkg_Cs(double *x, double *p) {
  if (reject0 && x[0]>=320 && x[0]<=420) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2]*x[0]*x[0];
}


double bkg_Co1(double *x, double *p) {
  /* if (reject1 && x[0] >= 600 && x[0] <= 780) {
    TF1::RejectPoint();
    return 0;
  } */
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double bkg_Co2(double *x, double *p) {
  return p[0] + p[1] * x[0];
}


double signal(double *x, double *p) {
  return p[0] * exp( -0.5 * pow((x[0] - p[1]), 2) / pow((p[2]), 2) );
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

/*double f_Cobalt(double *x, double *p) {
  return signal(x, p) + signal(x, &p[3]) + bkg_Co2(x, &p[6]);
}*/


TFitResultPtr Caesium() {

  std::string treepath = "data/137Cs.root",
              figpath = "figures/calibrationMinAmp/137Cs.pdf",
              elementname = "{}^{137}Cs";

  double LowLim = 280, UpLim = 420;
  double peakMin = 320, peakMax = 420;
  int nbins = 200;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Cs, LowLim, UpLim, 3);
  bkg->SetParameters(7.8e2, -2.24, 1.7e-3);

  h->Fit(bkg, "SRLNQI");

  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Cs, LowLim, UpLim, 6);
  fitFunc->SetParameters(1000, 370, 15,
                         bkg->GetParameter(0), bkg->GetParameter(1), bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  //cout << results->Prob() << endl;
  return results;

}


TFitResultPtr Cobalt1() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/calibrationMinAmp/60Co_1.pdf",
              elementname = "{}^{60}Co";

  double LowLim = 500, UpLim = 680;
  double bkgMin = 500, bkgMax = 600;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Co1, bkgMin, bkgMax, 2);
  bkg->SetParameters(1.3e3, -.2); //, 1e-5);

  h->Fit(bkg, "SRLNQI");

  reject1 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Co1, LowLim, UpLim, 5);
  fitFunc->SetParameters(1000, 640, 25,
                         bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  return results;
}


TFitResultPtr Cobalt2() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/calibrationMinAmp/60Co_2.pdf",
              elementname = "{}^{60}Co";

  double LowLim = 680, UpLim = 1000;
  double bkgMin = 800, bkgMax = 1000;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", bkg_Co2, bkgMin, bkgMax, 2);
  //bkg->SetParameters(1.3e3, -.2, 1e-5);

  h->Fit(bkg, "SRLNQI");

  TF1 *fitFunc = new TF1("fitFunc", f_Co2, LowLim, UpLim, 5);
  fitFunc->SetParameters(1000, 720, 25, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins,


    LowLim, UpLim, results);

  return results;

}
