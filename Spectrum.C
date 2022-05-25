
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/calibration_3/2405/22Na.root",
              treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 60;
  double LowLim = 106e3, UpLim = 150e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
