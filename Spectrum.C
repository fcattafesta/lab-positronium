
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/22Na.root", treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 200;
  double LowLim=45e3, UpLim=62e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
