int Cut(double data, double inf, double sup) {

  if (data < inf || data > sup) return -1;
  return 1;
}

void TripleCut() {

  std::string treepath[6] = {"data/2405/triple_1.root",
                             "data/2405/triple_2.root",
                             "data/2405/triple_3.root",
                             "data/1705/triple_1.root",
                             "data/1705/triple_2.root",
                             "data/1705/triple_3.root"};

  double a1 = 40, a2 = 223.38, a3 = 88.85 , a4 = 42.3, a5 = 206.75, a6 = 86.92,
         da1 = 0.2, da2 = 8.62, da3 = 3.56;
  double b1 = 40, b2 = 223, b3 = 89;

  double data1, data2, data3, data4, data5, data6,
         Energy1, Energy2, Energy3, Energy4, Energy5, Energy6,
         time_1, time_2, time_3, time_4, time_5, time_6;

  int nbins = 50;
  double inf = 0, sup = 2e3;
  double EnergyLow = 1e2,
         EnergyUp = 4.5e2;


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

  auto f4 = new TFile(treepath[3].c_str(), "READ");
  auto t4 = f4->Get<TTree>("tree;3");
  t4->SetBranchAddress("Time", &time_4);
  t4->SetBranchAddress("EnergyTrap", &Energy4);

  auto f5 = new TFile(treepath[4].c_str(), "READ");
  auto t5 = f5->Get<TTree>("tree;3");
  t5->SetBranchAddress("Time", &time_5);
  t5->SetBranchAddress("EnergyTrap", &Energy5);

  auto f6 = new TFile(treepath[5].c_str(), "READ");
  auto t6 = f6->Get<TTree>("tree;3");
  t6->SetBranchAddress("Time", &time_6);
  t6->SetBranchAddress("EnergyTrap", &Energy6);


  int nentries = t1->GetEntries();

  TH1D * h = new TH1D("h", "Total Energy (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t = new TH1D("h_t", "Total Energy (out-of-coincidence); Energy [keV]",
                        nbins, inf, sup);

  TH1D * h_1 = new TH1D("h_1", "pmt1 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t1 = new TH1D("h_t1", "pmt1 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);

  TH1D * h_2 = new TH1D("h_2", "pmt2 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t2 = new TH1D("h_t2", "pmt2 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);

  TH1D * h_3 = new TH1D("h_3", "pmt3 (coincidence); Energy [keV]", nbins, inf, sup);
  TH1D * h_t3 = new TH1D("h_t3", "pmt3 (out-of-coincidence); Energy [keV]",
                         nbins, inf, sup);


    for (int i=0; i<nentries; i++) {
      t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
      data1 = Energy1 / a1;
      data2 = Energy2 / a2;
      data3 = Energy3 / a3;
      auto time_21 = time_2 - time_1;
      auto time_31 = time_3 - time_1;
      auto time_32 = time_3 - time_2;

      if (Cut(time_21, -30, 20) == 1 && Cut(time_31, -40, 20) == 1 &&
          Cut(time_32, -20, 25) == 1) {

            if (Cut(data1, inf, sup) == 1) h_1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_2->Fill(data2);

            if (Cut(data3, inf, sup) == 1) h_3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1)
                h->Fill(data1 + data2 + data3);
      }
      if (Cut(time_21, -30, 20) == -1 && Cut(time_31, -40, 20) == -1 &&
          Cut(time_32, -20, 25) == -1) {

            if (Cut(data1, inf, sup) == 1) h_t1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_t2->Fill(data2);

            if (Cut(data3, inf, sup) == 1 ) h_t3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1)
                h_t->Fill(data1 + data2 + data3);
      }
    }

    nentries = t4->GetEntries();

    for (int i=0; i<nentries; i++) {
      t4->GetEntry(i); t5->GetEntry(i); t6->GetEntry(i);
      data1 = Energy4 / a4;
      data2 = Energy5 / a5;
      data3 = Energy6 / a6;
      auto time_21 = time_5 - time_4;
      auto time_31 = time_6 - time_4;
      auto time_32 = time_6 - time_5;

      if (Cut(time_21, -30, 20) == 1 && Cut(time_31, -40, 20) == 1 &&
          Cut(time_32, -20, 25) == 1) {

            if (Cut(data1, inf, sup) == 1) h_1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_2->Fill(data2);

            if (Cut(data3, inf, sup) == 1) h_3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1)
                h->Fill(data1 + data2 + data3);
      }
      if (Cut(time_21, -30, 20) == -1 && Cut(time_31, -40, 20) == -1 &&
          Cut(time_32, -20, 25) == -1) {

            if (Cut(data1, inf, sup) == 1) h_t1->Fill(data1);

            if (Cut(data2, inf, sup) == 1) h_t2->Fill(data2);

            if (Cut(data3, inf, sup) == 1 ) h_t3->Fill(data3);

            if (Cut(data1, EnergyLow, EnergyUp) == 1 &&
                Cut(data2, EnergyLow, EnergyUp) == 1 &&
                Cut(data3, EnergyLow, EnergyUp) == 1)
                h_t->Fill(data1 + data2 + data3);
      }
    }

    auto c = new TCanvas();
    h->Draw();
    auto c_t = new TCanvas();
    h_t->Draw();

    auto c1 = new TCanvas();
    h_1->Draw();
    auto c_t1 = new TCanvas();
    h_t1->Draw();

    auto c2 = new TCanvas();
    h_2->Draw();
    auto c_t2 = new TCanvas();
    h_t2->Draw();

    auto c3 = new TCanvas();
    h_3->Draw();
    auto c_t3 = new TCanvas();
    h_t3->Draw();

}
