#include "scripts/Calibration_1.h"

Bool_t reject0 = kTRUE;

void Triple(int pol=1) {

  std::string treepath1 = "data/1705/triple_1.root",
              treepath2 = "data/1705/triple_2.root",
              treepath3 = "data/1705/triple_3.root",
              figpath = Form("figures/triple/1705_triple_pol%dfix_no_cut.pdf", pol),
              elementname = "{}^{22}Na";


  double LowLim = 0e3, UpLim = 2e3;
  double peakMin = 0.48e3, peakMax = 0.6e3;

  int nbins = 50;

  double EnergyLow = 2 * 511 * (1 - 0.02) ,
         EnergyUp = 2 * 511 * (1 + 0.02);

  double a1 = 42.3, a2 = 206.7, a3 = 86.9 ,
         da1 = 0.2, da2 = 2.5, da3 = 1.2;
  double b = 0, db = 0;

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


  auto h = CalSpectrum_sum_var(treepath1, treepath2, treepath3, pol, nbins,
                           LowLim, UpLim, EnergyLow, EnergyUp, a1, b, a2, b, a3, b);

  h->Fit(bkg, "SRLNQI");
  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_tot, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, .511e3, .02e3,
                           bkg->GetParameter(0), bkg->GetParameter(1), bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNI");

  double Stat_error = CalibrationError(fitFunc->GetParameter(1), pol, a1, da1, b, db);
  cout << "Stat. error on peak = " << Stat_error << endl;

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

}
