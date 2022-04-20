#include "utilities.h"

void analysis() {

  std::string filepath = "data/raw/132Cs_calibration_1204.txt", treepath = "data/132Cs.root";
  std::string figpath = "figures/132CsImproved.pdf";

  MakeTree(filepath, treepath);
  ComputeEnergyImproved(treepath);

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2");

  int nentries = t->GetEntries();
  double data[nentries], Energy;

  t->SetBranchAddress("Energy", &Energy);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data[i] = Energy;
  }

  int nbins = 80;
  double maxEnergy = TMath::MaxElement(nentries, data);
  double cut = 50e3;
  //double cut = maxEnergy;

  auto h = new TH1D("h", "; Energy [a.u.]", nbins, 0., cut);

  for (int i=0; i<nentries; i++) {
    if (data[i] <= cut) h->Fill(data[i]);
  }

  auto c1 = new TCanvas();
  h->Draw();
  HistStyle(h);
  c1->SaveAs(figpath.c_str());
  //auto c2 = new TCanvas();
  //DrawSignal(treepath, 1);

}
