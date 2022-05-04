#include "dataset.h"

void EnergyIntegral(std::string treepath, int low, int up) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy;

  auto bEnergy = t->Branch("EnergyIntegral", &Energy);

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

    Energy = (height * (up-low)) - signal->Integral(low, up, 1e-3);
    bEnergy->Fill();
  }
  t->Write();
  f->Close();
}

void EnergyTrap(std::string treepath, int low, int up) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy, dEnergy, dEnergy_2;

  auto bEnergy = t->Branch("EnergyTrap", &Energy);
  auto EnergyError = t->Branch("EnergyError", &dEnergy);
  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    Energy = 0; dEnergy = 0; dEnergy_2=0;

    TF1 *base = new TF1("base", "[0]", 0, 200);
    g->Fit(base, "RQN");

    double height = base->GetParameter(0);
    double dh = base->GetParError(0);

    for (int j=low; j<up-1; j++) {
      Energy = Energy + height - (v[j] + v[j+1])/2;
      dEnergy_2 = dEnergy_2 + 2*dh*dh;
    }

    dEnergy = TMath::Sqrt(dEnergy_2);
    bEnergy->Fill();
    EnergyError->Fill();
  }

  t->Write();
  f->Close();
}

void EnergyMinAmp(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];

  double Energy, dEnergy;

  auto bEnergy = t->Branch("EnergyMinAmp", &Energy);
  auto EnergyError = t->Branch("EnergyError", &dEnergy);
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
    dEnergy = TMath::Sqrt(2)*base->GetParError(0);
    bEnergy->Fill();
    EnergyError->Fill();

  }
  t->Write();
  f->Close();
}

void EnergyThr(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy, dEnergy;

  auto bEnergy = t->Branch("EnergyThr", &Energy);
  auto EnergyError = t->Branch("EnergyError", &dEnergy);

  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();

    double dE1 = 0, dE2 = 0;

    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    TF1 *base = new TF1("base", "[0]", 0, 200);
    g->Fit(base, "RQN");

    double Amp = base->GetParameter(0) - TMath::MinElement(RECORD_LENGTH, g->GetY());
    double dv = base->GetParError(0);
    //double thr = Amp - 0.1 * Amp;
    double thr = base->GetParameter(0) - 0.01 * base->GetParameter(0);
    int t1 = 0, t2 = 0;

    for (int k=0; k<RECORD_LENGTH-1; k++) {
      if (v[k] - dv > thr && v[k+1] + dv <= thr) {
        if (t1==0) t1 = k+1;
        dE1 = dE1 + dv/TMath::Abs(v[k+1]-v[k]);
        //cout << k+1 << endl;
      }

      if (v[k] + dv <= thr && v[k+1] - dv > thr) {
        t2 = k;
        dE2 = dE2 + dv/TMath::Abs(v[k+1]-v[k]);
      }
    }

    Energy = t2 - t1;
    dEnergy = TMath::Sqrt(dE1*dE1 + dE2*dE2);
    //cout << Energy << endl;
    bEnergy->Fill();

  }
  t->Write();
  f->Close();
}
