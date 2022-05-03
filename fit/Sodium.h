#include "../scripts/spectrum.h"
#include "../scripts/plot.h"

double background(double *x, double *p) {
  return p[0] + p[1] * x[0];
}

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func(double *x, double *p) {
  return signal(x, p) + background(x, &p[3]);
}

TFitResultPtr Sodium() {

  std::string treepath = "data/22Na.root",
              figpath = "figures/fit/22Na.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{22}Na";

  double calConv = 41.48, calOffset = 1036;

  double LowLim = .35e3, UpLim = .6e3;
  double fitMin = 0, fitMax = 1.5e3;
  int nbins = 200;

  auto h = CalibrateSpectrum(treepath, treename, branchname, nbins,
                             LowLim, UpLim, calConv, calOffset);

  TF1 *bkg = new TF1("bkg", "pol1", .35e3, .45e3);
  bkg->SetParameters(.4e3, -1e-2);

  h->Fit(bkg, "SRLNQ");

  TF1 *fitFunc = new TF1("fitFunc", func, fitMin, fitMax, 5);
  fitFunc->SetParameters(1.2e3, .5e3, .02e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQ");

  auto c = new TCanvas();
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c, .1, .65, .4, .85, h,
                           fitMin, fitMax, fitFunc);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  c->SaveAs(figpath.c_str());

  return results;

}
