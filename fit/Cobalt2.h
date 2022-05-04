#include "Cobalt1.h"

double background2(double *x, double *p) {
  return p[0] + p[1] * x[0];
}

double signal2(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func2(double *x, double *p) {
  return signal2(x, p) + background2(x, &p[3]);
}

TFitResultPtr Cobalt2() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/fit/60Co_2.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{60}Co";

  double LowLim = 53e3, UpLim = 67e3;
  double fitMin = 53e3, fitMax = 67e3;
  int nbins = 200;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", "pol1", 60e3, 66e3);
  bkg->SetParameters(10., -1e-5);

  h->Fit(bkg, "SRLNQ");

  TF1 *fitFunc = new TF1("fitFunc", func2, fitMin, fitMax, 5);
  fitFunc->SetParameters(.2e3, 56e3, 1.1e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQ");

  auto c = new TCanvas();
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c, .45, .65, .7, .85, h,
                           fitMin, fitMax, fitFunc);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  c->SaveAs(figpath.c_str());
  c->Destructor();

  return results;

}