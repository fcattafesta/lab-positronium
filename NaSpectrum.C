#include "scripts/spectrum.h"
#include "scripts/plot.h"

Bool_t reject = kTRUE;

double bkg_Na(double *x, double *p) {
  if (reject && x[0]>=470 && x[0]<=570) {
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

  std::string treepath = "data/range/22Na_2804.root",
              figpath = "figures/22NaSpectrum/range_400_700.pdf",
              treename = "tree;4",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";


  /*
  std::string treepath = "data/22Na/19mV.root",
              figpath = "figures/22NaSpectrum/19mV_spectrum.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";
              */

  //double cte = 1100, d_cte=210, slope = 41.89, d_slope=0.20, corr = -0.9595;
  //double cte = 27.3, d_cte=7.3, slope = 0.5227, d_slope=0.0069, corr = -0.9605;
  //double cte = 0, d_cte=0, slope = 0.5473, d_slope=0.019, corr = 0;

  double cte = 940, d_cte=170, slope = 40.44, d_slope=0.16, corr = -0.9586;



  double LowLim = 0.4e3, UpLim = 0.6e3;
  double peakMin = 0.45e3, peakMax = 0.53e3;
  int nbins = 200;

  double error=0;
  int idx=0;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                              nbins, LowLim, UpLim, cte, slope);

                              /*
  double a = -0.0011, b = 43.7;
  auto h = CalibrateSpectrum(treepath, treename, branchname,
                              nbins, LowLim, UpLim, a, b);
                              */

  auto dh = CalibrationError(treepath, treename, branchname, "EnergyError",
                              nbins, peakMin, peakMax, cte, d_cte, slope, d_slope, corr);

  TF1 *bkg = new TF1("bkg", bkg_Na, LowLim, UpLim, 3);
  bkg->SetParameters(.3e3, -0.03, .003);

  h->Fit(bkg, "SRLNI");

  reject = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_Na, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, .511e3, .02e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");



  cout << "Syst. error on peak = " << dh->GetMean() << " " << dh->GetRMS() << endl;



  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

}
