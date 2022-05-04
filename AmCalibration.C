#include "scripts/spectrum.h"
#include "scripts/plot.h"

void AmCalibration() {

  std::string treepath = "data/241Am/Noise_50thr.root",
              figpath = "figures/241Am/noise50thr.pdf",
              treename = "tree;2",
              branchname = "EnergyMinAmp";

  double slope = 41.83, intercept = 15460;

  double LowLim = .0e3, UpLim = 1e3;  // keV
  double fitMin = 2.5e3, fitMax = 3.5e3;
  int nbins = 200;

  auto h = MakeSpectrum(treepath, treename, branchname,
                             nbins, LowLim, UpLim);

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
