#include "scripts/plot.h"

void Signal() {

  std::string treepath = "data/137Cs.root";
  std::string figpath = "figures/signal/Cs_1833.pdf";
  int event = 5396;

  auto base = new TF1("base", "pol0", 0, 400);

  auto res = new TGraph();


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

  auto g = DrawSignal(treepath, event);

  auto noise = new TH1D("noise", "noise", 20, -100, 100);

  g->Fit(base, "RQN");

  double thr = base->GetParameter(0) - 0.05 * (base->GetParameter(0)- TMath::MinElement(RECORD_LENGTH, g->GetY()));
  int t1 = 0, t2 = 0;

  for (int k=0; k<RECORD_LENGTH; k++) {
    auto amp = g->GetPointY(k);
    if (amp < thr) {
      if (t1==0) t1 = k;
      if (t1!=0) t2 = k;
    }
  }


  cout << t1 << " " << g->GetPointY(t1) << "  " << t2 << " " << g->GetPointY(t2) << endl;

  auto soglia = new TF1("soglia", "pol1", t1, t2);
  soglia->SetParameters(thr, 0);

  for (int i=0; i<400; i++) {
    auto amp = g->GetPointY(i);
    noise->Fill(amp - base->GetParameter(0));
  }

  g->GetXaxis()->SetLimits(t1, t2);



  cout << base->GetParError(0) << endl;
  cout << noise->GetRMS() << endl;


  base->SetRange(t1, t2);

  base->Draw("SAME AL");
  soglia->Draw("SAME AL");

  for (int i=t1; i<t2; i++) {
    auto diff = g->GetPointY(i) - base->Eval(i);
    res->AddPoint(i, diff);
  }

  pad2->cd();

  res->Draw("AP");
  res->SetMarkerStyle(8);
  res->SetMarkerSize(.4);

  auto zero = new TF1("zero", "0", t1, t2);
  zero->Draw("AL SAME");
  zero->SetLineStyle(9);
  zero->SetLineColor(kBlack);
  zero->SetLineWidth(1);

  auto resXaxis = res->GetXaxis(); resXaxis->SetLimits(t1, t2);
  resXaxis->SetTitle("Time [u.a.]");
  resXaxis->SetTitleSize(.1); resXaxis->SetTitleOffset(.9);
  resXaxis->SetLabelSize(.08);
  auto resYaxis = res->GetYaxis();
  resYaxis->SetRangeUser(-30, 10);
  resYaxis->SetTitle("Res.");
  resYaxis->SetTitleSize(.1); resYaxis->SetTitleOffset(.5);
  resYaxis->SetLabelSize(.08);


  //c->SaveAs(figpath.c_str());





}
