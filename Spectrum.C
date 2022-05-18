
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/1805/triple_3.root",
              treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 20;
  double LowLim = 0e3, UpLim = 80e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
