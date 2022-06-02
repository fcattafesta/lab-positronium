#include "scripts/time.h"

void ComputeTime() {

  std::string treepath[2] = {"data/2605/coinc_1.root",
                             "data/2605/coinc_2.root",
                             };

  double time_1, time_2;
  int nbins = 50;
  double inf = -1e3, sup = 1e3;



  for (int i=0; i<2; i++){
    cout << treepath[i] << endl;
    Time(treepath[i]);
    cout << "Finished" << endl;
  }
/*
  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);

  int n1 = t1->GetEntries(), n2 = t2->GetEntries();

  if (n1!=n2) cout << "Different entry number" << endl;

  int nentries = n1;

  auto h = new TH1D("h", ";#DeltaT_{12} [ns]", nbins, inf, sup);

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i);
    auto delta = time_2 - time_1;
    if (delta >= inf && delta <= sup) h->Fill(delta);
  }

  h->Draw();
  h->Fit("gaus", "L");
*/

}
