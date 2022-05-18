
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/calibration_2/1205/22Na.root", treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 200;
  double LowLim=50e3, UpLim=1000e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
