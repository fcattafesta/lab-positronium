#include "scripts/spectrum.h"
#include "scripts/plot.h"

void Background() {

  std::string treepath[2] = {"data/bkg.root", "data/bkg_noise.root"},
              figpath[2] = {"figures/background/bkg.pdf",
                            "figures/background/bkg_noise.pdf"},
              branchname = "EnergyTrap2",
              treename = "tree;3",
              elementname[2] = {"No sources", "Sources nearby"};
  double LowLim[2] = {6e3, 6e3},
         UpLim[2] = {70e3, 70e3},
         fitMin[2] = {62e3, 27e3},
         fitMax[2] = {67e3, 30e3};
  int nbins[3] = {200, 200};
  TFitResultPtr results;
  TH1D *histos[2];

  /*for (int i=0; i<2; i++) {

    TF1 *fitFunc = new TF1("fitFunc", "gaus", fitMin[i], fitMax[i]);

    histos[i] = MakeSpectrum(treepath[i], treename, branchname, nbins[i], LowLim[i], UpLim[i]);
    //histos[i]->Scale(1./histos[i]->Integral());
    auto c = new TCanvas();
    histos[i]->Draw("HIST");

    if (i==0) {

      auto legend = new TLegend(.25, .65, .5, .85);
      legend->SetOption("NDC NB");
      legend->AddEntry(histos[i], "Energy spectrum");
      legend->SetFillStyle(0);
      legend->SetHeader(elementname[i].c_str(), "C");
      legend->Draw();

      auto fitLegend = new TPaveText(.5, .65, .75, .85);
      fitLegend->SetOption("NDC NB");
      fitLegend->SetFillStyle(0);
      fitLegend->SetBorderSize(0.);
      fitLegend->SetTextFont(42);
      fitLegend->SetTextSize(.03);
      fitLegend->SetTextAlign(11);
      auto sEntries = Form("Entries: %.0f", histos[i]->GetEntries());
      fitLegend->AddText(sEntries);
      fitLegend->Draw();
    }

    if (i==1) {
      results = histos[i]->Fit(fitFunc, "SRLNQ");

      fitFunc->Draw("AL SAME");

      auto legend = DrawLegend(c, .25, .65, .65, .85, histos[i],
                               fitMin[i], fitMax[i], fitFunc);
      legend->SetHeader(elementname[i].c_str(), "C");
    }

    DrawDate(c);
    MyStyle(histos[i], fitFunc);
    //c->SaveAs(figpath[i].c_str());

    c->Destructor(); fitFunc->Delete();
  }*/
    DrawSignal(treepath[0], 10);
}
