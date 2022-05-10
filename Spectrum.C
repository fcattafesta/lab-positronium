
#include "scripts/spectrum.h"

void Spectrum() {
  std::string treepath = "data/22Na_2804.root", treename = "tree;4", EnergyBranch = "EnergyThr";
  int nbins = 1000;
  double LowLim=0, UpLim=300;

  auto h = MakeSpectrum(treepath, treename, EnergyBranch, nbins, LowLim, UpLim);

  h->Draw();
}
