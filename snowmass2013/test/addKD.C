
// 
// run by root -l -n loadlib.C addKD.C
// 
void addKD() {
  
  double sqrtsVal = 250;
  double mH = 125.;
  KDcalcZH test(sqrtsVal, mH);
  test.addDtoTree("Events_20130618/unweighted_unpol_g1_1M_false.root", "SelectedTree");
  test.addDtoTree("Events_20130618/unweighted_unpol_g4_1M_false.root", "SelectedTree");
  test.addDtoTree("Events_20130626/unweighted_unpol_f_3_250GeV_5M_false.root", "SelectedTree");
  test.addDtoTree("bkgData/ee_ZZ_llbb_250GeV_25M_false.root","SelectedTree");
}
