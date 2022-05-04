
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

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);

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



double StatEnergyError(std::string treepath, std::string treename, std::string EnergyBranch,
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
                         std::string "EnergyError", int nbins, double LowLim, double UpLim,
                         double calConv, double calOffset) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data, Error_data, Energy, EnergyError;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);
  t->SetBranchAddress("EnergyError", &EnergyError);


  auto h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);
  auto dh =  = new TH1D("h", "; dEnergy [a.u.]", nbins, LowLim, UpLim);
  auto res[2] = [h, dh];

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data = (Energy - calOffset[0]) / calConv[0];
    Error_data = ((EnergyError + calOffset[1])/(Energy - calOffset[0])) + (calConv[1]/calConv[0]);
    if (Cut(data[i], UpLim, LowLim) == 1) {
      h->Fill(data);
      dh->Fill(Error_data);
  }

  return res;

}
