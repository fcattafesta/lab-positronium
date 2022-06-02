#include "scripts/plot.h"

void DrawHist(TCanvas* c, TH1 *h) {
  h->SetLineColor(kBlack); h->SetLineWidth(1);
  h->SetFillColorAlpha(46, 0.5);
  gStyle->SetOptStat(0); gStyle->SetStatBorderSize(0.);
  gStyle->SetStatX(1.); gStyle->SetStatY(1.);

  DrawDate(c);

  auto fitLegend = new TPaveText(.6, .7, .7, .8);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.03);
  fitLegend->SetTextAlign(11);

  auto sEntries = Form("Entries: %.0f events", h->GetEntries());
  auto sMean = Form("Mean: %.2f ns", h->GetMean());
  auto sDev = Form("Std. Dev.: %.2f ns", h->GetStdDev());
  fitLegend->AddText(sEntries);
  fitLegend->AddText(sMean);
  fitLegend->AddText(sDev);
  fitLegend->Draw();

  c->Update();
}
void TimeResolution() {

  std::string treepath[2] = {"data/1205/coinc_1_t.root",
                          "data/1205/coinc_2_t.root"};


  double time_1, time_2;
  int nbins = 20;
  double inf = -30, sup = -1 * inf;

  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;4");
  t1->SetBranchAddress("Time", &time_1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;4");
  t2->SetBranchAddress("Time", &time_2);

  auto nentries = t1->GetEntries();

  auto h = new TH1D("h", ";#Deltat_{21} [ns]", nbins, inf, sup);

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i);
    h->Fill(time_2 - time_1);
  }
  auto c = new TCanvas();
  h->Draw();
  DrawHist(c, h);
  c->SaveAs("figures/timeres.pdf");
}
