#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[5] = {"data/22Na.root", "data/60Co.root",
                              "data/137Cs.root", "data/241Am.root",
                              "data/90Sr.root"};

  std::string bkgtree[2] = {"data/bkg.root", "data/bkg_noise.root"};

  for (auto i=0; i<4; i++) {
    cout << treepaths[i] << endl;
    //EnergyTrap(treepaths[i]);
    //cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }

  for (auto i=0; i<2; i++) {
    EnergyTrap(bkgtree[i]);
  }

  //EnergyTrap(treepaths[0]);
}
