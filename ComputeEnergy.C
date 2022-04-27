#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[1] = {"data/22Na.root"};

  for (auto i=0; i<1; i++) {
    EnergyTrap(treepaths[i]);
    //cout << "Trap finished" << endl;
    EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }
}
