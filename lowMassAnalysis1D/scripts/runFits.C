
void runFits(){
  gROOT->ProcessLine(".L fitmZZSignal.C");
  gROOT->ProcessLine(".L fitmZZBackground.C");
  const int numMasses=5;
  const int numBtags=3;
  int mass[numMasses]={130,140,150,160,170};
  int btag[numBtags]={0,1,2};
  for(int iMass=0; iMass<5; iMass++){
    for(int iBtag=0; iBtag<numBtags; iBtag++){
      fitmZZSignal(mass[iMass],btag[iBtag],0);
      fitmZZSignal(mass[iMass],btag[iBtag],1);
      fitmZZBackground(mass[iMass],btag[iBtag],0);
      fitmZZBackground(mass[iMass],btag[iBtag],1);
    }//end loop over btags
  }//end loop over masses
}
