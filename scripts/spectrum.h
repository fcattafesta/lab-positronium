
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


TH1D * CalSpectrum_linear(std::string treepath, std::string treename, std::string EnergyBranch,
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


TH1D * CalSpectrum_quadratic(std::string treepath, std::string treename, std::string EnergyBranch,
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


double CalibrationError(double Energy, int pol, double a, double da, double b=0, double db=0, double corr=0){

  double EnergyError=0;
  double part_a=0, part_b =0 ;

  if (pol==1) {
    part_a = Energy/a;
    part_b = -1/a;
  }
  if (pol==2) {
    part_a = (0.5*b/(a*a)) + 0.5*(-(b*b/(2*a*a*a)) - (Energy*Energy/a) + (b*Energy/(a*a)) )/(Energy+0.5*(b/a));
    part_b = Energy/(b+2*a*Energy);
  }

  EnergyError = TMath::Sqrt( pow(part_a*da, 2) + pow(part_b*db, 2) + 2*part_a*part_b*corr*da*db );
  return EnergyError;

}
