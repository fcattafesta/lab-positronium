#include "scripts/spectrum.h"
#include "scripts/plot.h"

void CalibrationTrap() {

  std::string treepath[3] = {"data/60Co.root", "data/60Co.root",
                              "data/137Cs.root"},
              figpath[4] = {"figures/calibrationTrap/60Co2704_1.pdf",
                            "figures/calibrationTrap/60Co2704_2.pdf",
                            "figures/calibrationTrap/137Cs2704.pdf",
                            "figures/calibrationTrap/regression.pdf"},
              branchname = "EnergyTrap",
              treename[3] = {"tree;2", "tree;2", "tree;2"},
              elementname[3] = {"{}^{60}Co", "{}^{60}Co", "{}^{137}Cs"};
  double LowLim[3] = {8e3, 8e3, 4e3},
         UpLim[3] = {64e3, 64e3, 33e3},
         fitMin[3] = {48e3, 54e3, 26.5e3},
         fitMax[3] = {53e3, 60e3, 31e3};
  int nbins[3] = {200, 200, 200};
  TFitResultPtr results[3];
  TH1D *histos[3];

  for (int i=0; i<3; i++) {

    TF1 *fitFunc = new TF1("fitFunc", "gaus", fitMin[i], fitMax[i]);

    histos[i] = MakeSpectrum(treepath[i], treename[i], branchname, nbins[i], LowLim[i], UpLim[i]);
    results[i] = histos[i]->Fit(fitFunc, "SRLNQ");

    auto c = new TCanvas();
    histos[i]->Draw();
    fitFunc->Draw("AL SAME");

    auto legend = DrawLegend(c, .25, .65, .65, .85, histos[i],
                             fitMin[i], fitMax[i], fitFunc);
    legend->SetHeader(elementname[i].c_str(), "C");
    DrawDate(c);
    MyStyle(histos[i], fitFunc);
    c->SaveAs(figpath[i].c_str());

    c->Destructor(); fitFunc->Delete();
  }

  double Ref[3] = {1173, 1333., 661.6};
  double errRef[4] = {0, 0, 0};
  double fitPeak[3], errPeak[3];

  for (int i=0; i<3; i++) {
    fitPeak[i] = results[i]->GetParams()[1];
    errPeak[i] = results[i]->GetErrors()[1];
  }

  auto g = new TGraphErrors(3, Ref, fitPeak, errRef, errPeak);

  auto calibr = new TF1("calibr", "pol1", 0, 2000);

  auto calFitRes = g->Fit(calibr, "SRNQ EX0");

  auto c = new TCanvas(); c->SetGrid();

  g->Draw("AP SAME");
  calibr->Draw("AL SAME");
  g->SetTitle("");
  g->SetMarkerStyle(8);
  auto xaxis = g->GetXaxis(); xaxis->SetTitle("Reference [keV]");
  xaxis->SetRangeUser(0., 1500.);
  auto yaxis = g->GetYaxis(); yaxis->SetTitle("Fitted [u.a.]");
  yaxis->SetTitleOffset(1.4);
  yaxis->SetRangeUser(calFitRes->GetParams()[0] - 1e3,
                      TMath::MaxElement(3, fitPeak) + 1e3);
  c->Update();
  DrawDate(c);
  auto fitLegend = new TPaveText(.15, .65, .75, .9);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.04);
  fitLegend->SetTextAlign(11);

  auto sIntercept = Form("Intercept: %.0f #pm %.0f [u.a.]", calFitRes->GetParams()[0],
                                                     calFitRes->GetErrors()[0]);
  auto sSlope = Form("Slope: %.2f #pm %.2f [u.a./keV]", calFitRes->GetParams()[1],
                                             calFitRes->GetErrors()[1]);
  auto sChi = Form("#chi^{2}/ndof: %.2f/%.0u", calFitRes->Chi2(), calFitRes->Ndf());
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(sIntercept);
  fitLegend->AddText(sSlope);
  fitLegend->AddText(sChi);
  fitLegend->Draw();

  c->SaveAs(figpath[3].c_str());

}
