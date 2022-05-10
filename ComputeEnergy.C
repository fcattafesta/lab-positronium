#include "scripts/energy.h"

void ComputeEnergy() {



  int low[2] = {300, 350},
      up[2] = {800, 750};



  std::string treepaths[3] = {"data/range/22Na_2804.root",
                              "data/range/60Co.root",
                              "data/range/137Cs.root",
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
  for (int i=0; i<1; i++) {
    for (int j=; j<4; j++) EnergyTrap(treepaths[i], low[j], up[j]);
  }

*/
  //int low = 200, up= 1029;

  int j=1;

  for (auto i=0; i<1; i++) {
    cout << treepaths[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    //cout << "Intergal finished" << endl;
    EnergyTrap(treepaths[i], low[j], up[j]);
    cout << "Trap finished" << endl;
    //EnergyMinAmp(treepaths[i]);
    //cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i], 0.005);
    //cout << "Thr finished" << endl;
  }


  /*
  for (auto i=2; i<3; i++) {
    cout << bkgtree[i] << endl;
    //EnergyIntegral(bkgtree[i], low, up);
    cout << "Integral finished" << endl;
    EnergyTrap(bkgtree[i], low[1], up[1]);
    cout << "Trap finished" << endl;
    EnergyMinAmp(bkgtree[i]);
    cout << "MinAmp finished" << endl;
  }
  */


  //EnergyMinAmp(treeam);


}
