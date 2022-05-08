#include "scripts/spectrum.h"
#include "scripts/plot.h"

Bool_t reject = kTRUE;

double bkg_Na(double *x, double *p) {
  if (reject && x[0]>=460 && x[0]<=560) {
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

<<<<<<< HEAD
  std::string treepath = "data/22Na_2.root",
              figpath = "figures/22NaSpectrum/0405_calMinAmp.pdf",
              treename = "tree;3",
              branchname = "EnergyMinAmp",
=======
  std::string treepath = "data/22Na/19mV.root",
              figpath = "figures/22NaSpectrum/19mV_spectrum.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap",
>>>>>>> 49d02f098906a20eadcba7a113e968cb1d3279bf
              elementname = "{}^{22}Na";

  //double cte = 1021, d_cte=30, slope = 41.50, d_slope=0.035, corr = -0.9516;

<<<<<<< HEAD
  double cte = 26.89, d_cte=1.00, slope = 0.5228, d_slope=0.0011, corr = -0.9490;

  double LowLim = 0.35e3, UpLim = 0.6e3;
  double peakMin = 0.46e3, peakMax = 0.56e3;
  int nbins = 400;
=======
  double LowLim = 200, UpLim = 1.6e3;
  double fitMin = .35e3, fitMax = .6e3;
  int nbins = 200;
>>>>>>> 49d02f098906a20eadcba7a113e968cb1d3279bf

  double error=0;
  int idx=0;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim, cte, slope);
  auto dh = CalibrationError(treepath, treename, branchname, "EnergyError",
                            nbins, peakMin, peakMax, cte, d_cte, slope, d_slope, corr);

  TF1 *bkg = new TF1("bkg", bkg_Na, LowLim, UpLim, 3);
  bkg->SetParameters(.3e3, -0.3, .3);

  h->Fit(bkg, "SRLNQI");

  reject = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Na, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, .511e3, .02e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNI");

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

}
