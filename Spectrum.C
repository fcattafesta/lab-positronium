
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/calibration_3/2505/60Co.root",
              treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 50;
  double LowLim = 75e3, UpLim = 120e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
