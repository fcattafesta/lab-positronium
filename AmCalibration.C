#include "scripts/spectrum.h"
#include "scripts/plot.h"

void AmCalibration() {

  std::string treepath = "data/241Am.root",
              figpath = "figures/22NaSpectrum/strangepeakAm2804.pdf",
              treename = "tree;2",
              branchname = "EnergyTrap";

  double slope = 41.83, intercept = 15460;

  double LowLim = 100, UpLim = 1000;  // keV
  double fitMin = 630, fitMax = 700;
  int nbins = 300;

  auto h = CalibrateSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim,
                             slope, intercept);
  auto fitFunc = new TF1("fitFunc", "gaus", fitMin, fitMax);

  auto c1 = new TCanvas();
  auto result = h->Fit(fitFunc, "SRLNQ");
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c1, .35, .65, .65, .85, h, fitMin, fitMax, fitFunc);
  legend->SetHeader("^{241}Am", "C");

  DrawDate(c1);

  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  c1->SaveAs(figpath.c_str());

}
