#include "scripts/energy.h"

void ComputeEnergy(int idx) {

<<<<<<< HEAD
  std::string treepaths[5] = {"data/22Na.root",
                              "data/60Co.root",
                              "data/137Cs.root",
                              "data/241Am.root",
                              "data/90Sr.root"};

  std::string bkgtree[2] = {"data/bkg.root", "data/bkg_noise.root"};

  int low = 200, up = 800;

  for (auto i=4; i<5; i++) {
    cout << treepaths[i] << endl;
    //EnergyIntegral(treepaths[i], low, up);
    cout << "Intergal finished" << endl;
    //EnergyTrap(treepaths[i], low, up);
    cout << "Trap finished" << endl;
    EnergyMinAmp(treepaths[i]);
    cout << "MinAmp finished" << endl;
=======
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
>>>>>>> c35cd7d50b6a47b5e9b49b14fddaa7f3437697ee
    //EnergyThr(treepaths[i]);
    //cout << "Thr finished" << endl;
  }
  */

  EnergyTrap2(treepaths[idx]);
  EnergyMinAmp(treepaths[idx]);
  //EnergyThr(treepaths[idx]);

<<<<<<< HEAD
  /*
  for (auto i=0; i<2; i++) {
    cout << bkgtree[i] << endl;
    EnergyTrap(bkgtree[i], low, up);
    cout << "Trap finished" << endl;
    EnergyMinAmp(bkgtree[i]);
    cout << "MinAmp finished" << endl;
  }
  */

=======
>>>>>>> c35cd7d50b6a47b5e9b49b14fddaa7f3437697ee
}
