#include "scripts/spectrum.h"
#include "scripts/plot.h"

void calibration() {

  int npoints = 4;
  double eRef[4] = {59.54, 661.6, 1173, 1333.};
  double eFit[4] = {43046, 43137, 64502, 71250};
  double errFit[4] = {16, 3, 18, 20};
  double errx[4] = {0, 0, 0, 0.};

  auto g = new TGraphErrors(npoints, eRef, eFit, errx, errFit);

  auto fit = new TF1("linear", "pol1", 0, 2000);

  auto c1 = new TCanvas();
  c1->SetGrid();
  g->Fit(fit, "RN EX0");
  g->Draw("AP SAME");
  g->SetTitle("");
  auto xaxis = g->GetXaxis();
  xaxis->SetTitle("Reference [keV]");
  auto yaxis = g->GetYaxis();
  yaxis->SetTitle("Fit Value [u.a.]");
  fit->Draw("AL SAME");
  g->SetMarkerStyle(8);
  
}
