#include "scripts/spectrum.h"
#include "scripts/plot.h"

void analysis() {

  std::string treepath = "data/60Co.root", figpath = "figures/60Co2604.pdf";

  double LowLim = 20e3;
  double UpLim = 80e3;
  int nbins = 200;

  auto h = MakeSpectrum(treepath, nbins, LowLim, UpLim);

  TF1 *peak = new TF1("peak", "gaus", 127e3, 139e3);

  auto c1 = new TCanvas();
  auto result = h->Fit(peak, "SRLNQ");
  h->Draw();
  gPad->Update();
  peak->Draw("AL SAME");

  auto legend = DrawLegend(c1, .25, .65, .65, .85, h, peak);
  legend->SetHeader("^{60}Co", "C");

  DrawDate(c1);

  MyStyle(h, peak);
  c1->SaveAs(figpath.c_str());

  /*auto c2 = new TCanvas();
  DrawSignal(treepath, 10);
  DrawDate(c2);
  //c2->SaveAs("figures/signal.pdf"); */
}
