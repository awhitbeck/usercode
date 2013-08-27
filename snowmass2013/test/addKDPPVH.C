
// 
// run by root -l -n loadlib.C addKDPPVH.C\
// 
void addKDPPVH(TString fileDir,  TString fileName, bool withAcc) {
  
  double sqrtsVal=14000;
  double mH=125.;
  KDcalcPPZH test(sqrtsVal, mH);
  test.addDtoTree(Form("%s/%s.root", fileDir.Data(), fileName.Data()),"SelectedTree", withAcc);
}
