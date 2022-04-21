#include "utilities.h"

int Cut(double Energy, double UpLim, double LowLim) {

  if (Energy < LowLim && Energy > UpLim) return -1;
  return 1;
}

TH1D * MakeSpectrum(std::string treepath, int nbins, double LowLim, double UpLim) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2");

  int nentries = t->GetEntries();
  double data[nentries], Energy;

  t->SetBranchAddress("Energy", &Energy);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data[i] = Energy;
  }

  auto h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  for (int i=0; i<nentries; i++) {
    if (Cut(data[i], UpLim, LowLim) == 1) h->Fill(data[i]);
  }

  return h;

}
