
using namespace RooFit;

void test(){

  m4lSignalBase *test[10];

  for(int i=0; i<10; i++){
    test[i] = new m4lSignalBase("4mu","8TeV","test",115.0+i*2.0);
    
    test[i]->initializePDFs("SM_inputs_8TeV/inputs_4mu.txt");

    if(i==0)
      RooPlot *plot = test[i]->m4l->frame();

    test[i]->m4lModel->plotOn(plot,LineColor(i));
 
  }

plot->Draw();

}
