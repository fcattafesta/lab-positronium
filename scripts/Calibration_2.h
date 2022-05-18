#include "fitpeaks.h"

const int npeak = 4;

TFitResultPtr Calibration_2(std::string treepath[npeak], std::string figpath[npeak],
                          std::string elementname[npeak], std::string figregrpath,
                          TF1 * calibr) {

  double histMin[npeak] = {203e3, 247e3, 100e3, 78e3},
         histMax[npeak] = {247.3e3, 290e3, 160e3, 130e3},
         peakMin[npeak] = {222e3, 250e3, 124e3, 103e3},
         peakMax[npeak] = {242e3, 270e3, 146e3, 122e3},
         initBkg[npeak][3] = {{40, -.2, 1e-5}, {10., -1e-5, 0.},
                              {10, .03, 1.e-7}, {100, .03, 1.e-7}},
         initPeak[npeak][3] = {{.2e3, 49e3, 1.1e3}, {.2e3, 56e3, 1.1e3},
                               {1e3, 28.5e3, 1e3}, {.4e3, 54e3, .8e3}};


  TFitResultPtr results[npeak];

  for (int i=0; i<npeak; i++) {
    results[i] = FitPeak(treepath[i], figpath[i], elementname[i], histMin[i], histMax[i],
                         peakMin[i], peakMax[i], initBkg[i], initPeak[i]);
  }

  double ref[npeak] = {1173.23, 1332.50, 661.6, 1274.53},
         errRef[npeak] = {0, 0, 0, 0};
  double fitPeak[npeak], newErr[npeak], errPeak[npeak];

  for (int i=0; i<npeak; i++) {
    fitPeak[i] = results[i]->GetParams()[1];
    errPeak[i] = results[i]->GetErrors()[1];
   }

  auto g = new TGraphErrors(npeak, ref, fitPeak, errRef, errPeak);

  auto res1 = g->Fit(calibr, "SRN EX0");

  auto chinorm = res1->Chi2() / res1->Ndf();

  for (int i=0; i<npeak; i++) {
    newErr[i] = errPeak[i] * sqrt(chinorm);
    g->SetPointError(i, errRef[i], newErr[i]);
  }

  auto calFitRes = g->Fit(calibr, "SRN EX0");

  auto c = new TCanvas(); c->SetGrid();

  TPad *pad1 = new TPad("", "", 0, 0.33, 1, 1);
  TPad *pad2 = new TPad("", "", 0, 0, 1, 0.33);
  pad1->SetBottomMargin(0.00001);
  pad1->SetBorderMode(0);
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.2);
  pad2->SetBorderMode(0);
  pad1->Draw();
  pad2->Draw();

  pad1->cd(); pad1->SetGrid();

  g->Draw("AP SAME");
  calibr->Draw("AL SAME");
  calibr->SetLineColor(46);
  g->SetTitle("");
  g->SetMarkerStyle(8); g->SetMarkerSize(1.);
  auto xaxis = g->GetXaxis(); xaxis->SetLimits(0., 1500.);
  xaxis->SetTitle("Reference [keV]");
  auto yaxis = g->GetYaxis(); yaxis->SetTitle("Fitted [u.a.]");
  yaxis->SetTitleOffset(1.2); yaxis->SetTitleSize(.045);
  yaxis->SetLabelSize(.04);
  yaxis->SetRangeUser(calFitRes->GetParams()[0] - 10e3,
                      TMath::MaxElement(npeak, fitPeak) + 5e3);
  c->Update();
  DrawDate(c);
  auto fitLegend = new TPaveText(.15, .55, .75, .85);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.05);
  fitLegend->SetTextAlign(11);

  auto s0 = Form("[0]: %.4f #pm %.4f [u.a.]", calFitRes->GetParams()[0],
                                                     calFitRes->GetErrors()[0]);
  auto s1 = Form("[1]: %.4f #pm %.4f [u.a./keV]", calFitRes->GetParams()[1],
                                             calFitRes->GetErrors()[1]);
  auto s2 = Form("[2]: %.4f #pm %.4f [u.a./keV]", calFitRes->GetParams()[2],
                                                      calFitRes->GetErrors()[2]);
  auto sChi = Form("#chi^{2}/ndof: %.2f/%.0u", calFitRes->Chi2(), calFitRes->Ndf());
  auto sProb = Form("Prob. : %E", calFitRes->Prob());
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(s0);
  fitLegend->AddText(s1);
  fitLegend->AddText(s2);
  fitLegend->AddText(sChi);
  fitLegend->AddText(sProb);
  fitLegend->Draw();

  pad2->cd(); pad2->SetGrid();

  auto res = new TGraph();

  for (int i=0; i<npeak; i++) {
    auto diff = (fitPeak[i] - calibr->Eval(ref[i])) / newErr[i];
    res->AddPoint(ref[i], diff);
  }

  res->Draw("AP");
  res->SetMarkerStyle(8);

  auto zero = new TF1("zero", "0", 0, 1.5e3);
  zero->Draw("AL SAME");
  zero->SetLineStyle(9);
  zero->SetLineColor(kBlack);
  zero->SetLineWidth(1);

  auto resXaxis = res->GetXaxis(); resXaxis->SetLimits(0., 1500.);
  resXaxis->SetTitle("Reference [keV]");
  resXaxis->SetTitleSize(.1); resXaxis->SetTitleOffset(.9);
  resXaxis->SetLabelSize(.08);
  auto resYaxis = res->GetYaxis();
  resYaxis->SetTitle("Norm. res.");
  resYaxis->SetTitleSize(.1); resYaxis->SetTitleOffset(.5);
  resYaxis->SetLabelSize(.08);


  c->SaveAs(figregrpath.c_str());
  c->Destructor();

  return calFitRes;

}
