#include "scripts/Calibration.h"

void CalTime(int pol=1) {



  std::string figpath[3][npeak] = {{"figures/time/1005/60Co1.pdf",
                                 "figures/time/1005/60Co2.pdf",
                                 "figures/time/1005/137Cs.pdf",
                                 "figures/time/1005/22Na.pdf"},
                                {"figures/time/1105/60Co1.pdf",
                                 "figures/time/1105/60Co2.pdf",
                                 "figures/time/1105/137Cs.pdf",
                                 "figures/time/1105/22Na.pdf"},
                                 {"figures/time/1205/60Co1.pdf",
                                  "figures/time/1205/60Co2.pdf",
                                  "figures/time/1205/137Cs.pdf",
                                  "figures/time/1205/22Na.pdf"}},

               treepath[3][npeak] = {{"data/1005/60Co.root", "data/1005/60Co.root",
                                     "data/1005/137Cs.root", "data/1005/22Na.root"},
                                    {"data/1105/60Co.root", "data/1105/60Co.root",
                                     "data/1105/137Cs.root", "data/1105/22Na.root"},
                                     {"data/1205/60Co.root", "data/1205/60Co.root",
                                      "data/1205/137Cs.root", "data/1205/22Na.root"}},

               elementname[npeak] = {"{}^{60}Co", "{}^{60}Co",
                                     "{}^{137}Cs", "{}^{22}Na"},

               figregrpath[3] = {Form("figures/time/1005_pol%dfix.pdf", pol),
                                 Form("figures/time/1105_pol%dfix.pdf", pol),
                                 Form("figures/time/1205_pol%dfix.pdf", pol)};



  TFitResultPtr results[3];

  if (pol==1) {
    auto f_cal = new TF1("f_cal", "[0]*x", -1, 2000);
    for (int i=0; i<3; i++) results[i] = Calibration(treepath[i], figpath[i],
                                                     elementname, figregrpath[i], f_cal);
  }

  if (pol==2) {
    auto f_cal = new TF1("f_cal", "[0]*x + [1]*x*x", -1, 2000);
    for (int i=0; i<3; i++) results[i] = Calibration(treepath[i], figpath[i],
                                                   elementname, figregrpath[i], f_cal);
  }


  cout << " " << endl;
  cout << "Risultati fit di regressione:" << endl;
  cout << " " << endl;

  for (int i=0; i<3; i++) {

    if (pol==1) {
    cout << "double  " << Form("slope = %.2f", results[i]->GetParams()[0]) << Form(", d_slope = %.2f;", results[i]->GetErrors()[0]) << endl;
    cout << " " << endl;
    }
    if (pol==2) {
    cout << "double  " << Form("a = %.4f", results[i]->GetParams()[1]) << Form(", da = %.4f;", results[i]->GetErrors()[1]) << endl;
    cout << "double  " << Form("b = %.2f", results[i]->GetParams()[0]) << Form(", db = %.2f;", results[i]->GetErrors()[0]) << endl;
    cout << "double  " << Form("corr = %.2f", results[i]->Correlation(0,1)) << endl;
    cout << " " << endl;
    }

  }

  cout << figregrpath[1] << endl;

}
