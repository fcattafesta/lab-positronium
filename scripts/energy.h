#include "dataset.h"

void EnergyTrap(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy;

  auto bEnergy = t->Branch("EnergyTrap", &Energy);
  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    TF1 *signal = new TF1("signal",[&](double *x, double *){ return g->Eval(x[0]);},
                          0, RECORD_LENGTH-1, 0);
    TF1 *base = new TF1("base", "[0]", 0, 200);

    g->Fit(base, "RQN");
    double height = base->GetParameter(0);

    Energy = (height * RECORD_LENGTH) - signal->Integral(0, RECORD_LENGTH-1, 1e-3);
    bEnergy->Fill();
  }
  t->Write();
  f->Close();
}

void EnergyMinAmp(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy;

  auto bEnergy = t->Branch("EnergyMinAmp", &Energy);
  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    TF1 *base = new TF1("base", "[0]", 0, 200);

    g->Fit(base, "RQN");

    Energy = base->GetParameter(0) - TMath::MinElement(RECORD_LENGTH, g->GetY());
    bEnergy->Fill();
  }
  t->Write();
  f->Close();
}

void EnergyThr(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy;

  auto bEnergy = t->Branch("EnergyThr", &Energy);
  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();

    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    TF1 *base = new TF1("base", "[0]", 0, 200);
    g->Fit(base, "RQN");

    double Amp = base->GetParameter(0) - TMath::MinElement(RECORD_LENGTH, g->GetY());
    double thr = 0.1 * Amp;
    int t1=1, t2=10;

    for (int k=0; k<RECORD_LENGTH-1; k++) {
      if (v[k] > thr && v[k+1] <= thr) {
        t1 = k+1;
        cout << k+1 << endl;}
      if (v[k] <= thr && v[k+1] > thr) t2 = k;
    }

    Energy = t2 - t1;
    //cout << Energy << endl;
    bEnergy->Fill();

  }
  t->Write();
  f->Close();
}
