#include "scripts/Calibration_1.h"

Bool_t reject0 = kTRUE;

void NaSpectrum(int pol=2) {

  std::string treepath = "data/1005/coinc_1.root",
              figpath = Form("figures/time_1/check/1005_coinc1_pol%dfix_after.pdf", pol),
              treename = "tree;2",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";


  double LowLim = 0.4e3, UpLim = 0.7e3;
  double peakMin = 0.48e3, peakMax = 0.6e3;

  double slope = 42.77, d_slope = 0.22;
  double cte = 0,  d_cte=0;

  double a = -0.0005, da = 0.0002;
  double b = 42.89, db = 0.17;
  double corr = -0.95;


  if (pol==1) {
    a = slope;
    da = d_slope;
    b = cte;
    db = d_cte;
  }


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


  TF1 *bkg = new TF1("bkg", f_bkg, LowLim, UpLim, 3);
  bkg->SetParameters(.3e3, -0.03, .003);


  auto h = CalSpectrum(treepath, treename, branchname, pol, nbins, LowLim, UpLim, a, b);

  h->Fit(bkg, "SRLNQI");
  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_tot, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, .511e3, .02e3,
                           bkg->GetParameter(0), bkg->GetParameter(1), bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNI");

  double Stat_error = CalibrationError(fitFunc->GetParameter(1), pol, a, da, b, db, corr);
  cout << "Stat. error on peak = " << Stat_error << endl;

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

}
