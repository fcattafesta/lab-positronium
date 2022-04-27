#include <string>

int RECORD_LENGTH = 1030;

void MakeTree(std::string filepath, std::string treepath) {

  int RecordLength, BoardID, Channel, Event;
  double TriggerTT;
  int Amplitudes[RECORD_LENGTH];

  ifstream infile (filepath);

  auto f = new TFile(treepath.c_str(), "RECREATE");

  auto tree = new TTree("tree", "ADC Acquisitions");
  tree->SetAutoSave(0);

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
