#include "../scripts/spectrum.h"
#include "../scripts/plot.h"

Bool_t reject = kTRUE;

double background(double *x, double *p) {
  if (reject && x[0]>=46e3 && x[0] <= 53e3) {
    TF1::RejectPoint();
    return 0;
  }
  return p[0] + p[1] * x[0] + p[2] * x[0] * x[0];
}

double signal(double *x, double *p) {
  return p[0] * exp(-0.5 * pow(((x[0] - p[1]) / p[2]), 2));
}

double func(double *x, double *p) {
  return signal(x, p) + background(x, &p[3]);
}

void Cobalt1() {

  std::string treepath = "data/range/60Co.root",
              branchname = "EnergyTrap",
              treename[3] = {"tree;2", "tree;3", "tree;4"},
              figpath[3] = {"figures/range/cobalt1/0_1029.pdf",
                            "figures/range/cobalt1/200_800.pdf",
                            "figures/range/cobalt1/300_700.pdf"},
              elementname[3] = {"0-1029", "200-800", "300-700"};

  double LowLim = 40e3, UpLim = 53e3;
  double fitMin = 40e3, fitMax = 53e3;
  int nbins = 100;

  for (int i=0; i<3; i++) {

    if (i==2) {UpLim = 52e3; fitMax = 52e3;
               LowLim = 39e3; fitMin = 39e3;}

    auto h = MakeSpectrum(treepath, treename[i], branchname, nbins, LowLim, UpLim);

    TF1 *bkg = new TF1("bkg", background, fitMin, fitMax, 3);
    bkg->SetParameters(400, -.02, 0.);

    h->Fit(bkg, "SRLNQ");

    reject = kFALSE;

    TF1 *fitFunc = new TF1("fitFunc", func, fitMin, fitMax, 6);
    fitFunc->SetParameters(.2e3, 49e3, 1.1e3,
                           bkg->GetParameter(0), bkg->GetParameter(1),
                           bkg->GetParameter(2));

    auto results = h->Fit(fitFunc, "SRLNQI");

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

    auto legend = DrawLegendFit(c, .1, .65, .55, .85, h,
                             fitMin, fitMax, fitFunc, results);
    legend->SetHeader(elementname[i].c_str(), "C");
    DrawDate(c);
    MyStyle(h, fitFunc);

    auto xaxis = h->GetXaxis(); xaxis->SetTitle("Energy [keV]");
    xaxis->SetLimits(LowLim, UpLim);
    pad2->cd(); pad2->SetGrid();

    auto res = new TGraph();

    for (int j=1; j<=nbins; j++) {
      auto diff = (h->GetBinContent(j) - fitFunc->Eval(h->GetBinCenter(j))) /
                  h->GetBinError(j);
      res->AddPoint(h->GetBinCenter(j), diff);
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

    c->SaveAs(figpath[i].c_str());
    c->Destructor();
    reject = kTRUE;
  }
}
