#include "scripts/spectrum.h"
#include "scripts/plot.h"

void NaSpectrum() {

  std::string treepath = "data/22Na.root",
              figpath = "figures/22NaSpectrum/2804.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap";

  double cte = 1043, d_cte=31, slope = 41.46, d_slope=0.04, corr = -0.952;

  double LowLim = 100, UpLim = 1600;  // keV
  double fitMin = 470, fitMax = 545;
  int nbins = 300;

  double error=0;
  int idx=0;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim, cte, slope);
  auto dh = CalibrationError(treepath, treename, branchname, "EnergyError",
                            nbins, LowLim, UpLim, cte, d_cte, slope, d_slope, corr);

  auto fitFunc = new TF1("fitFunc", "gaus", fitMin, fitMax);

  auto c1 = new TCanvas();
  auto result = h->Fit(fitFunc, "SRLNQ");
  h->Draw();
  for (int j=0; j<h->GetEntries(); j++) {
    if (Cut(h->GetBinContent(j), fitMin, fitMax) ==1) {
      error = error + dh->GetBinContent(j);
      idx ++;
    }
  }

  fitFunc->Draw("AL SAME");

  error = error/idx;
  cout << "Errore sistematico = " << error << endl;


  auto legend = DrawLegend(c1, .35, .65, .65, .85, h, fitMin, fitMax, fitFunc);
  legend->SetHeader("^{22}Na", "C");

  DrawDate(c1);

  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  //c1->SaveAs(figpath.c_str());

}
