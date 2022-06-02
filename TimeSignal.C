#include "scripts/plot.h"
//int RECORD_LENGTH = 1030;

void TimeSignal() {

  auto f = new TFile("data/1205/pmt1/137Cs.root", "READ");
  auto t = f->Get<TTree>("tree;2");

  int nentries = t->GetEntries();
  int v[RECORD_LENGTH];
  double Time;
  double counter_eq = 0, counter_neq = 0;

  t->SetBranchAddress("Amplitudes", &v);

  for (int i=1; i<2; i++) {

    auto g = new TGraph();
    t->GetEntry(i);

    for (int j=0; j<RECORD_LENGTH; j++) {
      g->AddPoint(j, v[j]);
    }

    TF1 *base = new TF1("base", "[0]", 0, 300);
    g->Fit(base, "RQN");

    auto baseline = base->GetParameter(0);

    int thr_1 = baseline - 50, thr_2 = baseline - 100;
    int time_1 = 0, time_2 = 0;

    for (int j=0; j<RECORD_LENGTH; j++) {
      if (v[j] <= thr_1) {
        time_1 = j;
        break;
      }
    }

    for (int j=0; j<RECORD_LENGTH; j++) {
      if (v[j] <= thr_2) {
        time_2 = j;
        break;
      }
    }

    double m, q;

    if (time_1 != time_2) {
      m = (thr_2 - thr_1) / (time_2 - time_1);
      q = thr_1 - m * time_1;
      Time = ((baseline - q) / m) * 4;
      counter_neq = counter_neq + 1;
    }
    else {
      Time = time_1 * 4;
      counter_eq = counter_eq + 1;
    }

    auto line = new TF1("line", "pol1", 0, 1029);
    line->SetParameters(q, m);
    base->SetRange(0, 1029);

    auto c = new TCanvas();
    g->Draw("AL");
    line->Draw("AL SAME");
    base->Draw("AL SAME");
    base->SetLineColor(kBlue);
    DrawDate(c);
    auto xaxis = g->GetXaxis(); xaxis->SetTitle("Time [u.a.]");
    auto yaxis = g->GetYaxis();
    yaxis->SetTitle("Amplitude [a.u.]");
    yaxis->SetTitleOffset(1.4);
    c->SaveAs("figures/signal/137Cs_t.pdf");
  }


  f->Close();
}
