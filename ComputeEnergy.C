#include "scripts/energy.h"

void ComputeEnergy() {

  std::string materialstree[3] = {"data/2605/Gel/Gel_1.root",
                                  "data/2605/Gel/Gel_2.root",
                                  "data/2605/Gel/Gel_3.root"};

  std::string treepaths[3] = {"data/2605/pmt2/22Na.root",
                              "data/2605/pmt2/60Co.root",
                              "data/2605/pmt2/137Cs.root"};

  std::string coinctree[2] = {"data/2605/coinc_1.root",
                              "data/2605/coinc_2.root"};

/*
  std::string coinctree[3] = {"data/2405/triple_1.root",
                              "data/2405/triple_2.root",
                              "data/2405/triple_3.root"};
*/

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

  for (auto i=0; i<3; i++) {
    cout << materialstree[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    //cout << "Intergal finished" << endl;
    EnergyTrap(materialstree[i], 300, 1029);
    cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i], 0.005);
    //cout << "Thr finished" << endl;
  }


  for (auto i=2; i<2; i++) {
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
