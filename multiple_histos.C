#include "scripts/utilities.h"

void multiple_histos() {

  std::string treepath = "data/137Cs.root", figpath = "figures/241Am.pdf";

  double LowLim = 20e3;
  double UpLim = 50e3;
  int nbins = 200;

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2");

  int nentries = t->GetEntries();
  double data[nentries], Energy;

  t->SetBranchAddress("Energy", &Energy);



  auto h1 = new TH1D("h1", "; Energy [a.u.]", nbins, LowLim, UpLim);
  auto h2 = new TH1D("h2", "; Energy [a.u.]", nbins, LowLim, UpLim);
  auto h3 = new TH1D("h3", "; Energy [a.u.]", nbins, LowLim, UpLim);
  auto h4 = new TH1D("h4", "; Energy [a.u.]", nbins, LowLim, UpLim);
  auto h5 = new TH1D("h5", "; Energy [a.u.]", nbins, LowLim, UpLim);


  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    if (Cut(data[i], UpLim, LowLim) == 1) {
      if (i%5==0) h1->Fill(Energy);
      if (i%5==1) h2->Fill(Energy);
      if (i%5==2) h3->Fill(Energy);
      if (i%5==3) h4->Fill(Energy);
      if (i%5==4) h5->Fill(Energy);
    }
  }

  TF1 *peak = new TF1("peak", "gaus", 41.5e3, 46e3);

  int p1, p2, p3, p4, p5;

  auto c1 = new TCanvas();
  auto res1 = h1->Fit(peak, "SRLNQ"); p1 = peak->GetParameter(1);
  auto res2 = h2->Fit(peak, "SRLNQ"); p2 = peak->GetParameter(1);
  auto res3 = h3->Fit(peak, "SRLNQ"); p3 = peak->GetParameter(1);
  auto res4 = h4->Fit(peak, "SRLNQ"); p4 = peak->GetParameter(1);
  auto res5 = h5->Fit(peak, "SRLNQ"); p5 = peak->GetParameter(1);

  int v[5] = {p1, p2, p3, p4, p5};

  cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << endl;
  cout << "Media = " << TMath::Mean(5, v) << endl;
  cout << "Dev. Std. = " << TMath::RMS(5, v) << endl;


}
