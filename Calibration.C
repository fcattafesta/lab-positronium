#include "scripts/spectrum.h"
#include "scripts/plot.h"

void Calibration() {

  std::string treepath[4] = {"data/60Co.root", "data/60Co.root",
                              "data/137Cs.root", "data/241Am.root"},
              figpath[4] = {"figures/calibration/60Co2704_1.pdf",
                            "figures/calibration/60Co2704_2.pdf",
                            "figures/calibration/137Cs2704.pdf",
                            "figures/calibration/241Am2704.pdf"},
              branchname = "EnergyTrap",
              elementname[4] = {"{}^{60}Co", "{}^{60}Co", "{}^{137}Cs",
                                "{}^{241}Am"};
  double LowLim[4] = {20e3, 20e3, 20e3, 20e3},
         UpLim[4] = {80e3, 80e3, 50e3, 90e3},
         fitMin[4] = {62e3, 69e3, 42e3, 41e3},
         fitMax[4] = {68e3, 73e3, 45e3, 45e3};
  int nbins[4] = {200, 200, 200, 200};
  TFitResultPtr results[4];
  TH1D *histos[4];

  for (int i=0; i<4; i++) {

    TF1 *fitFunc = new TF1("fitFunc", "gaus", fitMin[i], fitMax[i]);

    histos[i] = MakeSpectrum(treepath[i], branchname, nbins[i], LowLim[i], UpLim[i]);
    results[i] = histos[i]->Fit(fitFunc, "SRLNQ");

    auto c = new TCanvas();
    histos[i]->Draw();
    fitFunc->Draw("AL SAME");

    auto legend = DrawLegend(c, .25, .65, .65, .85, histos[i], fitFunc);
    legend->SetHeader(elementname[i].c_str(), "C");
    DrawDate(c);
    MyStyle(histos[i], fitFunc);
    c->SaveAs(figpath[i].c_str());

    c->Destructor(); fitFunc->Delete();
  }

  double Ref[4] = {1173, 1333., 661.6, 59.9,};
  double errRef[4] = {0, 0, 0, 0};
  double fitPeak[4], errPeak[4];

  for (int i=0; i<4; i++) {
    fitPeak[i] = results[i]->GetParams()[1];
    errPeak[i] = results[i]->GetErrors()[1];
  }

  auto g = new TGraphErrors(4, Ref, fitPeak, errRef, errPeak);

  auto calibr = new TF1("calibr", "pol1", 0, 2000);

  auto calFitRes = g->Fit(calibr, "SRNQ EX0");

  auto c = new TCanvas(); c->SetGrid();

  g->Draw("AP SAME");
  calibr->Draw("AL SAME");
  g->SetTitle("");
  g->SetMarkerStyle(8);
  auto xaxis = g->GetXaxis(); xaxis->SetTitle("Reference [keV]");
  auto yaxis = g->GetYaxis(); yaxis->SetTitle("Fitted [u.a.]");
  yaxis->SetTitleOffset(1.4);
  yaxis->SetRangeUser(calFitRes->GetParams()[0] - 5e3,
                      TMath::MaxElement(4, fitPeak) + 5e3);
  c->Update();
  DrawDate(c);
  auto fitLegend = new TPaveText(0.15, 0.55, 0.75, 0.9);
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

  c->SaveAs("figures/calibration/regression.pdf");

}
