import ROOT

class superMela2Dtemplate:

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
    Template               - TH2F of nominal template shape
    Template_LeptScaleUp   - TH2F of template shape (+1 sig. lepton scale)
    Template_LeptScaleDown - TH2F of template shape (-1 sig. lepton scale)
    Template_LeptScaleUp   - TH2F of template shape (+1 sig. lepton resol)
    Template_LeptScaleDown - TH2F of template shape (-1 sig. lepton resol)
    ----------------------
    TempDataHist_LeptSmearDown - RooDataHist of template shape (-1 sig. lepton resol)
    TempDataHist_LeptSmearUp   - RooDataHist of template shape (+1 sig. lepton resol)
    TempDataHist_LeptSmearDown - RooDataHist of template shape (-1 sig. lepton scale)
    TempDataHist_LeptSmearUp   - RooDataHist of template shape (+1 sig. lepton scale)
    TempDataHist               - RooDataHist of nominal template shape
    ----------------------
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (-1 sig. lepton resol)
    TemplatePdf_LeptSmearUp   - RooHistPdf of template shape (+1 sig. lepton resol)
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (-1 sig. lepton scale)
    TemplatePdf_LeptSmearUp   - RooHistPdf of template shape (+1 sig. lepton scale)
    TemplatePdf               - RooHistPdf of nominal template shape
    ---------------------- 
    Pdf                  - FastVerticalInterpHistPdf2D template
    morphVarList         - RooArgList of nuisance parameters
    funcList             - RooArgList of alternative shapes
    alphaMorph_scale     - RooRealVar, nuisance parameter for scale unc.
    alphaMorph_resol     - RooRealVar, nuisance parameter for resol unc.

    """

    def __init__(self,inputFileName,discVar,smdVar,channel,sqrts,tag,morph,morphTag):

        self.inputFileName = inputFileName
        self.TempFile = ROOT.TFile(inputFileName)
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

        self.Template      = self.TempFile.Get("h_superDpsD")

        if (self.morph):
            self.Template_LeptScaleUp   = self.TempFile.Get("h_superDpsD_LeptScaleUp")
            self.Template_LeptScaleDown = self.TempFile.Get("h_superDpsD_LeptScaleDown")
            self.Template_LeptSmearUp   = self.TempFile.Get("h_superDpsD_LeptSmearUp")
            self.Template_LeptSmearDown = self.TempFile.Get("h_superDpsD_LeptSmearDown")
            
        # Catch if there was an error getting TH2

        if ( self.Template == None):
            print "superMELA2Dtemplate::loadTemplates - ERROR"
            print self.inputFileName, "h_superDpsD"

        if ( self.morph ):
            
            if ( self.Template_LeptScaleUp == None ):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName, "h_superDpsD_LeptScaleUp"
                
            if ( self.Template_LeptScaleDown == None):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName, "h_superDpsD_LeptScaleDown"
                    
            if ( self.Template_LeptSmearUp == None):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName, "h_superDpsD_LeptSmearUp"
                        
            if ( self.Template_LeptSmearDown == None):
                print "superMELA2Dtemplate::loadTemplates - ERROR"
                print self.inputFileName, "h_superDpsD_LeptSmearDown"            

    def initRooDataHist(self):
    
        TemplateName = "TempDataHist_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template)

        if ( self.morph ):
            TemplateName = "TempDataHist_LeptScaleUp_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_LeptScaleUp = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_LeptScaleUp)
            TemplateName = "TempDataHist_LeptScaleDown_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_LeptScaleDown = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_LeptScaleDown)
            TemplateName = "TempDataHist_LeptSmearUp_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_LeptSmearUp = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_LeptSmearUp)
            TemplateName = "TempDataHist_LeptSmearDown_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TempDataHist_LeptSmearDown = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.smdVar,self.discVar),self.Template_LeptSmearDown)
            
    def initRooHistPdf(self):
        
        TemplateName = "TemplatePdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist)

        if ( self.morph ):
            TemplateName = "TemplatePdf_{2}_LeptScaleUp_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_LeptScaleUp =ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_LeptScaleUp)
            TemplateName = "TemplatePdf_{2}_LeptScaleDown_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_LeptScaleDown = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_LeptScaleDown)
            TemplateName = "TemplatePdf_{2}_LeptSmearUp_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_LeptSmearUp =ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_LeptSmearUp)
            TemplateName = "TemplatePdf_{2}_LeptSmearDown_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
            self.TemplatePdf_LeptSmearDown = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.smdVar,self.discVar),self.TempDataHist_LeptSmearDown)
            
    def initFastVertMorph(self):

        self.morphVarList = ROOT.RooArgList()
        self.funcList     = ROOT.RooArgList()

        morphVarName = "CMS_zz4l_smd_leptScale_{1}_{0:.0f}".format(self.channel,self.morphTag)
        self.alphaMorph_scale   = ROOT.RooRealVar(morphVarName,morphVarName,0,-20,20)
        morphVarName = "CMS_zz4l_smd_leptResol_{1}_{0:.0f}".format(self.channel,self.morphTag)
        self.alphaMorph_resol   = ROOT.RooRealVar(morphVarName,morphVarName,0,-20,20)

        self.funcList.add(self.TemplatePdf)
        
        if(self.morph):
            self.morphVarList.add(self.alphaMorph_scale)
            self.morphVarList.add(self.alphaMorph_resol)
            self.funcList.add(self.TemplatePdf_LeptScaleUp)
            self.funcList.add(self.TemplatePdf_LeptScaleDown)
            self.funcList.add(self.TemplatePdf_LeptSmearUp)
            self.funcList.add(self.TemplatePdf_LeptSmearDown)
            
        TemplateName = "TemplateMorphPdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.Pdf = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,self.smdVar,self.discVar,False,self.funcList,self.morphVarList,1.0,1)
        
