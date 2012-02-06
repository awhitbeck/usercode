// ===================================================
// script for getting simple significance and limits
// 1. declare observables and PDFs
// 2. build factory
// 3. run calcuatoins

void runSigAndLimits(){
    
    gSystem->Load("libRooFit");
    using namespace RooFit;
    
    gSystem->Load("../src/statsFactory_cc.so");
    
    // ================ O b s e r v a b l e s   a n d   P D F s
    // observables
    RooRealVar* mZZ = new RooRealVar("mZZ","mZZ",110,140);
    RooArgSet* observables = new RooArgSet( *mZZ );

    // signal PDF
    RooRealVar* mean = new RooRealVar("mean","mean", 125, 100., 600.); mean->setConstant( kTRUE );
    RooRealVar* sigma = new RooRealVar("sigma","sigma",2., 0., 30.); sigma->setConstant( kTRUE );
    RooGaussian* sigPDF = new RooGaussian("sigPDF","sigPDF",*mZZ,*mean,*sigma);
    
    // background PDF
    RooRealVar* p0 = new RooRealVar("p0","p0",1,-10,10); p0->setConstant( kTRUE );
    RooRealVar* p1 = new RooRealVar("p1","p1", 0,-10,10); p1->setConstant( kTRUE );
    RooPolynomial* bkgPDF = new RooPolynomial("bkgPDF","bkgPDF",*mZZ,RooArgList(*p0,*p1));
    
    // ================ B u i l d   s t a t s   f a c t o r y
    // arguments ---> (observables, pdf1, pdf2, output root file)
    statsFactory myHiggs1DTest( observables, sigPDF, bkgPDF, "test.root" ); 

    // ================ R u n   s i g n i f i c a n c e   c a l c u l a t i o n
    // arguments ---> (nSig, nBkg, nToys)
    myHiggs1DTest.runSignificance( 5, 10, 100 );

    // ================ R u n   u p p e r   l i m i t   c a l c u l a t i o n
    // arguments ---> (nSig, nBkg, nToys) [nSig is not really used though]
    myHiggs1DTest.runUpperLimit( 5, 10, 100 );
    
}
