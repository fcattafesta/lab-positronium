#include "scripts/spectrum.h"
#include "scripts/plot.h"

void calibration() {

  int npoints = 3;
  double eRef[3] = {59.54, 661.6, 1173, 1333.};
  double eFit[3] = {43046, 44779, 119086, 132649};
  double errFit[3] = {190, 13, 50, , 30};
  double errx[3] = {0, 0, 0};

  auto g = new TGraphErrors(npoints, eRef, eFit, errx, errFit);

  auto fit = new TF1("linear", "pol1", 0, 2000);

  auto c1 = new TCanvas();

  g->Fit(fit, "RN EX0");
  g->Draw("AP SAME");
  auto xaxis = g->GetXaxis();
  xaxis->SetTitle("Reference [keV]");
  auto yaxis = g->GetYaxis();
  yaxis->SetTitle("Fit Value [u.a.]");
  fit->Draw("AL SAME");
  g->SetMarkerStyle(8);
}
