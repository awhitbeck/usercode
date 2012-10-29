import ROOT

class superMela2Dtemplate_zjets:

    """
    Stanardized class for construncting 2D templates
    ======================
    DATA MEMBERS:
    ======================
    inputeFileName       - name of input file
    discVar              - RooRealVar of KD variable
    smdVar               - RooRealVar of superMELA variable
    channel              - integer denoting channel 1:4mu 2:4e 3:2e2mu
    sqrts                - integer denoting
    tag                  - string denoting channel eg. ggH, ZH, WH, etc.
    morph                - boolean, true if template is morphable
    morphTag             - string to distinguish morph nuisance param
    TempFile             - TFile for reading histograms from
    ----------------------
    Template                  - TH2F of nominal template shape
    Template_Up               - TH2F of template shape (+1 sig. lepton scale)
    Template_Down             - TH2F of template shape (-1 sig. lepton scale)
    ----------------------
    TempDataHist_Down         - RooDataHist of template shape (-1 sig. lepton resol)
    TempDataHist_Up           - RooDataHist of template shape (+1 sig. lepton resol)
    TempDataHist              - RooDataHist of nominal template shape
    ----------------------
    TemplatePdf_Down          - RooHistPdf of template shape (-1 sig. lepton resol)
    TemplatePdf_Up            - RooHistPdf of template shape (+1 sig. lepton resol)
    TemplatePdf               - RooHistPdf of nominal template shape
    ---------------------- 
    Pdf                  - FastVerticalInterpHistPdf2D template
    morphVarList         - RooArgList of nuisance parameters
    funcList             - RooArgList of alternative shapes
    alphaMorph           - RooRealVar, nuisance parameter for scale unc.

    """

    def __init__(self,inputFileName_zjets,inputFileName_qqZZ,discVar,smdVar,channel,sqrts,tag,morph,morphTag):

        self.inputFileName_qqZZ  = inputFileName_qqZZ
        self.TempFile_qqZZ       = ROOT.TFile(inputFileName_qqZZ)
        self.inputFileName_zjets = inputFileName_zjets
        self.TempFile_zjets      = ROOT.TFile(inputFileName_zjets)
        self.discVar  = discVar 
        self.smdVar   = smdVar  
        self.channel  = channel
        self.sqrts    = sqrts
        self.tag      = tag
        self.morph    = morph
        self.morphTag = morphTag
        self.loadTemplates()
        self.initRooDataHist()
        self.initRooHistPdf()
        self.initFastVertMorph()
        
    def loadTemplates(self):

        self.Template      = self.TempFile_zjets.Get("h_superDpsD")

        if (self.morph):
            self.Template_Up   = self.TempFile_qqZZ.Get("h_superDpsD")
            self.Template_Down = self.TempFile_zjets.Get("h_superDpsD")
            
        # Catch if there was an error getting TH2

        if ( self.Template == None):
            print "superMELA2Dtemplate::loadTemplates - ERROR"
            print self.inputFileName_zjets, "h_superDpsD"

        if ( self.morph ):
            
            if ( self.Template_Up == None ):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName_qqZZ, "h_superDpsD"
                
            if ( self.Template_Down == None):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName_zjets, "h_superDpsD"
                    
    def initRooDataHist(self):
    
        TemplateName = "TempDataHist_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template)

        if ( self.morph ):
            TemplateName = "TempDataHist_Up_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_Up = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_Up)
            TemplateName = "TempDataHist_Down_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_Down = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_Down)
            
    def initRooHistPdf(self):
        
        TemplateName = "TemplatePdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist)

        if ( self.morph ):
            TemplateName = "TemplatePdf_{2}_Up_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_Up =ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_Up)
            TemplateName = "TemplatePdf_{2}_Down_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_Down)
            
    def initFastVertMorph(self):

        self.morphVarList = ROOT.RooArgList()
        self.funcList     = ROOT.RooArgList()

        morphVarName = "CMS_zz4l_smd_zjets_bkg_{0:.0f}".format(self.channel)
        self.alphaMorph   = ROOT.RooRealVar(morphVarName,morphVarName,0,-20,20)

        self.funcList.add(self.TemplatePdf)
        
        if(self.morph):
            self.morphVarList.add(self.alphaMorph)
            self.funcList.add(self.TemplatePdf_Up)
            self.funcList.add(self.TemplatePdf_Down)
            
        TemplateName = "TemplateMorphPdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.Pdf = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,self.smdVar,self.discVar,False,self.funcList,self.morphVarList,1.0,1)
        
