#include "scripts/calibration.h"

void CalTime() {

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

               figregrpath[3] = {"figures/time/1005_pol2fix.pdf",
                                 "figures/time/1105_pol2fix.pdf",
                                 "figures/time/1205_pol2fix.pdf"};

  TFitResultPtr results[3];

  auto f_cal = new TF1("f_cal", "[0]*x + [1]*x*x", -1, 2000);

  for (int i=0; i<3; i++) results[i] = Calibration(treepath[i], figpath[i],
                                                   elementname, figregrpath[i], f_cal);


}
