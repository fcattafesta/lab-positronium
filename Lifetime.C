#include "scripts/spectrum.h"
#include "scripts/plot.h"

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
                         81.82, 0);

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

void Lifetime() {

  std::string treepath[3] = {"data/2605/Gel/Gel_1.root",
                             "data/2605/Gel/Gel_2.root",
                             "data/2605/Gel/Gel_3.root"};

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

  auto h_time = new TH1D("h_time", ";t[ns]", 30, -.01e3, 2e3);
  auto g1 = new TGraph();
  auto g2 = new TGraph();

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
    auto e1 = energy_1 / 37.94 ;
    auto e2 = energy_2 / 211.84;
    auto e3 = energy_3 / 81.8;
    if (e3 >= 1.2e3) {
      g1->AddPoint(e1, e2);
      if (time_1 <= 100e3 && time_2 <= 100e3 &&
          e1 >= 300 && e1 <= 600 &&
          e2 >= 300 && e2 <= 600)
          g2->AddPoint(time_1, time_2);
      if (time_1 >= 2e3 && time_1 <= 2.2e3 &&
          time_2 >= 2e3 && time_2 <= 2.2e3 &&
          e1 >= 300 && e1 <= 600 &&
          e2 >= 300 && e2 <= 600)
          h_time->Fill(-(time_3 - 0.5 * (time_1 + time_2)));
    }

  }

  auto c1 = new TCanvas(); c1->SetGrid();
  g1->Draw("AP");
  g1->GetXaxis()->SetTitle("Energy 1");
  g1->GetXaxis()->SetLimits(0, 800);
  g1->GetYaxis()->SetTitle("Energy 2");
  g1->GetYaxis()->SetRangeUser(0, 800);
  c1->SaveAs("figures/lifetime/Gel/e1vse2.pdf");


  auto c2 = new TCanvas(); c2->SetGrid();
  g2->Draw("AP");
  g2->GetXaxis()->SetTitle("Time 1");
  g2->GetYaxis()->SetTitle("Time 2");
  c2->SaveAs("figures/lifetime/Gel/e1vse2.pdf");

  auto c3 = new TCanvas(); c3->SetGrid(); c3->SetLogy(0);
  h_time->Draw();
  h_time->Fit("expo", "L");
  c3->SetLogy();
  c2->SaveAs("figures/lifetime/Gel/lifetime.pdf");


}
