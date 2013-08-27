
// 
// run by root -l -n loadlib.C addKD.C
// 
void addKD(TString fileDir, TString fileName, double sqrtsVal) 
{
  
  double mH = 125.;
  KDcalcZH test(sqrtsVal, mH);
  test.addDtoTree(Form("%s/%s.root", fileDir.Data(), fileName.Data()),"SelectedTree");
}
