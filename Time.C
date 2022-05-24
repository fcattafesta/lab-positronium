
void Time() {
  std::string treepath[3] = {"data/2405/triple_1.root",
                             "data/2405/triple_2.root",
                             "data/2405/triple_3.root"};

  double time_1, time_2, time_3;
  int nbins = 50;
  double inf = -100, sup = 100;

  auto f1 = new TFile(treepath[0].c_str(), "READ");
  auto t1 = f1->Get<TTree>("tree;3");
  t1->SetBranchAddress("Time", &time_1);

  auto f2 = new TFile(treepath[1].c_str(), "READ");
  auto t2 = f2->Get<TTree>("tree;3");
  t2->SetBranchAddress("Time", &time_2);

  auto f3 = new TFile(treepath[2].c_str(), "READ");
  auto t3 = f3->Get<TTree>("tree;3");
  t3->SetBranchAddress("Time", &time_3);

  int nentries = t1->GetEntries();

  auto h_21 = new TH1D("h_21", ";#DeltaT_{21} [ns]", nbins, inf, sup);
  auto h_31 = new TH1D("h_31", ";#DeltaT_{31} [ns]", nbins, inf, sup);

  auto g = new TGraph();

  for (int i=0; i<nentries; i++) {
    t1->GetEntry(i); t2->GetEntry(i); t3->GetEntry(i);
    auto delta_21 = time_2 - time_1;
    auto delta_31 = time_3 - time_1;
    if (delta_31 >= inf && delta_31 <= sup && delta_21 >= inf && delta_21 <= sup)
    g->AddPoint(delta_21, delta_31);
    if (delta_21 >= inf && delta_21 <= sup) h_21->Fill(delta_21);
    if (delta_31 >= inf && delta_31 <= sup) h_31->Fill(delta_31);
  }

  auto c1 = new TCanvas();
  h_21->Draw();
  c1->SaveAs("figures/triple/time_21.pdf");

  auto c2 = new TCanvas();
  h_31->Draw();
  c2->SaveAs("figures/triple/time_31.pdf");

  auto c3 = new TCanvas(); c3->SetGrid();
  g->Draw("AP");
  g->SetMarkerStyle(8);
  auto xaxis = g->GetXaxis(); xaxis->SetTitle("#Deltat_{21} [ns]");
  xaxis->SetLimits(inf, sup);
  auto yaxis = g->GetYaxis(); yaxis->SetTitle("#Deltat_{31} [ns]");
  yaxis->SetRangeUser(inf, sup);
  c3->SaveAs("figures/triple/21vs31.pdf");
}
