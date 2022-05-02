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

void Cobalt1() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/fit/60Co_1.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{60}Co";

  double LowLim = 40e3, UpLim = 53e3;
  double fitMin = 40e3, fitMax = 53e3;
  int nbins = 200;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", "pol1", 40e3, 47e3);
  bkg->SetParameters(.4e2, -.4);

  h->Fit(bkg, "SRLN");

  TF1 *fitFunc = new TF1("fitFunc", func, fitMin, fitMax, 5);
  fitFunc->SetParameters(.2e3, 49e3, 1.1e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLN");

  auto c = new TCanvas();
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c, .1, .65, .35, .85, h,
                           fitMin, fitMax, fitFunc);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  c->SaveAs(figpath.c_str());

}
