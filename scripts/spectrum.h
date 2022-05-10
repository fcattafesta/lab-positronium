
int Cut(double Energy, double UpLim, double LowLim) {

  if (Energy < LowLim && Energy > UpLim) return -1;
  return 1;
}

// Be careful on tree cycle value: it should be the last value

TH1D * MakeSpectrum(std::string treepath, std::string treename, std::string EnergyBranch,
                    int nbins, double LowLim, double UpLim) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data[nentries], Energy;

  auto h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);

  for (int ev=0; ev<nentries; ev++) {
    t->GetEntry(ev);
    data[ev] = Energy;
    if (Cut(data[ev], UpLim, LowLim) == 1) h->Fill(data[ev]);
  }

  return h;

}



double SystEnergyError(std::string treepath, std::string treename, std::string EnergyBranch,
                      std::string EnergyErrorBranch, double LowLim, double UpLim) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str());

  int nentries = 0;
  double Energy, EnergyError;

  double ErrInt=0.;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);
  t->SetBranchAddress(EnergyErrorBranch.c_str(), &EnergyError);

  for (int i=0; i<t->GetEntries(); i++) {
    if (Cut(Energy, UpLim, LowLim) == 1) {
      t->GetEntry(i);
      ErrInt = ErrInt + EnergyError;
      nentries++;
    }
  }

  ErrInt = ErrInt/nentries;
  return ErrInt;
}

TH1D * CalibrateSpectrum(std::string treepath, std::string treename, std::string EnergyBranch,
                        int nbins, double LowLim, double UpLim, double cte, double slope){

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data, Energy;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);


  TH1D * h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data = (Energy - cte) / slope;
    if (Cut(data, UpLim, LowLim) == 1) h->Fill(data);
  }

  return h;

}


TH1D * CalibrateSpectrum_var(std::string treepath, std::string treename, std::string EnergyBranch,
                        int nbins, double LowLim, double UpLim, double a, double b){

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data, Energy;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);


  TH1D * h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);

    data = (0.5/a)*(-b + TMath::Sqrt(b*b+4*a*Energy));
    if (Cut(data, UpLim, LowLim) == 1) h->Fill(data);
  }

  return h;

}


TH1D * CalibrationError(std::string treepath, std::string treename, std::string EnergyBranch,
                        std::string EnergyErrorBranch, int nbins, double LowLim, double UpLim,
                        double b, double db, double a, double da, double corr){

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data, Energy, Error_data, EnergyError;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);
  t->SetBranchAddress(EnergyErrorBranch.c_str(), &EnergyError);

  TH1D * dh = new TH1D("dh", "; dEnergy [a.u.]", nbins, 0, 1000);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data = (Energy - b) / a;
    Error_data = TMath::Sqrt( pow((Energy-b)*da/(a*a), 2) + pow((db/a), 2) - 2*(Energy-b)*corr*da*db/(a*a*a) + pow((EnergyError/a), 2) );
    if (Cut(data, UpLim, LowLim) == 1) {
      dh->Fill(Error_data);
    }
  }

  return dh;

}
