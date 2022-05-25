

void Acceptance_3() {

  int count=0, n_ev = 1e8;

  double M = 1022;

  TRandom3 genx, geny, gen;


  double x_1 = -25.5, y_1 = -14.9,
         //x_1 = -30.0, y_1 = -17.5,
         x_2 = -x_1, y_2 = y_1, x_3 = 0, y_3 = 16.7;

  double r = 2.5, th_1 = TMath::Pi()/3;

  double dx = r*TMath::Cos(th_1), dy = r*TMath::Sin(th_1);



  for (int i=0; i<n_ev; i++) {


  double x0 = genx.Uniform(-1.7, 1.7);
  double y0 = geny.Uniform(-1.7, 1.7);

  /*
  double ang_12_min = TMath::ASin( TMath::Abs( (-23.7 - x0)/TMath::Sqrt( pow(-17.1 - y0, 2) + pow(-23.7 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (+23.7 - x0)/TMath::Sqrt( pow(-17.1 - y0, 2) + pow(+23.7 - x0, 2) ) ) );

  double ang_12_max = TMath::ASin( TMath::Abs( (-26.8 - x0)/TMath::Sqrt( pow(-12.7 - y0, 2) + pow(-26.8 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (+26.8 - x0)/TMath::Sqrt( pow(-12.7 - y0, 2) + pow(+26.8 - x0, 2) ) ) );

  double ang_13_min = TMath::ASin( TMath::Abs( (-26.8 - x0)/TMath::Sqrt( pow(-12.7 - y0, 2) + pow(-26.8 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (-2.5 - x0)/TMath::Sqrt( pow(16.7 - y0, 2) + pow(-2.5 - x0, 2) ) ) );

  double ang_13_max = TMath::ASin( TMath::Abs( (-23.7 - x0)/TMath::Sqrt( pow(-17.1 - y0, 2) + pow(-23.7 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (+2.5 - x0)/TMath::Sqrt( pow(16.7 - y0, 2) + pow(+2.5 - x0, 2) ) ) );

  double ang_23_min = TMath::ASin( TMath::Abs( (+26.8 - x0)/TMath::Sqrt( pow(-12.7 - y0, 2) + pow(+26.8 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (+2.5 - x0)/TMath::Sqrt( pow(16.7 - y0, 2) + pow(+2.5 - x0, 2) ) ) );

  double ang_23_max = TMath::ASin( TMath::Abs( (+23.7 - x0)/TMath::Sqrt( pow(-17.1 - y0, 2) + pow(+23.7 - x0, 2) ) ) ) +
                      TMath::ASin( TMath::Abs( (-2.5 - x0)/TMath::Sqrt( pow(16.7 - y0, 2) + pow(-2.5 - x0, 2) ) ) );





  double theta12 = gen.Uniform(0, TMath::Pi());
  double theta13 = gen.Uniform(0, TMath::Pi());
  double theta23 = 2*TMath::Pi() - theta12 - theta13;

  //cout << ang_12_min << " " << ang_12_max << " " << ang_13_min << " " << ang_13_max << " " << ang_23_min << " " << ang_23_max << endl;
  //cout << theta12 << " " << theta13 << " " << theta23 << endl;
  //cout << " " << endl;


  if (
    (theta12<=ang_12_max && theta12>=ang_12_min && theta13<=ang_13_max && theta13>=ang_13_min && theta23<=ang_23_max && theta23>=ang_23_min) ||
    (theta12<=ang_12_max && theta12>=ang_12_min && theta13<=ang_23_max && theta13>=ang_23_min && theta23<=ang_13_max && theta23>=ang_13_min) ||
    (theta12<=ang_13_max && theta12>=ang_13_min && theta13<=ang_12_max && theta13>=ang_12_min && theta23<=ang_23_max && theta23>=ang_23_min) ||
    (theta12<=ang_13_max && theta12>=ang_13_min && theta13<=ang_23_max && theta13>=ang_23_min && theta23<=ang_13_max && theta23>=ang_13_min) ||
    (theta12<=ang_23_max && theta12>=ang_23_min && theta13<=ang_13_max && theta13>=ang_13_min && theta23<=ang_12_max && theta23>=ang_12_min) ||
    (theta12<=ang_23_max && theta12>=ang_23_min && theta13<=ang_12_max && theta13>=ang_12_min && theta23<=ang_13_max && theta23>=ang_13_min))
    count++;

  */

  double Energy1 = gen.Uniform(0, M);
  double Energy2 = gen.Uniform(0, M-Energy1);
  double Energy3 = M - Energy1 - Energy2;

  double theta1 = gen.Uniform(-TMath::Pi(), TMath::Pi());
  double theta2 = gen.Uniform(-TMath::Pi(), TMath::Pi());
  double theta3 = TMath::ACos(M - 1 - Energy1*(1+TMath::Cos(theta1)) - Energy2*(1+TMath::Cos(theta2)));

  double ang1_min = -TMath::Pi() - TMath::ASin( (x_1 + dx - x0)/TMath::Sqrt( pow(y_1 - dy - y0, 2) + pow(x_1 + dx - x0, 2) ) );

  double ang1_max = -TMath::Pi() - TMath::ASin( (x_1 - dx - x0)/TMath::Sqrt( pow(y_1 + dy - y0, 2) + pow(x_1 - dx - x0, 2) ) );

  double ang2_min = TMath::Pi() - TMath::ASin( (x_2 + dx - x0)/TMath::Sqrt( pow(y_2 + dy - y0, 2) + pow(x_2 + dx - x0, 2) ) );

  double ang2_max = TMath::Pi() - TMath::ASin( (x_2 - dx - x0)/TMath::Sqrt( pow(y_2 - dy - y0, 2) + pow(x_2 - dx - x0, 2) ) );

  double ang3_min = TMath::ASin( (x_3 - r - x0)/TMath::Sqrt( pow(y_3 - y0, 2) + pow(x_3 - r - x0, 2) ) );

  double ang3_max = TMath::ASin( (x_3 + r - x0)/TMath::Sqrt( pow(y_3 - y0, 2) + pow(x_3 + r - x0, 2) ) );


  if (
    (theta1<=ang1_max && theta1 >= ang1_min && theta2<=ang2_max && theta2 >= ang2_min && theta3<=ang3_max && theta3 >= ang3_min) ||
    (theta1<=ang1_max && theta1 >= ang1_min && theta2<=ang3_max && theta2 >= ang3_min && theta3<=ang2_max && theta3 >= ang2_min) ||
    (theta1<=ang2_max && theta1 >= ang2_min && theta2<=ang1_max && theta2 >= ang1_min && theta3<=ang3_max && theta3 >= ang3_min) ||
    (theta1<=ang2_max && theta1 >= ang2_min && theta2<=ang3_max && theta2 >= ang3_min && theta3<=ang1_max && theta3 >= ang1_min) ||
    (theta1<=ang3_max && theta1 >= ang3_min && theta2<=ang2_max && theta2 >= ang2_min && theta3<=ang1_max && theta3 >= ang1_min) ||
    (theta1<=ang3_max && theta1 >= ang3_min && theta2<=ang1_max && theta2 >= ang1_min && theta3<=ang2_max && theta3 >= ang2_min)
  ) count++;

  //cout << ang1_min << " " << ang1_max << " " << ang2_min << " " << ang2_max << " " << ang3_min << " " << ang3_max << endl;
  //cout << theta1 << " " << theta2 << " " << theta3 << endl;
  //cout << " " << endl;



  }

  double acc = 1.*count/n_ev;

  cout << acc << endl;










}
