#include <string>

int RECORD_LENGTH = 1030;

void MakeTree(std::string filepath, std::string treepath) {

  int RECORD_LENGTH = 1030;

  int RecordLength, BoardID, Channel, Event, TriggerTT;
  int Amplitudes[RECORD_LENGTH];

  ifstream infile (filepath);

  auto f = new TFile(treepath.c_str(), "RECREATE");

  auto tree = new TTree("tree", "ADC Acquisitions");

  //tree->Branch("RecordLength", &RecordLength);
  //tree->Branch("BoardID", &BoardID);
  tree->Branch("Channel", &Channel);
  tree->Branch("Event", &Event);
  tree->Branch("TriggerTT", &TriggerTT);
  tree->Branch("Amplitudes", &Amplitudes, "array[1030]/I");  // RECORD_LENGTH

  while (infile.peek() != EOF) {

    char tmp;
    int Pattern, Offset;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> RecordLength;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> BoardID;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Channel;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Event;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Pattern;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> TriggerTT;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Offset;

    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();

    for (int i=0; i < RECORD_LENGTH; i++) {
      infile >> Amplitudes[i];
    }
    tree->Fill();
  }

  finalcode:
  tree->Write();
  f->Close();
  delete f;
  return ;
}

void ComputeEnergy(std::string treepath) {

  auto f = new TFile(treepath.c_str(), "UPDATE");
  auto t = f->Get<TTree>("tree"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Energy;

  auto bEnergy = t->Branch("Energy", &Energy);
  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    Energy = g->Integral(-1, -1);

    bEnergy->Fill();
  }
  t->Write();
  f->Close();
}

void DrawSignal(std::string treepath, int event) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];

  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

  if (i==event) {
    g->Draw("AL");
    g->SetMarkerStyle(20);
    auto xaxis = g->GetXaxis();
    xaxis->SetTitle("Time [a.u.]");
    auto yaxis = g->GetYaxis();
    yaxis->SetTitle("Amplitude [a.u.]");
    yaxis->SetTitleOffset(1.4);
    }
  }
}

void HistStyle(TH1D * h) {
  h->SetLineColor(kBlack); h->SetLineWidth(1);
  h->SetFillColorAlpha(46, 0.5);

  gStyle->SetOptStat(10); gStyle->SetStatBorderSize(0);
  gStyle->SetStatX(1.); gStyle->SetStatY(1.);
}
