#include "../scripts/spectrum.h"
#include "../scripts/plot.h"

Bool_t reject = kTRUE;

double background(double *x, double *p) {
  if (reject && x[0]>=540 && x[0]<=550) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func(double *x, double *p) {
  return signal(x, p) + background(x, &p[3]);
}

TFitResultPtr Sodium() {

  std::string treepath = "data/22Na.root",
              figpath = "figures/fit/22Na_pol2.pdf",
              branchname = "EnergyTrap",
              treename = "tree;2",
              elementname = "{}^{22}Na";

  double calConv = 41.48, calOffset = 1036;

  double LowLim = .35e3, UpLim = .6e3;
  double fitMin = .35e3, fitMax = .6e3;
  int nbins = 100;

  auto h = CalibrateSpectrum(treepath, treename, branchname, nbins,
                              LowLim, UpLim, calConv, calOffset);
  auto dh = CalibrationError(treepath, treename, branchname, "EnergyError",
                              nbins, LowLim, UpLim, cte, d_cte, slope, d_slope, corr);


  TF1 *bkg = new TF1("bkg", background, fitMin, fitMax, 3);
  bkg->SetParameters(.3e3, -0.3, .3);

  h->Fit(bkg, "SRLNQ");

  reject = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", func, fitMin, fitMax, 6);
  fitFunc->SetParameters(1.2e3, .5e3, .02e3,
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

  auto legend = DrawLegendFit(c, .1, .65, .4, .85, h,
                           fitMin, fitMax, fitFunc, results);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [u.a.]");
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


  return results;

}
