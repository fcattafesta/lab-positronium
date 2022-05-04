#include "../scripts/spectrum.h"
#include "../scripts/plot.h"

Bool_t reject0 = kTRUE;

double background0(double *x, double *p) {
  if (reject0 && x[0]>=25e3 && x[0]<=31.5e3) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double signal0(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func0(double *x, double *p) {
  return signal0(x, p) + background0(x, &p[3]);
}

TFitResultPtr Caesium() {

  std::string treepath = "data/137Cs.root",
              figpath = "figures/fit/137Cs.pdf",
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname = "{}^{137}Cs";

  double LowLim = 22e3, UpLim = 32e3;
  double fitMin = 22e3, fitMax = 32e3;
  int nbins = 100;

  auto h = MakeSpectrum(treepath, treename, branchname, nbins, LowLim, UpLim);

  TF1 *bkg = new TF1("bkg", background0, fitMin, fitMax, 3);
  bkg->SetParameters(100, .03, 1.e-7);

  h->Fit(bkg, "SRLNQ");

  reject0 = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", func0, fitMin, fitMax, 6);
  fitFunc->SetParameters(2.5e3, 28e3, .8e3,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h->Fit(fitFunc, "SRLNQI");

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

  auto legend = DrawLegendFit(c, .1, .65, .55, .85, h,
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

  cout << results->Prob() << endl;
  return results;

}
