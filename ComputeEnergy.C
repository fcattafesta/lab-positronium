#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[3] = {"data/1205/22Na.root",
                              "data/1205/60Co.root",
                              "data/1205/137Cs.root"};

  std::string coinctree[2] = {"data/1205/coinc_1.root",
                              "data/1205/coinc_2.root"};

  for (auto i=3; i<3; i++) {
    cout << treepaths[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    //cout << "Intergal finished" << endl;
    EnergyTrap(treepaths[i], 300, 1029);
    cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i], 0.005);
    //cout << "Thr finished" << endl;
  }

  for (auto i=0; i<2; i++) {
    cout << coinctree[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    //cout << "Intergal finished" << endl;
    EnergyTrap(coinctree[i], 300, 1029);
    cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i], 0.005);
    //cout << "Thr finished" << endl;
  }

}
