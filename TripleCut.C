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

  auto sEntries = Form("After cut: %.0f events", h->GetEntries());
  fitLegend->AddText("Total sample: 254 events");
  fitLegend->AddText(sEntries);
  fitLegend->Draw();

  c->Update();
}

int Cut(double data, double inf, double sup) {

  if (data < inf || data > sup) return -1;
  return 1;
}

void TripleCut() {

  std::string treepath[6] = {"data/2405/triple_1.root",
                             "data/2405/triple_2.root",
                             "data/2405/triple_3.root",
                             "data/1705/triple_1.root",
                             "data/1705/triple_2.root",
                             "data/1705/triple_3.root"};

  double a1 = 40, a2 = 223.38, a3 = 88.85 , a4 = 42.3, a5 = 206.75, a6 = 86.92,
         da1 = 0.2, da2 = 8.62, da3 = 3.56;
  double b1 = 42.3, b2 = 206.75, b3 = 86.92;

  double data1, data2, data3, data4, data5, data6,
         Energy1, Energy2, Energy3, Energy4, Energy5, Energy6,
         time_1, time_2, time_3, time_4, time_5, time_6;

  double coinc[9], outcoinc[13];

  int j = 0, k = 0;

  int nbins = 50;
  double inf = 0, sup = 2e3;
  double EnergyLow = 1e2,
         EnergyUp = 4.5e2;

  int n17 = 0, n24 = 0;


  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);
  t1->SetBranchAddress("EnergyTrap", &Energy1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);
  t2->SetBranchAddress("EnergyTrap", &Energy2);


  auto f3 = new TFile(treepath[2].c_str(), "READ");
  auto t3 = f3->Get<TTree>("tree;3");
  t3->SetBranchAddress("Time", &time_3);
  t3->SetBranchAddress("EnergyTrap", &Energy3);

  auto f4 = new TFile(treepath[3].c_str(), "READ");
  auto t4 = f4->Get<TTree>("tree;3");
  t4->SetBranchAddress("Time", &time_4);
  t4->SetBranchAddress("EnergyTrap", &Energy4);

  auto f5 = new TFile(treepath[4].c_str(), "READ");
  auto t5 = f5->Get<TTree>("tree;3");
  t5->SetBranchAddress("Time", &time_5);
  t5->SetBranchAddress("EnergyTrap", &Energy5);

  auto f6 = new TFile(treepath[5].c_str(), "READ");
  auto t6 = f6->Get<TTree>("tree;3");
  t6->SetBranchAddress("Time", &time_6);
  t6->SetBranchAddress("EnergyTrap", &Energy6);


  int nentries = t1->GetEntries();

  TH1D * h = new TH1D("h", "Total Energy (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t = new TH1D("h_t", "Total Energy (out-of-coincidence); Energy [keV]",
                        nbins, inf, sup);

  TH1D * h_1 = new TH1D("h_1", "pmt1 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t1 = new TH1D("h_t1", "pmt1 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);

  TH1D * h_2 = new TH1D("h_2", "pmt2 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t2 = new TH1D("h_t2", "pmt2 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);

  TH1D * h_3 = new TH1D("h_3", "pmt3 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t3 = new TH1D("h_t3", "pmt3 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);


  auto g = new TGraph();
  auto g_t = new TGraph();

    for (int i=0; i<nentries; i++) {
      t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
      data1 = Energy1 / a1;
      data2 = Energy2 / a2;
      data3 = Energy3 / a3;
      auto time_21 = time_2 - time_1;
      auto time_31 = time_3 - time_1;
      auto time_32 = time_3 - time_2;

      if (Cut(time_21, -1e6, 1e6) == 1 && Cut(time_31, -1e6, 1e6) == 1 &&
          Cut(time_32, -1e6, 1e6) == 1) {
            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
            g->AddPoint(time_21, time_31);
            g_t->AddPoint(time_21, time_32);
          }
          }
      if (Cut(time_21, -30, 20) == 1 && Cut(time_31, -40, 20) == 1 &&
          Cut(time_32, -20, 25) == 1) {

            if (Cut(data1, inf, sup) == 1) h_1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_2->Fill(data2);

            if (Cut(data3, inf, sup) == 1) h_3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
                h->Fill(data1 + data2 + data3);
                coinc[k] = data1 + data2 + data3;
                k++;
                n17++;
              }
      }
      if ((Cut(time_21, -30, 20) == -1 || Cut(time_31, -40, 20) == -1 ||
          Cut(time_32, -20, 25) == -1) &&
        (Cut(time_21, -1.5e3, 1.5e3) == 1 && Cut(time_31, -6e2, 1.5e3) == 1 &&
            Cut(time_32, -6e2, 1.5e3) == 1)) {

            if (Cut(data1, inf, sup) == 1) h_t1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_t2->Fill(data2);

            if (Cut(data3, inf, sup) == 1 ) h_t3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
                h_t->Fill(data1 + data2 + data3);
                outcoinc[j] = data1 + data2 + data3;
                j++;
              }

      }
    }

    nentries = t4->GetEntries();

    for (int i=0; i<nentries; i++) {
      t4->GetEntry(i); t5->GetEntry(i); t6->GetEntry(i);
      data1 = Energy4 / a4;
      data2 = Energy5 / a5;
      data3 = Energy6 / a6;
      auto time_21 = time_5 - time_4;
      auto time_31 = time_6 - time_4;
      auto time_32 = time_6 - time_5;

      if (Cut(time_21, -1e6, 1e6) == 1 && Cut(time_31, -1e6, 1e6) == 1 &&
          Cut(time_32, -1e6, 1e6) == 1)  {
            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
            g->AddPoint(time_21, time_31);
            g_t->AddPoint(time_21, time_32);
          }
          }

      if (Cut(time_21, -30, 20) == 1 && Cut(time_31, -40, 20) == 1 &&
          Cut(time_32, -20, 25) == 1) {

            if (Cut(data1, inf, sup) == 1) h_1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_2->Fill(data2);

            if (Cut(data3, inf, sup) == 1) h_3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
                h->Fill(data1 + data2 + data3);
                coinc[k] = data1 + data2 + data3;
                k++;
                n24++;
              }
      }
      if ((Cut(time_21, -30, 20) == -1 || Cut(time_31, -40, 20) == -1 ||
          Cut(time_32, -20, 25) == -1) &&
        (Cut(time_21, -1.5e3, 1.5e3) == 1 && Cut(time_31, -6e2, 1.5e3) == 1 &&
            Cut(time_32, -6e2, 1.5e3) == 1)) {

            if (Cut(data1, inf, sup) == 1) h_t1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_t2->Fill(data2);

            if (Cut(data3, inf, sup) == 1 ) h_t3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1) {
                h_t->Fill(data1 + data2 + data3);
                outcoinc[j] = data1 + data2 + data3;
                j++;
              }
      }
    }

    auto line1 = new TF1("line1", "pol0", -2.5e3, 1.5e3);
    auto line2 = new TF1("line2", "pol0", -2.5e3, 1.5e3);

    auto c = new TCanvas();
    h->Draw();
    DrawHist(c, h);
    c->SaveAs("figures/triple/tot.pdf");
    auto c_t = new TCanvas();
    h_t->Draw();
    DrawHist(c_t, h_t);
    c_t->SaveAs("figures/triple/tot_t.pdf");


    auto c1 = new TCanvas();
    h_1->Draw();
    DrawHist(c1, h_1);
    c1->SaveAs("figures/triple/pmt1.pdf");

    auto c_t1 = new TCanvas();
    h_t1->Draw();
    DrawHist(c_t1, h_t1);
    c_t1->SaveAs("figures/triple/pmt1_t.pdf");


    auto c2 = new TCanvas();
    h_2->Draw();
    DrawHist(c2, h_2);
    c2->SaveAs("figures/triple/pmt2.pdf");

    auto c_t2 = new TCanvas();
    h_t2->Draw();
    DrawHist(c_t2, h_t2);
    c_t2->SaveAs("figures/triple/pmt2_t.pdf");


    auto c3 = new TCanvas();
    h_3->Draw();
    DrawHist(c3, h_3);
    c3->SaveAs("figures/triple/pmt3.pdf");

    auto c_t3 = new TCanvas();
    h_t3->Draw();
    DrawHist(c_t3, h_t3);
    c_t3->SaveAs("figures/triple/pmt3_t.pdf");

    auto c4 = new TCanvas(); c4->SetGrid();
    auto pt = new TPaveText(.0, .95, .2, 1);
    pt->SetOption("NDC NB");
    pt->SetFillColor(0);
    pt->SetTextFont(42);
    pt->SetTextSize(.03);

    auto today = new TDatime();
    auto sDate = today->AsSQLString();
    pt->AddText(sDate);


    g->Draw("AP");
    line1->SetParameter(0,-600);
    line1->Draw("AL SAME");
    line1->SetLineColor(kRed);
    line1->SetLineStyle(2);
    line2->SetParameter(0, 1500);
    line2->Draw("AL SAME");
    line2->SetLineColor(kRed);
    line2->SetLineStyle(2);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(0.6);
    auto xaxis = g->GetXaxis(); xaxis->SetTitle("#Deltat_{21} [ns]");
    auto yaxis = g->GetYaxis(); yaxis->SetTitle("#Deltat_{31} [ns]");
    yaxis->SetTitleOffset(1.4);
    pt->Draw();
    c4->SaveAs("figures/triple/21vs31.pdf");

    auto c5 = new TCanvas(); c5->SetGrid(); DrawDate(c5);
    g_t->Draw("AP");
    line1->SetParameter(0,-600);
    line1->Draw("AL SAME");
    line1->SetLineColor(kRed);
    line1->SetLineStyle(2);
    line2->SetParameter(0, 1500);
    line2->Draw("AL SAME");
    line2->SetLineColor(kRed);
    line2->SetLineStyle(2);
    g_t->SetMarkerStyle(20);
    g_t->SetMarkerSize(0.6);
    auto xaxis_t = g_t->GetXaxis(); xaxis_t->SetTitle("#Deltat_{21} [ns]");
    auto yaxis_t = g_t->GetYaxis(); yaxis_t->SetTitle("#Deltat_{32} [ns]");
    yaxis_t->SetTitleOffset(1.4);
    pt->Draw();
    c5->SaveAs("figures/triple/21vs32.pdf");

    int idx[9];

    for (int i=0; i<13; i++) {
      cout << outcoinc[i] << endl;
    }

    TMath::Sort(9, coinc, idx, kFALSE);

  cout << n17 << "-----" << n24 << endl;
}
