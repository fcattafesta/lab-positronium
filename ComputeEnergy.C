#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[4] = {"data/22Na.root", "data/60Co.root",
                              "data/137Cs.root", "data/241Am.root"};

  /*for (auto i=0; i<4; i++) {
    cout << treepaths[i] << endl;
    EnergyTrap(treepaths[i]);
    //cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }*/

  EnergyTrap(treepaths[3]);
}
