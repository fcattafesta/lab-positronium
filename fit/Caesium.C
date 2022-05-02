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

void Caesium() {

  std::string treepath = "data/137Cs.root",
              figpath = "figures/fit/137Cs.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{137}Cs";

  double LowLim = 22e3, UpLim = 32e3;
  double fitMin = 22e3, fitMax = 32e3;
  int nbins = 150;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", "pol1", 22e3, 26e3);
  bkg->SetParameters(100, -1.3);

  h->Fit(bkg, "SRLN");

  TF1 *fitFunc = new TF1("fitFunc", func, fitMin, fitMax, 5);
  fitFunc->SetParameters(2.5e3, 28e3, .8e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLN");

  auto c = new TCanvas();
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegend(c, .1, .65, .55, .85, h,
                           fitMin, fitMax, fitFunc);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  c->SaveAs(figpath.c_str());

}
