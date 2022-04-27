#include "spectrum.h"

TFitResultPtr FitGausPeak(TH1D *h, double eMin, double eMax) {

  TF1 *peak = new TF1("peak", "gaus", eMin, eMax);

  auto result = h->Fit(peak, "SRLNQ");

  return result;

}
