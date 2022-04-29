
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

TH1D * CalibrateSpectrum(std::string treepath, std::string treename, std::string EnergyBranch,
                         int nbins, double LowLim, double UpLim,
                         double calConv, double calOffset) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data[nentries], Energy;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);

  for (int i=0; i<nentries; i++) {
    t->GetEntry(i);
    data[i] = (Energy - calOffset) / calConv;
  }

  auto h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  for (int i=0; i<nentries; i++) {
    if (Cut(data[i], UpLim, LowLim) == 1) h->Fill(data[i]);
  }

  return h;

}
