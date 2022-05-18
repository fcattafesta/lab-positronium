#include "scripts/calibration_3.h"

void CalTime_3() {

  std::string figpath[2][npeak] = {{"figures/time/1005/60Co1.pdf",
                                 "figures/time/1005/60Co2.pdf",
                                 "figures/time/1005/137Cs.pdf"},
                                {"figures/time/1005/60Co1_check.pdf",
                                 "figures/time/1005/60Co2_check.pdf",
                                 "figures/time/1005/137Cs_check.pdf"}},

               //treepath[2][npeak] = {{"data/1005/60Co.root", "data/1005/60Co.root", "data/1005/137Cs.root"},
                                     //{"data/1105/60Co.root", "data/1105/60Co.root", "data/1105/137Cs.root"}},

               treepath[2][npeak] = {{"data/1005/60Co.root", "data/1005/60Co.root", "data/1005/137Cs.root"},
                                     {"data/1005/60Co_check.root", "data/1005/60Co_check.root", "data/1005/137Cs_check.root"}},

               elementname[npeak] = {"{}^{60}Co", "{}^{60}Co", "{}^{137}Cs"},

               figregrpath[2] = {"figures/time/1005_before.pdf",
                                 "figures/time/1005_after.pdf"};

  TFitResultPtr results[2];

  auto f_cal = new TF1("f_cal", "[0]*x + [1]*x*x", -1, 2000);

  for (int i=0; i<2; i++) {

    results[i] = Calibration(treepath[i], figpath[i], elementname, figregrpath[i], f_cal);
  }

}
