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
              branchname = "EnergyTrap",
              treename = "tree;2",
              elementname = "{}^{60}Co";

  double LowLim = 53e3, UpLim = 67e3;
  double fitMin = 53e3, fitMax = 67e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", "pol1", 60e3, 66e3);
  bkg->SetParameters(10., -1e-5);

  h->Fit(bkg, "SRLNQ");

  TF1 *fitFunc = new TF1("fitFunc", func2, fitMin, fitMax, 5);
  fitFunc->SetParameters(.2e3, 56e3, 1.1e3, bkg->GetParameter(0), bkg->GetParameter(1));

  auto results = h->Fit(fitFunc, "SRLNQ");

  auto c = new TCanvas();

  TPad *pad1 = new TPad("","",0,0.33,1,1);
  TPad *pad2 = new TPad("","",0,0,1,0.33);
  pad1->SetBottomMargin(0.00001);
  pad1->SetBorderMode(0);
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.2);
  pad2->SetBorderMode(0);
  pad1->Draw();
  pad2->Draw();

  pad1->cd();
  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegendFit(c, .45, .65, .7, .85, h,
                           fitMin, fitMax, fitFunc, results);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);

  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [u.a.]");
  xaxis->SetLimits(LowLim, UpLim);

  pad2->cd(); pad2->SetGrid();

  auto res = new TGraph();

  for (int i=1; i<=nbins; i++) {
    if (h->GetBinError(i)!= 0) {
      auto diff = (h->GetBinContent(i) - fitFunc->Eval(h->GetBinCenter(i))) /
                h->GetBinError(i);
      res->AddPoint(h->GetBinCenter(i), diff);
    }
  }

  res->Draw("AP");
  res->SetMarkerStyle(8);
  res->SetMarkerSize(.4);

  auto zero = new TF1("zero", "0", LowLim, UpLim);
  zero->Draw("AL SAME");
  zero->SetLineStyle(9);
  zero->SetLineColor(kBlack);
  zero->SetLineWidth(1);

  auto resXaxis = res->GetXaxis(); resXaxis->SetLimits(LowLim, UpLim);
  resXaxis->SetTitle("Energy [u.a.]");
  resXaxis->SetTitleSize(.1); resXaxis->SetTitleOffset(.9);
  resXaxis->SetLabelSize(.08);
  auto resYaxis = res->GetYaxis();
  resYaxis->SetTitle("Norm. res.");
  resYaxis->SetTitleSize(.1); resYaxis->SetTitleOffset(.5);
  resYaxis->SetLabelSize(.08);

  pad2->Update();

  //c->SaveAs(figpath.c_str());
  c->Destructor();

  return results;

}
