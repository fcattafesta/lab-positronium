#include "scripts/energy.h"

void ComputeEnergy(int idx) {

  std::string treepaths[5] = {"data/22Na.root", "data/60Co.root",
                              "data/137Cs.root", "data/241Am.root",
                              "data/90Sr.root"};

  /*
  for (auto i=0; i<5; i++) {
    cout << treepaths[i] << endl;
    EnergyTrap2(treepaths[i]);
    //cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }
  */

  EnergyTrap2(treepaths[idx]);
  EnergyMinAmp(treepaths[idx]);
  //EnergyThr(treepaths[idx]);

}
