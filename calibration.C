#include "scripts/spectrum.h"
#include "scripts/plot.h"

void calibration() {

  std::string treepath = "data/137Cs.root", figpath = "figures/137CsImproved.pdf";

  double LowLim = 40e3;
  double UpLim = 50e3;
  int nbins = 80;

  auto h = MakeSpectrum(treepath, nbins, LowLim, UpLim);

  TF1 *peak = new TF1("peak", "gaus", 42.5e3, 47e3);

  auto c1 = new TCanvas();
  auto result = h->Fit(peak, "SRLNQ");
  h->Draw();
  gPad->Update();
  peak->Draw("AL SAME");

  auto legend = DrawLegend(c1, .35, .65, .85, .85, h, peak);
  legend->SetHeader("^{137}Cs", "C");

  DrawDate(c1);

  MyStyle(h, peak);
  //c1->SaveAs(figpath.c_str());
}
