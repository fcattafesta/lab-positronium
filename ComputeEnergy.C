#include "scripts/energy.h"

void ComputeEnergy() {

  std::string treepaths[5] = {"data/22Na.root",
                              "data/60Co.root",
                              "data/137Cs.root",
                              "data/241Am.root",
                              "data/90Sr.root"};

  std::string bkgtree[2] = {"data/bkg.root", "data/bkg_noise.root"};

  std::string treeam = "data/241Am/Noise_50thr.root";

  std::string treedist[3] = {"data/dist/137Cs_8cm.root",
                             "data/dist/137Cs_16cm.root",
                             "data/dist/137Cs_24cm.root"};

  int low = 200, up = 800;

  for (int i=0; i<3; i++) {
    EnergyTrap(treedist[i], low, up);
  }

  /*
  for (auto i=4; i<5; i++) {
    cout << treepaths[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    cout << "Intergal finished" << endl;
    //EnergyTrap(treepaths[i], low, up);
    cout << "Trap finished" << endl;
    EnergyMinAmp(treepaths[i]);
    cout << "MinAmp finished" << endl;
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }
  */

  /*
  for (auto i=0; i<2; i++) {
    cout << bkgtree[i] << endl;
    EnergyIntegral(bkgtree[i], low, up);
    cout << "Integral finished" << endl;
    EnergyTrap(bkgtree[i], low, up);
    cout << "Trap finished" << endl;
    EnergyMinAmp(bkgtree[i]);
    cout << "MinAmp finished" << endl;
  }*/

  //EnergyMinAmp(treeam);


}
