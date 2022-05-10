#include "dataset.h"

TGraph * DrawSignal(std::string treepath, int event) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;1"); //tree does not exist after an update (e.g. tree;1)

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];

  t->SetBranchAddress("Amplitudes", &v);

  for (int i=0; i<nentries; i++) {
    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

  if (i==event) {
    g->Draw("AL");
    g->SetMarkerStyle(20);
    auto xaxis = g->GetXaxis();
    xaxis->SetTitle("Time [a.u.]");
    auto yaxis = g->GetYaxis();
    yaxis->SetTitle("Amplitude [a.u.]");
    yaxis->SetTitleOffset(1.4);
    return g;
    }
  }
  return NULL;
}

void MyStyle(TH1D * h, TF1 * f) {
  h->SetLineColor(kBlack); h->SetLineWidth(1);
  h->SetFillColorAlpha(46, 0.5);

  f->SetLineColor(kBlue-4); f->SetLineWidth(3);

  gStyle->SetOptStat(0); gStyle->SetStatBorderSize(0.);
  gStyle->SetStatX(1.); gStyle->SetStatY(1.);
}

void DrawDate(TCanvas * c) {

  auto pt = new TPaveText(.0, .95, .2, 1);
  pt->SetOption("NDC NB");
  pt->SetFillColor(0);
  pt->SetTextFont(42);
  pt->SetTextSize(.03);

  auto today = new TDatime();
  auto sDate = today->AsSQLString();
  pt->AddText(sDate);
  pt->Draw();

  c->Update();
}

TLegend * DrawLegend(TCanvas *c, double x1, double y1, double x2, double y2,
                     TH1D *h, double fitMin, double fitMax, TF1 *peak) {

  gStyle->SetLegendBorderSize(0.);
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(.03);

  auto legend = new TLegend(x1, y1, x1+0.25, y1+0.25);
  legend->SetOption("NDC NB");
  legend->AddEntry(h, "Energy spectrum");
  legend->AddEntry(peak, "Gaussian fit");
  legend->SetFillStyle(0);
  legend->Draw();

  auto fitLegend = new TPaveText(x1+0.25, y1, x2, y2);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.03);
  fitLegend->SetTextAlign(11);

  auto sEntries = Form("Entries: %.0f", h->GetEntries());
  auto sFitEntries = Form(" Fit Entries: %.0f", h->Integral(h->FindBin(fitMin),
                                                           h->FindBin(fitMax)));
  auto sMean = Form(" Mean: %.2f #pm %.2f", peak->GetParameter(1), peak->GetParError(1));
  auto sSigma = Form(" Sigma: %.2f #pm %.2f", peak->GetParameter(2), peak->GetParError(2));
  auto sResolution = Form(" Resolution: %.2f%%", (peak->GetParameter(2)/peak->GetParameter(1))*100);
  fitLegend->AddText(sEntries);
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(sFitEntries);
  fitLegend->AddText(sMean);
  fitLegend->AddText(sSigma);
  fitLegend->AddText(sResolution);
  fitLegend->Draw();

  c->Update();
  return legend;
}

TLegend * DrawLegendFit(TCanvas *c, double x1, double y1, double x2, double y2,
                     TH1D *h, double fitMin, double fitMax, TF1 *peak,
                     TFitResultPtr results) {

  gStyle->SetLegendBorderSize(0.);
  gStyle->SetLegendFont(42);
  gStyle->SetLegendTextSize(.03);

  auto legend = new TLegend(x1, y1, x1+0.25, y1+0.25);
  legend->SetOption("NDC NB");
  legend->AddEntry(h, "Energy spectrum");
  legend->AddEntry(peak, "Gaussian fit");
  legend->SetFillStyle(0);
  legend->Draw();

  auto fitLegend = new TPaveText(x1+0.25, y1, x2, y2);
  fitLegend->SetOption("NDC NB");
  fitLegend->SetFillStyle(0);
  fitLegend->SetBorderSize(0.);
  fitLegend->SetTextFont(42);
  fitLegend->SetTextSize(.03);
  fitLegend->SetTextAlign(11);

  auto sEntries = Form("Entries: %.0f", h->GetEntries());
  auto sFitEntries = Form("Fit Entries: %.0f", h->Integral(h->FindBin(fitMin),
                                                           h->FindBin(fitMax)));
  auto sProb = Form("Prob. : %E", results->Prob());
  auto sMean = Form("Mean: %.2f #pm %.2f", peak->GetParameter(1), peak->GetParError(1));
  auto sSigma = Form("Sigma: %.2f #pm %.2f", peak->GetParameter(2), peak->GetParError(2));
  auto sResolution = Form("Resolution: %.2f%%", (peak->GetParameter(2)/peak->GetParameter(1))*100);
  fitLegend->AddText(sEntries);
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(sFitEntries);
  fitLegend->AddText(sMean);
  fitLegend->AddText(sSigma);
  fitLegend->AddText(sProb);
  fitLegend->AddText(sResolution);
  fitLegend->Draw();

  c->Update();
  return legend;
}

void DrawCalFits(TH1D * h, std::string figpath, std::string elementname,
                TF1 *fitFunc, int nbins, double LowLim, double UpLim,
                TFitResultPtr results) {

  auto c = new TCanvas();

  TPad *pad1 = new TPad("","",0,0.33,1,1);
  TPad *pad2 = new TPad("","",0,0,1,0.33);
  pad1->SetBottomMargin(0.00001);
  pad1->SetBorderMode(0);
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.2);
  pad2->SetBorderMode(0);
  pad1->Draw();
  pad2->Draw();

  pad1->cd();

  h->Draw();
  fitFunc->Draw("AL SAME");

  auto legend = DrawLegendFit(c, .1, .65, .35, .85, h,
                           LowLim, UpLim, fitFunc, results);
  legend->SetHeader(elementname.c_str(), "C");
  DrawDate(c);
  MyStyle(h, fitFunc);
  auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
  xaxis->SetLimits(LowLim, UpLim);
  pad2->cd(); pad2->SetGrid();

  auto res = new TGraph();

  for (int i=1; i<=nbins; i++) {
    if (h->GetBinContent(i)!=0) {
    auto diff = (h->GetBinContent(i) - fitFunc->Eval(h->GetBinCenter(i))) /
                h->GetBinError(i);
    res->AddPoint(h->GetBinCenter(i), diff);
  }
  }

  res->Draw("AP");
  res->SetMarkerStyle(8);
  res->SetMarkerSize(.4);

  auto zero = new TF1("zero", "0", LowLim, UpLim);
  zero->Draw("AL SAME");
  zero->SetLineStyle(9);
  zero->SetLineColor(kBlack);
  zero->SetLineWidth(1);

  auto resXaxis = res->GetXaxis(); resXaxis->SetLimits(LowLim, UpLim);
  resXaxis->SetTitle("Energy [keV]");
  resXaxis->SetTitleSize(.1); resXaxis->SetTitleOffset(.9);
  resXaxis->SetLabelSize(.08);
  auto resYaxis = res->GetYaxis();
  resYaxis->SetTitle("Norm. res.");
  resYaxis->SetTitleSize(.1); resYaxis->SetTitleOffset(.5);
  resYaxis->SetLabelSize(.08);

  c->SaveAs(figpath.c_str());
  //c->Destructor();
}
