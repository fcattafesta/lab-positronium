#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[6] = {
                                "data/calibration_1/2405/137Cs.root",
                                "data/calibration_1/2405/22Na.root",
                                "data/calibration_2/2405/137Cs.root",
                                "data/calibration_2/2405/22Na.root",
                                "data/calibration_3/2405/137Cs.root",
                                "data/calibration_3/2405/22Na.root"};
/*
  std::string coinctree[2] = {"data/1005/coinc_1.root",
                              "data/1005/coinc_2.root"};


  std::string coinctree[3] = {"data/2405/triple_1.root",
                              "data/2405/triple_2.root",
                              "data/2405/triple_3.root"};
*/

  for (auto i=4; i<6; i++) {
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

/*
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
*/

}
