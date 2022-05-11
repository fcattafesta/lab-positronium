#include "scripts/calibration.h"

void CalMethods() {

  std::string figpath[npeak] = {"figures/calibrationTrap/60Co1.pdf",
                                    "figures/calibrationTrap/60Co2.pdf",
                                    "figures/calibrationTrap/137Cs.pdf",
                                    "figures/calibrationTrap/22Na.pdf"},

               treepath[npeak] = {"data/60Co.root", "data/60Co.root",
                                 "data/137Cs.root", "data/22Na.root"},

               elementname[npeak] = {"{}^{60}Co", "{}^{60}Co",
                                     "{}^{137}Cs", "{}^{22}Na"},

               figregrpath[4] = {"figures/calibrationTrap/pol1.pdf",
                                 "figures/calibrationTrap/pol2.pdf",
                                 "figures/calibrationTrap/pol1_fix.pdf",
                                 "figures/calibrationTrap/pol2_fix.pdf"};

  TFitResultPtr results[4];

  TF1 * f_cal[4];

  f_cal[0] = new TF1("f_cal0", "pol1", -1, 2000);
  f_cal[1] = new TF1("f_cal1", "pol2", -1, 2000);
  f_cal[2] = new TF1("f_cal2", "[0]*x", -1, 2000);
  f_cal[3] = new TF1("f_cal3", "[0]*x + [1]*x*x", -1, 2000);

  for (int i=0; i<4; i++) {

    results[i] = Calibration(treepath, figpath, elementname, figregrpath[i],
                          f_cal[i]);
  }

}
