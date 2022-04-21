#include "scripts/spectrum.h"
#include "scripts/plot.h"

void analysis() {

  std::string treepath = "data/60Co.root";
  double LowLim = 20e3;
  double UpLim = 90e3;
  int nbins = 80;

  auto h = MakeSpectrum(treepath, nbins, LowLim, UpLim);

  TF1 *peak = new TF1("peak", "gaus", 73e3, 77e3);

  auto c1 = new TCanvas();
  auto result = h->Fit(peak, "SRLNQ");
  h->Draw();
  gPad->Update();
  peak->Draw("AL SAME");

  auto legend = DrawLegend(c1, .18, .65, .55, .85, h, peak);
  legend->SetHeader("^{60}Co", "C");

  DrawDate(c1);

  MyStyle(h, peak);
  c1->SaveAs("provaCo.pdf");
}
