
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/1205/coinc_2.root", treename = "tree;2", EnergyBranch = "EnergyTrap";
  int nbins = 200;
  double LowLim=100e3, UpLim=120e3;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
