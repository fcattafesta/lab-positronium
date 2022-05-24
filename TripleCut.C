int Cut(double Energy, double UpLim, double LowLim) {

  if (Energy < LowLim || Energy > UpLim) return -1;
  return 1;
}
void TripleCut() {

  std::string treepath[3] = {"data/2405/triple_1.root",
                             "data/2405/triple_2.root",
                             "data/2405/triple_3.root"},
              figpath = "figures/triple/1705_triple_pol%dfix_no_cut.pdf",
              elementname = "{}^{22}Na";

  double a1 = 42.3, a2 = 206.7, a3 = 86.9 ,
         da1 = 0.2, da2 = 2.5, da3 = 1.2;
  double b1 = 0, b2 = 0, b3 = 0;

  double data1, data2, data3, Energy1, Energy2, Energy3, time_1, time_2, time_3;

  int nbins = 50;
  double inf = 0, sup = 2e3;
  double EnergyLow = inf,
         EnergyUp = sup;


  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);
  t1->SetBranchAddress("EnergyTrap", &Energy1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);
  t2->SetBranchAddress("EnergyTrap", &Energy2);


  auto f3 = new TFile(treepath[2].c_str(), "READ");
  auto t3 = f3->Get<TTree>("tree;3");
  t3->SetBranchAddress("Time", &time_3);
  t3->SetBranchAddress("EnergyTrap", &Energy3);


  int nentries = t1->GetEntries();

  TH1D * h = new TH1D("h", "; Energy [keV]", nbins, inf, sup);

    for (int i=0; i<nentries; i++) {
      t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
      data1 = (Energy1 - b1) / a1;
      data2 = (Energy2 - b2) / a2;
      data3 = (Energy3 - b3) / a3;
      auto time_21 = time_2 - time_1;
      auto time_31 = time_3 - time_1;
      if ((Cut(data1+data2+data3, EnergyUp, EnergyLow) == 1) &&
           abs(time_21) <= 20 && abs(time_31) <= 20) {
             h->Fill(data1+data2+data3);
           }
    }

    h->Draw();

}
