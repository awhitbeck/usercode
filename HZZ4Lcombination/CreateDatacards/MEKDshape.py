

class MEKDshape:

    """
    Standardize class for initializing MEKD shape
    """

    def __init__(self,theInputs,channel,sqrts,MEKD,CMS_zz4l_mass,isSignal,tag):

        self.sqrts  =sqrts
        self.channel=channel

        if(isSignal):
            self.initSignal(theInputs,MEKD,CMS_zz4l_mass,tag)
        else:
            self.initBackground(theInputs,MEKD,CMS_zz4l_mass,tag)

    def initSignal(self,theInputs,MEKD,CMS_zz4l_mass,tag):

        name = "mekd_{2}_a0_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        self.sig_a0 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_sig_a0_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_{2}_a1_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        self.sig_a1 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_sig_a1_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_{2}_a2_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        self.sig_a2 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_sig_a2_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_{2}_a3_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        self.sig_a3 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_sig_a3_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_{2}_a4_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        self.sig_a4 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_sig_a4_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        self.Pdf = ROOT.RooGenericPdf("mekd_{0}".format(tag), "mekd_{0}".format(tag), "@3*exp((-(@0-@1)^2)/(2*@2^2))/@2+(1-@3)*exp((-(@0-@4)^2)/(2*@5^2))/@5", ROOT.RooArgList(MEKD,self.sig_a0, self.sig_a1, self.sig_a2, self.sig_a3, selfsig_a4))

        print "\n \n mekd_sig_a2 channel ",self.channel
        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.sig_a2.getVal() < 0 : print m, self.sig_a2.getVal()
            if self.sig_a2.getVal() > 1 : print m, selfsig_a2.getVal()
        print "\n \n mekd_sig_a1 channel ",self.channel
        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.sig_a1.getVal() <= 0 : print m, self.sig_a1.getVal()
        print "\n \n mekd_sig_a4 channel ",self.channel
        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.sig_a4.getVal() <= 0 : print m, self.sig_a4.getVal() 

    def initBackground(self,theInputs,MEKD,tag):

        name = "mekd_{2}_a0_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts,tag)
        print "mekd_qqZZ_a0_shape=",theInputs['mekd_qqZZ_a0_shape']
        print "mekd_sig_a0_shape=",theInputs['mekd_sig_a0_shape']
        self.bkg_a0 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_qqZZ_a0_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_qqZZ_a1_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts)
        self.bkg_a1 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_qqZZ_a1_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_qqZZ_a2_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts)
        self.bkg_a2 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_qqZZ_a2_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_qqZZ_a3_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts)
        self.bkg_a3 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_qqZZ_a3_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))
        name = "mekd_qqZZ_a4_{0:.0f}_{1:.0f}_centralValue".format(self.channel,self.sqrts)
        self.bkg_a4 = ROOT.RooFormulaVar(name,"("+theInputs['mekd_qqZZ_a4_shape']+")", ROOT.RooArgList(CMS_zz4l_mass))

        self.Pdf = ROOT.RooGenericPdf("mekd_qqZZ", "mekd_qqZZ", "@3*exp((-(@0-@1)^2)/(2*@2^2))/@2+(1-@3)*exp((-(@0-@4)^2)/(2*@5^2))/@5", ROOT.RooArgList(MEKD,mekd_qqZZ_a0, mekd_qqZZ_a1, mekd_qqZZ_a2, mekd_qqZZ_a3, mekd_qqZZ_a4))

        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.bkg_a2.getVal() < 0 : print m, self.bkg_a2.getVal()
            if self.bkg_a2.getVal() > 1 : print m, self.bkg_a2.getVal()
        print "\n \n mekd_qqZZ_a1 channel ",self.channel
        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.bkg_a1.getVal() <= 0 : print m, self.bkg_a1.getVal()
        print "\n \n mekd_qqZZ_a4 channel ",self.channel
        m = 100
        while m >= 100 and m < 150:
            CMS_zz4l_mass.setVal(m)
            m = m + 0.1
            if self.bkg_a4.getVal() <= 0 : print m, self.bkg_qqZZ_a4.getVal() 
        
