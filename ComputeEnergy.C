#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[3] = {"data/1005/pmt1/22Na.root",
                              "data/1005/pmt1/60Co_check.root",
                              "data/1005/pmt1/137Cs_check.root"};
/*
  std::string coinctree[2] = {"data/1005/coinc_1.root",
                              "data/1005/coinc_2.root"};
*/

  std::string coinctree[3] = {"data/1805/triple_1.root",
                              "data/1805/triple_2.root",
                              "data/1805/triple_3.root"};

  for (auto i=1; i<3; i++) {
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

  for (auto i=3; i<3; i++) {
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
