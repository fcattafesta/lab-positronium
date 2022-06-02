#include "scripts/spectrum.h"
#include "scripts/plot.h"

int TimeCut(double data, double inf, double sup) {

  if (data < inf || data > sup) return -1;
  return 1;
}

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

  auto sEntries = Form("After cut: %.0f events", h->GetEntries());
  fitLegend->AddText("Total sample: 254 events");
  fitLegend->AddText(sEntries);
  fitLegend->Draw();

  c->Update();
}


void CoincCut() {

  std::string treepath[2] = {"data/2605/coinc_1.root",
                             "data/2605/coinc_2.root"},

              elementname = "{}^{22}Na",
              treename = "tree;3", branchname = "EnergyTrap";

  double data1, data2,
         Energy1, Energy2,
         time_1, time_2;

  double a1 = 37.89, a2 = 206.05;

  int nevents = 0;

  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);
  t1->SetBranchAddress("EnergyTrap", &Energy1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);
  t2->SetBranchAddress("EnergyTrap", &Energy2);

  auto g = new TGraph();
  auto g_t = new TGraph();

  auto nentries = t1->GetEntries();

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i);
    if (TimeCut(time_1, -1e6, 1e6) == 1 && TimeCut(time_2, -1e6, 1e6) == 1)
      g_t->AddPoint(time_1, time_2);
    if (TimeCut(time_2 - time_1, -30, 20) == 1) {
        g->AddPoint(Energy1/a1, Energy2/a2);
        if (TimeCut(Energy1/a1, 300, 600) == 1 && TimeCut(Energy2/a2, 300, 600) == 1)
        nevents++;
      }
  }

  auto c = new TCanvas(); c->SetGrid();
  g->Draw("AP");
  g->SetMarkerStyle(1);
  g->SetMarkerSize(0.6);
  auto xaxis = g->GetXaxis(); xaxis->SetTitle("E_{1} [keV]");
  xaxis->SetLimits(0, 2e3);
  auto yaxis = g->GetYaxis(); yaxis->SetTitle("E_{2} [keV]");
  yaxis->SetTitleOffset(1.4);
  yaxis->SetRangeUser(0, 2e3);
  DrawDate(c);

  auto c_t = new TCanvas(); c_t->SetGrid();
  g_t->Draw("AP");
  auto xaxis_t = g_t->GetXaxis(); xaxis_t->SetTitle("t_{1} [ns]");
  //xaxis_t->SetLimits(0, 2e3);
  auto yaxis_t = g_t->GetYaxis(); yaxis_t->SetTitle("t_{2} [ns]");
  yaxis_t->SetTitleOffset(1.4);
  //yaxis_t->SetRangeUser(0, 2e3);
  DrawDate(c_t);

  cout<< nevents << endl;



}
