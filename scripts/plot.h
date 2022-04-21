
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
                     TH1D *h, TF1 *peak) {

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
  auto sMean = Form("Mean: %.0f #pm %.0f", peak->GetParameter(1), peak->GetParError(1));
  auto sSigma = Form("Sigma: %.0f #pm %.0f", peak->GetParameter(2), peak->GetParError(2));
  fitLegend->AddText(sEntries);
  fitLegend->AddText("Fit results:");
  fitLegend->AddText(sMean);
  fitLegend->AddText(sSigma);
  fitLegend->Draw();

  c->Update();
  return legend;
}
