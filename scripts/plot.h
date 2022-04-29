#include "dataset.h"

void DrawSignal(std::string treepath, int event) {

  auto f = new TFile(treepath.c_str(), "READ");
  auto t = f->Get<TTree>("tree;2"); //tree does not exist after an update (e.g. tree;1)

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
    }
  }
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
  auto sFitEntries = Form("Fit Entries: %.0f", h->Integral(h->FindBin(fitMin),
                                                           h->FindBin(fitMax)));
  auto sMean = Form("Mean: %.2f #pm %.2f", peak->GetParameter(1), peak->GetParError(1));
  auto sSigma = Form("Sigma: %.2f #pm %.2f", peak->GetParameter(2), peak->GetParError(2));
  auto sResolution = Form("Resolution: %.2f%%", (peak->GetParameter(2)/peak->GetParameter(1))*100);
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
