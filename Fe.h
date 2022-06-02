#include "scripts/spectrum.h"
#include "scripts/plot.h"

void DrawHist(TCanvas* c, TH1 *h, int nentries) {
  h->SetLineColor(kBlack); h->SetLineWidth(1);
  h->SetFillColorAlpha(46, 0.5);
  gStyle->SetOptStat(0); gStyle->SetStatBorderSize(0.);
  gStyle->SetStatX(1.); gStyle->SetStatY(1.);

  DrawDate(c);

  auto fitLegend = new TPaveText(.6, .6, .8, .8);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.03);
  fitLegend->SetTextAlign(11);

  auto sEntries = Form("After cut: %.0f events", h->GetEntries());
  auto sTotEntries = Form("Total sample: %.0d events", nentries);
  auto sMean = Form("Mean: %.2f #pm %.2f", h->GetMean(), h->GetMeanError());
  auto sStd = Form("Std. Dev.: %.2f #pm %.2f", h->GetStdDev(), h->GetStdDevError());
  auto sSkew = Form("Skewness: %.2f #pm %.2f ", h->GetSkewness(1), h->GetSkewness(11));
  fitLegend->AddText(sTotEntries);
  fitLegend->AddText(sEntries);
  fitLegend->AddText(sMean);
  fitLegend->AddText(sStd);
  fitLegend->AddText(sSkew);
  fitLegend->Draw();

  c->Update();
}

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

Bool_t reject = kTRUE;

TFitResultPtr Peak() {

  std::string treepath[3] = {"data/2605/pmt1/22Na.root",
                             "data/2605/pmt2/22Na.root",
                             "data/2605/pmt3/22Na.root"},

              fitfigpath[3] = {"figures/lifetime/peaks/annihilation_1.pdf",
                               "figures/lifetime/peaks/annihilation_2.pdf",
                               "figures/lifetime/peaks/neon_3.pdf"},
              elementname = "{}^{22}Na",
              treename = "tree;2", branchname = "EnergyTrap";

  auto h_3 = CalSpectrum(treepath[2], treename, branchname, 1, 100, 1.15e3, 1.5e3,
                         83.11, 0);

  auto f_bkg = [=](double *x, double *p) {
      if (reject && x[0]>=1250 && x[0]<=1400) {
      TF1::RejectPoint();
      return 0.;
    }
    return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
  };

  auto f_tot = [&](double *x, double *p) {
       return signal(x, p) + f_bkg(x, &p[3]);
    };

  TF1 *bkg = new TF1("bkg", f_bkg, 1.15e3, 1.25e3, 3);
  bkg->SetParameters(20, -.01, 0.);

  h_3->Fit(bkg, "SRLNI");

  reject = kFALSE;

  TF1 *fitFunc = new TF1("fitFunc", f_tot, 1.15e3, 1.5e3, 6);
  fitFunc->SetParameters(130, 1300, 50,
                         bkg->GetParameter(0), bkg->GetParameter(1),
                         bkg->GetParameter(2));

  auto results = h_3->Fit(fitFunc, "SRLNQI");

  DrawCalFits(h_3, fitfigpath[2], elementname, fitFunc, 100, 1.15e3, 1.5e3, results);

  reject = kTRUE;

  return results;



}

void Fe() {

  std::string treepath[3] = {"data/2605/Fe/Fe_1.root",
                             "data/2605/Fe/Fe_2.root",
                             "data/2605/Fe/Fe_3.root"};

  double energy_1, energy_2, energy_3, time_1, time_2, time_3;

  auto result = Peak();
  double e_gamma = result->GetParams()[1],
         e_sigma = result->GetParams()[2];

  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);
  t1->SetBranchAddress("EnergyTrap", &energy_1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);
  t2->SetBranchAddress("EnergyTrap", &energy_2);


  auto f3 = new TFile(treepath[2].c_str(), "READ");
  auto t3 = f3->Get<TTree>("tree;3");
  t3->SetBranchAddress("Time", &time_3);
  t3->SetBranchAddress("EnergyTrap", &energy_3);

  int nentries = t3->GetEntries();

  auto h_time = new TH1D("h_time", ";#Deltat_{12,3}[ns]", 50,
                         -2e3, 2e3);
  auto g1 = new TGraph();
  auto g2 = new TGraph();

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
    auto e1 = energy_1 / 37.89 ;
    auto e2 = energy_2 / 206.93;
    auto e3 = energy_3 / 83.11;
    if (e3 >= 1.2e3) {
      g1->AddPoint(e1, e2);
      if (time_1 <= 100e3 && time_2 <= 100e3 &&
          e1 >= 300 && e1 <= 600 &&
          e2 >= 300 && e2 <= 600)
          g2->AddPoint(time_1, time_2);
      if ((abs(time_2 - time_1) <= 40) &&
          e1 >= 300 && e1 <= 600 &&
          e2 >= 300 && e2 <= 600)
          h_time->Fill(-(time_3 - 0.5 * (time_1 + time_2)));
    }

  }

  const int n = h_time->GetEntries();
  int j = 0;
  double x[n];

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
    auto e1 = energy_1 / 37.89 ;
    auto e2 = energy_2 / 206.93;
    auto e3 = energy_3 / 83.11;
    if (e3 >= 1.2e3) {
      if ((abs(time_2 - time_1) <= 40) &&
          e1 >= 300 && e1 <= 600 &&
          e2 >= 300 && e2 <= 600) {
          x[j] = -(time_3 - 0.5 * (time_1 + time_2));
          j++;
        }
    }

  }

  int idx[n];
  double sorted[n];

  TMath::Sort(n, x, idx, kFALSE);

  for (int i=0; i<n; i++) {
    int index = idx[i];
    sorted[i] = x[index];
    cout<<sorted[i]<<endl;
  }

  auto fitLegend = new TPaveText(.15, .8, .25, .8);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.04);
  fitLegend->SetTextAlign(11);

  auto sCut = Form("E_{3} #geq %.0d keV",1200);
  fitLegend->AddText(sCut);

  auto c1 = new TCanvas(); c1->SetGrid();
  g1->Draw("AP");
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.3);
  g1->GetXaxis()->SetTitle("E_{1} [keV]");
  g1->GetXaxis()->SetLimits(0, 8e2);
  g1->GetYaxis()->SetTitle("E_{2} [keV]");
  g1->GetYaxis()->SetRangeUser(0, 8e2);
  DrawDate(c1);
  fitLegend->Draw();



  auto c2 = new TCanvas(); c2->SetGrid();
  g2->Draw("AP");
  g2->SetMarkerStyle(20);
  g2->SetMarkerSize(0.3);
  g2->GetXaxis()->SetTitle("t_{1} [ns]");
  g2->GetXaxis()->SetLimits(1e3, 2.5e3);
  g2->GetYaxis()->SetTitle("t_{2} [ns]");
  g2->GetYaxis()->SetRangeUser(1e3, 2.5e3);
  DrawDate(c2);
  auto text = new TPaveText(.15, .7, .25, .8);
  text->SetOption("NDC NB");
  text->SetFillStyle(0);
  text->SetBorderSize(0.);
  text->SetTextFont(42);
  text->SetTextSize(.04);
  text->SetTextAlign(11);

  text->AddText("E_{3} #geq 1200 keV");
  text->AddText("300 keV #leq E_{1}, E_{2} #leq 600 keV ");
  text->Draw();
  g2->GetXaxis()->SetLimits(2e3, 2.2e3);
  g2->GetYaxis()->SetRangeUser(2e3, 2.2e3);


  auto c3 = new TCanvas(); c3->SetGrid(); c3->SetLogy(0);
  h_time->Draw();
  DrawHist(c3, h_time, nentries);
  c3->SetLogy();

  c1->SaveAs("figures/lifetime/Fe/e1vse2.pdf");
  c2->SaveAs("figures/lifetime/Fe/t1vst2.pdf");
  c2->SaveAs("figures/lifetime/Fe/t1vst2_zoom.pdf");
  c3->SaveAs("figures/lifetime/Fe/lifetime.pdf");


}
