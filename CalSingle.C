#include "scripts/Calibration_3.h"

void CalSingle() {

  std::string figpath[npeak] = {"figures/time_3/2505/60Co1.pdf",
                                //"figures/time_3/2505/60Co2.pdf",
                                "figures/time_3/2505/137Cs.pdf",
                                //"figures/time_3/2505/22Na.pdf"
                              },

               treepath[npeak] = {"data/calibration_3/2505/60Co.root",
                                  //"data/calibration_3/2505/60Co.root",
                                  "data/calibration_3/2505/137Cs.root",
                                  //"data/calibration_3/2505/22Na.root"
                                },

               elementname[npeak] = {"{}^{60}Co", //"{}^{60}Co",
                                     "{}^{137}Cs"},//, "{}^{22}Na"},

               figregrpath = "figures/time_3/2505_pol1fix.pdf";


  TFitResultPtr results;


  TF1 * f_cal = new TF1("f_cal", "[0]*x", -1, 2000);

  results = Calibration_3(treepath, figpath, elementname, figregrpath, f_cal);


}
