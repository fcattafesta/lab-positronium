#include "scripts/Calibration_2.h"

void CalSingle() {

  std::string figpath[npeak] = {"figures/time_2/1005/60Co1.pdf",
                                "figures/time_2/1005/60Co2.pdf",
                                "figures/time_2/1105/137Cs.pdf",
                                "figures/time_2/1105/22Na.pdf"},

               treepath[npeak] = {"data/calibration_2/1105/60Co.root", "data/calibration_2/1105/60Co.root",
                                  "data/calibration_2/1105/137Cs.root", "data/calibration_2/1105/22Na.root"},

               elementname[npeak] = {"{}^{60}Co", "{}^{60}Co",
                                     "{}^{137}Cs", "{}^{22}Na"},

               figregrpath = "figures/time_2/1105_pol2fix.pdf";


  TFitResultPtr results;


  TF1 * f_cal = new TF1("f_cal", "[0]*x + [1]*x*x", -1, 2000);

  results = Calibration_2(treepath, figpath, elementname, figregrpath, f_cal);


}
