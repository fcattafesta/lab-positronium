
int Cut(double Energy, double UpLim, double LowLim) {

  if (Energy < LowLim || Energy > UpLim) return -1;
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


TH1D * CalSpectrum(std::string treepath, std::string treename, std::string EnergyBranch, int pol,
                        int nbins, double LowLim, double UpLim, double a, double b){

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  int nentries = t->GetEntries();
  double data, Energy;

  t->SetBranchAddress(EnergyBranch.c_str(), &Energy);


  TH1D * h = new TH1D("h", "; Energy [a.u.]", nbins, LowLim, UpLim);

  if (pol==1) {
    for (int i=0; i<nentries; i++) {
      t->GetEntry(i);
      data = (Energy - b) / a;
      if (Cut(data, UpLim, LowLim) == 1) h->Fill(data);
    }
  }
  if (pol==2) {
    for (int i=0; i<nentries; i++) {
      t->GetEntry(i);
      data = (0.5/a)*(-b + TMath::Sqrt(b*b+4*a*Energy));
      if (Cut(data, UpLim, LowLim) == 1) h->Fill(data);
    }
  }

  return h;

}


TH1D * CalSpectrum_sum(std::string treepath1, std::string treepath2,
                       std::string treepath3, int pol, int nbins, double LowLim,
                       double UpLim, double EnergyLow, double EnergyUp,
                       double a1, double b1, double a2, double b2,
                       double a3, double b3) {

  std::string treename = "tree;2", EnergyBranch = "EnergyTrap";

  auto f1 = new TFile(treepath1.c_str(), "READ");
  auto t1 = f1->Get<TTree>(treename.c_str()); // Change tree cycle for different energy method calculation

  auto f2 = new TFile(treepath2.c_str(), "READ");
  auto t2 = f2->Get<TTree>(treename.c_str());

  auto f3 = new TFile(treepath3.c_str(), "READ");
  auto t3 = f3->Get<TTree>(treename.c_str());

  int nentries = t1->GetEntries();
  double data1, data2, data3, Energy1, Energy2, Energy3;

  t1->SetBranchAddress(EnergyBranch.c_str(), &Energy1);
  t2->SetBranchAddress(EnergyBranch.c_str(), &Energy2);
  t3->SetBranchAddress(EnergyBranch.c_str(), &Energy3);

  TH1D * h = new TH1D("h", "; Energy [keV]", nbins, LowLim, UpLim);

  if (pol==1) {
    for (int i=0; i<nentries; i++) {
      t1->GetEntry(i);
      t2->GetEntry(i);
      t3->GetEntry(i);
      data1 = (Energy1 - b1) / a1;
      data2 = (Energy2 - b2) / a2;
      data3 = (Energy3 - b3) / a3;
      if ((Cut(data1, EnergyUp, EnergyLow) == 1) &&
          (Cut(data2, EnergyUp, EnergyLow) == 1) &&
          (Cut(data3, EnergyUp, EnergyLow) == 1) &&
          (Cut(data1+data2+data3, UpLim, LowLim) == 1)) {
             h->Fill(data1+data2+data3);
           }
    }
  }
  if (pol==2) {
    for (int i=0; i<nentries; i++) {
      t1->GetEntry(i);
      t2->GetEntry(i);
      t3->GetEntry(i);
      data1 = (0.5/a1)*(-b1 + TMath::Sqrt(b1*b1+4*a1*Energy1));
      data2 = (0.5/a2)*(-b2 + TMath::Sqrt(b2*b2+4*a2*Energy2));
      if (Cut(data1+data2, UpLim, LowLim) == 1) h->Fill(data1+data2);
    }
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
    part_a = (0.5*b/(a*a)) - 0.5*(+(b*b/(2*a*a*a)) + (Energy*Energy/a) + (b*Energy/(a*a)) )/(Energy+0.5*(b/a));
    part_b = Energy/(b+2*a*Energy);
  }

  EnergyError = TMath::Sqrt( pow(part_a*da, 2) + pow(part_b*db, 2) + 2*part_a*part_b*corr*da*db );
  return EnergyError;

}
