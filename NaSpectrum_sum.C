#include "scripts/Calibration.h"

Bool_t reject0 = kTRUE;

void NaSpectrum_sum(int pol=1) {

  std::string treepath1 = "data/PMT1/1205/coinc_1.root",
              treepath2 = "data/PMT2/1205/coinc_2.root",
              figpath = Form("figures/time/1205_totalEnergy_pol%dfix.pdf", pol),
              treename = "tree;2",
              branchname = "EnergyTrap",
              elementname = "{}^{22}Na";


  double LowLim = 0.8, UpLim = 1.2e3;
  double peakMin = 0.95e3, peakMax = 1.15e3;

  double a_1 = -0.0005, da_1 = 0.0002;
  double b_1 = 42.89, db_1 = 0.17;
  double corr_1 = -0.95;

  double a_2 = -0.0005, da_2 = 0.0002;
  double b_2 = 42.89, db_2 = 0.17;
  double corr_2 = -0.95;



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
  //bkg->SetParameters(.3e3, -0.03, .003);


  auto h = CalSpectrum(treepath, treename, branchname, pol, nbins, LowLim, UpLim, a, b);

  h->Fit(bkg, "SRLNI");
  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_tot, LowLim, UpLim, 6);
  fitFunc->SetParameters(1.2e3, 1.022e3, .02e3,
                           bkg->GetParameter(0), bkg->GetParameter(1), bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNI");

  double Stat_error_1 = CalibrationError(fitFunc->GetParameter(1)/2., pol, a1, da1, b1, db1, corr1);
  double Stat_error_2 = CalibrationError(fitFunc->GetParameter(1)/2., pol, a2, da2, b2, db2, corr2);
  cout << "Stat. error on peak = " << TMath::Sqrt( Stat_error_1*Stat_error_1 + Stat_error_2*Stat_error_2 ) << endl;

  DrawCalFits(h, figpath, elementname, fitFunc, nbins, LowLim, UpLim, results);

}
