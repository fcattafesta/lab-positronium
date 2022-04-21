#include "utilities.h"

void MakeDataset() {

  std::string filepaths[4] = {"data/raw/22Na_calibration_1204.txt",
                              "data/raw/60Co_calibration_1204.txt",
                              "data/raw/132Cs_calibration_1204.txt",
                              "data/raw/241Am_calibration_1204.txt"};

  std::string treepaths[4] = {"data/22Na.root",
                              "data/60Co.root",
                              "data/132Cs.root",
                              "data/241Am.root",};

  for (int i=0; i<4; i++) {
    MakeTree(filepaths[i], treepaths[i]);
    ComputeEnergyImproved(treepaths[i]);
  }
}
