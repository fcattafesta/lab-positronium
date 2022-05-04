#include "scripts/spectrum.h"
#include "scripts/plot.h"

void NaSpectrum() {

  std::string treepath = "data/22Na.root",
              figpath = "figures/22NaSpectrum/2804.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap";

  double slope[2] = [41.82, ], intercept[2] = [1180, ];

  double LowLim = 100, UpLim = 1600;  // keV
  double fitMin = 470, fitMax = 545;
  int nbins = 300;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim,
                             slope, intercept);
  auto fitFunc = new TF1("fitFunc", "gaus", fitMin, fitMax);

  auto c1 = new TCanvas();
  auto result = h[0]->Fit(fitFunc, "SRLNQ");
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c1, .35, .65, .65, .85, h, fitMin, fitMax, fitFunc);
  legend->SetHeader("^{22}Na", "C");

  DrawDate(c1);

  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  //c1->SaveAs(figpath.c_str());

}
