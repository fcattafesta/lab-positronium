

void Acceptance_2() {

  int count=0, n_ev = 1e8;

  double M = 1022;

  TRandom3 genx, geny, gen;


  for (int i=0; i<n_ev; i++) {


  double x0 = genx.Uniform(-1.7, 1.7);
  double y0 = geny.Uniform(-1.7, 1.7);





  double theta1 = gen.Uniform(-TMath::Pi(), 0);

  double ang1_min = -TMath::Pi() - TMath::ASin( (-12.0 - x0)/TMath::Sqrt( pow(+2.5 - y0, 2) + pow(-12.0 - x0, 2) ) );

  double ang1_max = -TMath::Pi() - TMath::ASin( (-12.0 - x0)/TMath::Sqrt( pow(-2.5 - y0, 2) + pow(-12.0 - x0, 2) ) );



  if (theta1<=ang1_max && theta1 >= ang1_min) count++;

  //cout << ang1_min << " " << ang1_max << " " << ang2_min << " " << ang2_max << " " << ang3_min << " " << ang3_max << endl;
  //cout << theta1 << " " << theta2 << " " << theta3 << endl;
  //cout << " " << endl;



  }

  double acc = 1.*count/n_ev;

  cout << acc << endl;










}
