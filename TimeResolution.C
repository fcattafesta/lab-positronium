
void TimeResolution() {

  std::string treepath[2] = {"data/1205/coinc_1_t.root",
                          "data/1205/coinc_2_t.root"};


  double time_1, time_2;
  int nbins = 30;
  double inf = -30, sup = -1 * inf;

  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;4");
  t1->SetBranchAddress("Time", &time_1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;4");
  t2->SetBranchAddress("Time", &time_2);

  auto nentries = t1->GetEntries();

  auto h = new TH1D("h", ";#Deltat_{12} [ns]", nbins, inf, sup);

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i);
    h->Fill(time_2 - time_1);
  }

  h->Draw();
}
