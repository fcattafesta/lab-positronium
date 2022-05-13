#include "scripts/calibration.h"

Bool_t reject0 = kTRUE;

void NaSpectrum() {

  std::string treepath = "data/1005/coinc_1.root",
              figpath = "figures/time/1005_coinc1_pol2fix.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";


  double LowLim = 0.4e3, UpLim = 0.7e3;
  double peakMin = 0.48e3, peakMax = 0.6e3;
  double cte = 0,  d_cte=0, slope = 40.96, d_slope = 0.22;
  double a = -0.0007, b= 43.34;
  int nbins = 200;

  auto f_bkg = [=](double *x, double *p) {
      if (reject0 && x[0]>=peakMin && x[0]<=peakMax) {
      TF1::RejectPoint();
      return 0.;
    }
    return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
  };

  auto f_tot = [&](double *x, double *p) {
       return signal(x, p) + f_bkg(x, &p[3]);
    };

/*
  auto h = CalibrateSpectrum(treepath, treename, branchname,
                              nbins, LowLim, UpLim, cte, slope);
*/
  auto h = CalibrateSpectrum_var(treepath, treename, branchname,
                                  nbins, LowLim, UpLim, a, b);

  TF1 *bkg = new TF1("bkg", f_bkg, LowLim, UpLim, 3);
  bkg->SetParameters(.3e3, -0.03, .003);

  h->Fit(bkg, "SRLNI");

  reject0 = kFALSE;


  TF1 *fitFunc = new TF1("fitFunc", f_tot, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, .511e3, .02e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  //TF1 *fitFunc = new TF1("fitFunc", "pol2", 490, 520);

  auto results = h->Fit(fitFunc, "SRLNQI");

  //cout << -fitFunc->GetParameter(1)/(2*fitFunc->GetParameter(2)) << endl;


  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

  cout << "Stat. error on peak = " << fitFunc->GetParameter(1)*(d_slope/slope) << endl;

}
