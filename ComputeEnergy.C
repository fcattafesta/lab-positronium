#include "scripts/energy.h"

void ComputeEnergy() {


  int low[3] = {0, 200, 300},
      up[3] = {1029, 800, 700} ;

  std::string treepaths[3] = {"data/22Na.root",
                              "data/60Co.root",
                              "data/137Cs.root",
                              //"data/241Am.root",
                              //"data/90Sr.root"
                            };

  std::string treerange = {"data/range/60Co.root"};

  std::string natree[2] = {"data/22Na/19mV.root",
                           "data/22Na/30mV.root"};


  std::string bkgtree[3] = {"data/background/bkg.root",
                            "data/background/bkg_noise.root",
                            "data/background/bkg_0thr.root"};

  //std::string treeam = "data/241Am/Noise_50thr.root";

/*
  std::string treedist[3] = {"data/dist/137Cs_8cm.root",
                             "data/dist/137Cs_16cm.root",
                             "data/dist/137Cs_24cm.root"};


  for (int i=0; i<3; i++) {
    EnergyTrap(treedist[i], low, up);
  }

*/
/*
  for (int i=0; i<2; i++) {
    EnergyTrap(natree[i], low[1], up[1]);
  }*/

  /*
  for (auto i=0; i<3; i++) {
    cout << treepaths[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    //cout << "Intergal finished" << endl;
    EnergyTrap(treepaths[i], low, up);
    cout << "Trap finished" << endl;
    EnergyMinAmp(treepaths[i]);
    cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }
  */


  for (auto i=2; i<3; i++) {
    cout << bkgtree[i] << endl;
    //EnergyIntegral(bkgtree[i], low, up);
    cout << "Integral finished" << endl;
    EnergyTrap(bkgtree[i], low[1], up[1]);
    cout << "Trap finished" << endl;
    EnergyMinAmp(bkgtree[i]);
    cout << "MinAmp finished" << endl;
  }

  //EnergyMinAmp(treeam);


}
