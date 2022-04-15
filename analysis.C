#include "utilities.h"

void analysis() {

  std::string filepath = "data/22Na_calibration_1204.txt", treepath = "data/22Na.root";
  std::string figpath = "figures/22Na.pdf";

  MakeTree(filepath, treepath);
  ComputeEnergy(treepath);

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
  double cut = 90e3;

  auto h = new TH1D("h", "; Energy [a.u.]", nbins, 0., cut);

  for (int i=0; i<nentries; i++) {
    if (data[i] <= cut) h->Fill(data[i]);
  }

  auto c1 = new TCanvas();
  h->Draw();
  HistStyle(h);
  c1->SaveAs(figpath.c_str());
  /*auto c2 = new TCanvas();
  DrawSignal(treepath, 1); */

}
