import ROOT

class mela2Dtemplate:

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
    Template             - TH2F of nominal template shape 
    Template_Up          - TH2F of upward template shape  
    Template_Down        - TH2F of downward template shape
    ----------------------
    TempDataHist_Down    - RooDataHist of downward template shape
    TempDataHist_Up      - RooDataHist of upward template shape
    TempDataHist         - RooDataHist of nominal template shape
    ----------------------
    TemplatePdf_Down     - RooHistPdf of downward template shape
    TemplatePdf_Up       - RooHistPdf of upward template shape
    TemplatePdf          - RooHistPdf of nominal template shape
    ---------------------- 
    Pdf     - FastVerticalInterpHistPdf2D template
    """

    def __init__(self,inputFileName,discVar,m4lVar,channel,sqrts,tag,morph,morphTag):

        self.inputFileName = inputFileName
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

        self.Template      = self.TempFile.Get("h_mzzD")
        self.Template_Up   = self.TempFile.Get("h_mzzD_up")
        self.Template_Down = self.TempFile.Get("h_mzzD_dn")

        if (self.Template == None):
            print "mela2Dtemplate::loadTemplate - ERROR"
            print self.inputFileName, "h_mzzD"

        if (self.Template_Up == None):
            print "mela2Dtemplate::loadTemplate - ERROR"
            print self.inputFileName, "h_mzzD_dn"
        if (self.Template_Down == None):
            print "mela2Dtemplate::loadTemplate - ERROR"
            print self.inputFileName, "h_mzzD_up"
            
    def initRooDataHist(self):
    
        TemplateName = "TempDataHist_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template)
        TemplateName = "TempDataHist_Up_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_Up = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_Up)
        TemplateName = "TempDataHist_Down_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TempDataHist_Down = ROOT.RooDataHist(TemplateName,TemplateName,ROOT.RooArgList(self.m4lVar,self.discVar),self.Template_Down)

    def initRooHistPdf(self):
        
        TemplateName = "TemplatePdf_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist)
        TemplateName = "TemplatePdf_Up_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_Up = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_Up)
        TemplateName = "TemplatePdf_Down_{2}_{0:.0f}_{1:.0f}".format(self.channel,self.sqrts,self.tag)
        self.TemplatePdf_Down = ROOT.RooHistPdf(TemplateName,TemplateName,ROOT.RooArgSet(self.m4lVar,self.discVar),self.TempDataHist_Down)
        
    def initFastVertMorph(self):

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
        self.Pdf = ROOT.FastVerticalInterpHistPdf2D(TemplateName,TemplateName,self.m4lVar,self.discVar,True,self.funcList,self.morphVarList,1.0,1)
        
