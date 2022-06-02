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

  auto sEntries = Form("Entries: %.0f", h->GetEntries());
  auto sTotEntries = Form("Mean: %.0f", h->GetMean());
  fitLegend->AddText(sEntries);
  fitLegend->AddText(sTotEntries);

  fitLegend->Draw();

  c->Update();
}

void Compton() {

  std::string treepath = "data/1005/pmt1/22Na.root";

  double energy, a = 42.63, inf = 100, sup = 380, x[9];
  int nbins = 50, nevents = 9;

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2");
  t->SetBranchAddress("EnergyTrap", &energy);

  int nentries = t->GetEntries();

  auto h = new TH1D("h", ";Energy [keV]", nbins, inf, sup);
  auto h_tot = new TH1D("h_tot", ";Energy [keV]", nbins, 0, 2e3);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    auto tmp = (340./511) * (energy/a);
    h->Fill(tmp);
  }

  for (int i=0; i<nevents; i++) {
    auto r1 = h->GetRandom();
    auto r2 = h->GetRandom();
    auto r3 = h->GetRandom();
    h_tot->Fill(r1 + r2 + r3);
    x[i] = r1 + r2 + r3;
  }

  for (int i=0; i<nevents; i++) {
    cout << x[i] << endl;
  }

  auto c = new TCanvas();
  h_tot->Draw();
  DrawHist(c, h_tot);
  c->SaveAs("figures/triple/tot_simul.pdf");

  auto c1 = new TCanvas() ;
  h->Draw();
  DrawHist(c, h);
  c1->SaveAs("figures/triple/sim_spectrum.pdf");


}
