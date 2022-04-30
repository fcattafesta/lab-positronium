#include "scripts/spectrum.h"
#include "scripts/plot.h"

void Background() {

  std::string treepath[2] = {"data/bkg.root", "data/bkg_noise.root"},
              figpath[2] = {"figures/background/bkg.pdf",
                            "figures/background/bkg_noise.pdf"},
              branchname = "EnergyTrap",
              treename = "tree;2",
              elementname[2] = {"No sources", "Sources nearby"};
  double LowLim[2] = {20e3, 20e3},
         UpLim[2] = {80e3, 80e3},
         fitMin[2] = {62e3, 41e3},
         fitMax[2] = {67e3, 45e3};
  int nbins[3] = {200, 200};
  TFitResultPtr results;
  TH1D *histos[2];

  for (int i=0; i<2; i++) {

    TF1 *fitFunc = new TF1("fitFunc", "gaus", fitMin[i], fitMax[i]);

    histos[i] = MakeSpectrum(treepath[i], treename, branchname, nbins[i], LowLim[i], UpLim[i]);
    histos[i]->Scale(1./histos[i]->Integral());
    auto c = new TCanvas();
    histos[i]->Draw("HIST");

    if (i==1) {
      results = histos[i]->Fit(fitFunc, "SRLNQ");

      fitFunc->Draw("AL SAME");

      auto legend = DrawLegend(c, .25, .65, .65, .85, histos[i],
                               fitMin[i], fitMax[i], fitFunc);
      legend->SetHeader(elementname[i].c_str(), "C");
    }

    DrawDate(c);
    MyStyle(histos[i], fitFunc);
    c->SaveAs(figpath[i].c_str());

    c->Destructor(); fitFunc->Delete();
    }
}
