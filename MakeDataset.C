#include "scripts/utilities.h"

void MakeDataset() {

  std::string filepaths[2] = {"data/raw/60Co_cal1_2604.txt",
                              "data/raw/137Cs_cal1_2604.txt"};

  std::string treepaths[2] = {"data/60Co.root",
                              "data/137Cs.root"};

  for (int i=0; i<2; i++) {
    MakeTree(filepaths[i], treepaths[i]);
    ComputeEnergyImproved(treepaths[i]);
  }


  /*std::string filepath = "data/raw/241Am_cal1_2604.txt", treepath = "241Am.root";

  MakeTree(filepath, treepath);
  ComputeEnergyImproved(treepath); */
}
