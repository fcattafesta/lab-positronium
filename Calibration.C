#include "scripts/CalibrationFunctions.h"

void Calibration() {

  //std::string figpath = "figures/fit/regression.pdf";
  std::string treepath[3] = {"data/60Co.root", "data/60Co.root", "data/137Cs.root"},
              branchname = "EnergyTrap", treename = "tree;2";

  double fitMin[3] = {40e3, 53e3, 22e3}, fitMax[3] = {53e3, 67e3, 32e3};


  TFitResultPtr results[3] = {Cobalt1(), Cobalt2(), Caesium()};

  double Ref[3] = {1173, 1333., 661.6};
  double errRef[3] = {0, 0, 0};
  double errStat, errSyst;
  double fitPeak[3], errPeak[3];

  for (int i=0; i<3; i++) {
    fitPeak[i] = results[i]->GetParams()[1];
    errStat = results[i]->GetErrors()[1];
    errSyst = StatEnergyError(treepath[i], treename, branchname, "EnergyError", fitMin[i], fitMax[i]);
    errPeak[i] = errStat + errSyst;
    cout << errSyst <<endl;
  }

  auto g = new TGraphErrors(3, Ref, fitPeak, errRef, errPeak);

  auto calibr = new TF1("calibr", "pol1", -1., 2000);
  //calibr->FixParameter(0, 0.);



  auto calFitRes = g->Fit(calibr, "SRN EX0");

  auto c = new TCanvas(); c->SetGrid();

  TPad *pad1 = new TPad("","",0,0.33,1,1);
  TPad *pad2 = new TPad("","",0,0,1,0.33);
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
                      TMath::MaxElement(3, fitPeak) + 5e3);
  c->Update();
  DrawDate(c);
  auto fitLegend = new TPaveText(.15, .65, .75, .85);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.05);
  fitLegend->SetTextAlign(11);

  auto sIntercept = Form("Intercept: %.4f #pm %.4f [u.a.]", calFitRes->GetParams()[0],
                                                     calFitRes->GetErrors()[0]);
  auto sSlope = Form("Slope: %.4f #pm %.4f [u.a./keV]", calFitRes->GetParams()[1],
                                             calFitRes->GetErrors()[1]);
  auto sCorr = Form("Correlation: %.4f", calFitRes->Correlation(0,1));
  auto sChi = Form("#chi^{2}/ndof: %.2f/%.0u", calFitRes->Chi2(), calFitRes->Ndf());
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(sIntercept);
  fitLegend->AddText(sSlope);
  fitLegend->AddText(sCorr);
  fitLegend->AddText(sChi);
  fitLegend->Draw();

  pad2->cd(); pad2->SetGrid();

  auto res = new TGraph();

  for (int i=0; i<3; i++) {

    auto diff = (fitPeak[i] - calibr->Eval(Ref[i])) / errPeak[i];
    res->AddPoint(Ref[i], diff);

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


  //c->SaveAs(figpath.c_str());

}
