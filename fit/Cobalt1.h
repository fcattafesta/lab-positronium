#include "Caesium.h"

Bool_t reject1 = kTRUE;

double background1(double *x, double *p) {
  if (reject1 && x[0] >= 46e3 && x[0] <= 60e3) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double signal1(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func1(double *x, double *p) {
  return signal1(x, p) + background1(x, &p[3]);
}

TFitResultPtr Cobalt1() {

  std::string treepath = "data/60Co.root",
              figpath = "figures/fit/60Co_1.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{60}Co";

  double LowLim = 40e3, UpLim = 53e3;
  double fitMin = 40e3, fitMax = 53e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", background1, fitMin, fitMax, 3);
  bkg->SetParameters(40, -.2, 1e-5);

  h->Fit(bkg, "SRLNQ");

  reject1 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", func1, fitMin, fitMax, 6);
  fitFunc->SetParameters(.2e3, 49e3, 1.1e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

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

  auto legend = DrawLegendFit(c, .1, .65, .35, .85, h,
                           fitMin, fitMax, fitFunc, results);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);

  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  xaxis->SetLimits(LowLim, UpLim);
  pad2->cd(); pad2->SetGrid();

  auto res = new TGraph();

  for (int i=1; i<=nbins; i++) {
    auto diff = (h->GetBinContent(i) - fitFunc->Eval(h->GetBinCenter(i))) /
                h->GetBinError(i);
    res->AddPoint(h->GetBinCenter(i), diff);
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
  resXaxis->SetTitle("Energy [keV]");
  resXaxis->SetTitleSize(.1); resXaxis->SetTitleOffset(.9);
  resXaxis->SetLabelSize(.08);
  auto resYaxis = res->GetYaxis();
  resYaxis->SetTitle("Norm. res.");
  resYaxis->SetTitleSize(.1); resYaxis->SetTitleOffset(.5);
  resYaxis->SetLabelSize(.08);

  c->SaveAs(figpath.c_str());
  c->Destructor();

  return results;
}
