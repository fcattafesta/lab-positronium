#include "scripts/spectrum.h"
#include "scripts/plot.h"

Bool_t reject = kTRUE;

double bkg_Na(double *x, double *p) {
  if (reject && x[0]>=540 && x[0]<=550) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double f_Na(double *x, double *p) {
  return signal(x, p) + bkg_Na(x, &p[3]);
}

void NaSpectrum() {

  std::string treepath = "data/22Na/19mV.root",
              figpath = "figures/22NaSpectrum/19mV_spectrum.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";

  double cte = 1031, d_cte=30, slope = 41.49, d_slope=0.03, corr = -0.952;

  double LowLim = 200, UpLim = 1.6e3;
  double fitMin = .35e3, fitMax = .6e3;
  int nbins = 200;

  double error=0;
  int idx=0;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim, cte, slope);
  auto dh = CalibrationError(treepath, treename, branchname, "EnergyError",
                            nbins, LowLim, UpLim, cte, d_cte, slope, d_slope, corr);

  TF1 *bkg = new TF1("bkg", bkg_Na, fitMin, fitMax, 3);
  bkg->SetParameters(.3e3, -0.3, .3);

  h->Fit(bkg, "SRLNQ");

  reject = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Na, fitMin, fitMax, 6);
  fitFunc->SetParameters(1.2e3, .5e3, .02e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLN");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, fitMin, fitMax, LowLim, UpLim, results);

}
