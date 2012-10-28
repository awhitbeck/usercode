import ROOT

class superMela2Dtemplate:

    """
    Stanardized class for construncting 2D templates
    ======================
    DATA MEMBERS:
    ======================
    inputeFileName       - name of input file
    discVar              - RooRealVar of KD variable
    m4lVar               - RooRealVar of m4l variable
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
    TempDataHist_LeptSmearDown - RooDataHist of template shape (+1 sig. lepton resol)
    TempDataHist_LeptSmearDown - RooDataHist of template shape (-1 sig. lepton scale)
    TempDataHist_LeptSmearDown - RooDataHist of template shape (+1 sig. lepton scale)
    TempDataHist               - RooDataHist of nominal template shape
    ----------------------
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (-1 sig. lepton resol)
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (+1 sig. lepton resol)
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (-1 sig. lepton scale)
    TemplatePdf_LeptSmearDown - RooHistPdf of template shape (+1 sig. lepton scale)
    TemplatePdf               - RooHistPdf of nominal template shape
    ---------------------- 
    TemplateMorphPdf     - FastVerticalInterpHistPdf2D template
    morphVarList         - RooArgList of nuisance parameters
    funcList             - RooArgList of alternative shapes
    alphaMorph           - RooRealVar, nuisance parameter
    
    """

    def __init__(self,inputFileName,discVar,m4lVar,channel,sqrts,tag,morph,morphTag):

        self.TempFile = ROOT.TFile(inputFileName)
        self.discVar  = discVar 
        self.m4lVar   = m4lVar  
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
        self.Template_LeptScaleUp   = self.TempFile.Get("h_superDpsD_LeptScaleUp")
        self.Template_LeptScaleDown = self.TempFile.Get("h_superDpsD_LeptScaleDown")
        self.Template_LeptScaleUp   = self.TempFile.Get("h_superDpsD_LeptSmearUp")
        self.Template_LeptScaleDown = self.TempFile.Get("h_superDpsD_LeptSmearDown")

    def initRooDataHist(self):
    
        TemplateName = "TempDataHist_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template)
        TemplateName = "TempDataHist_LeptScaleUp_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_LeptScaleUp = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_LeptScaleUp)
        TemplateName = "TempDataHist_LeptScaleDown_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_LeptScaleDown = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_LeptScaleDown)
        TemplateName = "TempDataHist_LeptSmearUp_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_LeptSmearUp = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_LeptSmearUp)
        TemplateName = "TempDataHist_LeptSmearDown_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_LeptSmearDown = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_LeptSmearDown)
        
    def initRooHistPdf(self):
        
        TemplateName = "TemplatePdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist)
        TemplateName = "TemplatePdf_{2}_LeptScaleUp_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_LeptScaleUp =ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_LeptScaleUp)
        TemplateName = "TemplatePdf_{2}_LeptScaleDown_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_LeptScaleDown = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_LeptScaleDown)
        TemplateName = "TemplatePdf_{2}_LeptSmearUp_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_LeptSmearUp =ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_LeptSmearUp)
        TemplateName = "TemplatePdf_{2}_LeptSmearDown_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_LeptSmearDown = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_LeptSmearDown)
        
    def initFastVertMorph(self):

        # change the nuisance parameter name to match what alessio used
        # fill extra parameters and shapes into funcList and morphVarList

        self.morphVarList = ROOT.RooArgList()
        self.funcList     = ROOT.RooArgList()

        morphVarName = "CMS_zz4l_{1}MELA".format(self.channel,self.morphTag)
        self.alphaMorph   = ROOT.RooRealVar(morphVarName,morphVarName,0,-20,20)

        self.funcList.add(self.TemplatePdf)
        
        if(self.morph):
            self.morphVarList.add(self.alphaMorph)
            self.funcList.add(self.TemplatePdf_Up)
            self.funcList.add(self.TemplatePdf_Down)
            
        
        TemplateName = "TemplateMorphPdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplateMorphPdf = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,self.m4lVar,self.discVar,True,self.funcList,self.morphVarList,1.0,1)
        
