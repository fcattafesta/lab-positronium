#include <string>

int RECORD_LENGTH = 10;

void MakeTree(std::string filepath, std::string treepath) {

  int RecordLength, BoardID, Channel, Event;
  double TriggerTT;
  int Amplitudes[RECORD_LENGTH];

  /*ofstream fileout (filepath, ios::app);

  fileout << '$' << endl;

  fileout.close(); */

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
    cout << Event << endl;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Pattern;
    cout << "Pattern" << endl;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> TriggerTT;
    cout << "TriggerTT" << endl;

    while ((tmp = infile.get()) != ':') if(tmp == EOF) goto finalcode;
    infile >> Offset;

    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();
    tmp = infile.get();
    cout << "Offset" << endl;

    for (int i=0; i < RECORD_LENGTH; i++) {
      infile >> Amplitudes[i];
      cout << Amplitudes[i] << endl;
    }
    tree->Fill();
  }

  finalcode:
  cout << "end" << endl;
  tree->Write();
  f->Close();
  delete f;
  return ;
}

void prova() {

  std::string filepath = "data/raw/241Am_cal1_2604.txt";
  std::string treepath = "prova.root";
  std::string provapath = "prova.txt";
  MakeTree(provapath, treepath);
}
