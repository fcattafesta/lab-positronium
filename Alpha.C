
void Alpha() {

  double n3 = 9, n2 = 20137,
         dn3 = sqrt(n3), dn2 = sqrt(n2),
         e3 = 8e-4, e2 = 0.13,
         de3 = 0, de2 = 0,
         t3 = 7.5e3, t2 = 5.03336e2;

auto alpha = (n3 / (e3 * t3)) / (n2 / (e2 * t2));

auto dalpha = sqrt(pow(dn3/n3, 2) + pow(de3/e3, 2) +
                   pow(dn2/n2, 2) + pow(de2/e2, 2)) * alpha;

cout << alpha << " +- " << dalpha << endl;

}
