
import os
from ROOT import *
import ROOT
from superMela2Dtemplate import *
from mela2Dtemplate import *

def writeCards(channel, sqrts, observation, signalExpectation, backgroundExpectation):

    filename = "test_{0}_{1}TeV.txt".format(channel,sqrts)
    f = open(filename,"w")

    f.write("imax 1 \n")
    f.write("jmax * \n")
    f.write("kmax * \n")
    f.write("------------ \n")
    f.write("shapes * * test_{0}_{1}TeV.root w:$PROCESS \n".format(channel,sqrts))
    f.write("bin a1 \n")
    f.write("observation {0} \n".format(observation))
    f.write("------------ \n")
    f.write("## mass window [105.0,140.0] \n")
    f.write("bin a1 a1 \n")
    f.write("process ggH qqzz \n")
    f.write("process 0   1   \n")
    f.write("rate    {0} {1} \n".format(signalExpectation,backgroundExpectation))
    f.write("------------ \n")

    f.close()

def writeWorkspace(channel, sqrts, observation, useSuperMela):

    filename = "test_{0}_{1}TeV.root".format(channel, sqrts)
    outputFile = ROOT.TFile(filename,"RECREATE")

    w = ROOT.RooWorkspace("w","w")

    if useSuperMela:
        SMD = ROOT.RooRealVar("SMD","SMD",.5,0,1);
    else: 
        mass = ROOT.RooRealVar("mass","mass",121,100,2000);

    KD = ROOT.RooRealVar("KD","KD",.5,0,1);

    if channel=="4mu"                       : chan=1
    if channel=="4e"                        : chan=2
    if channel=="2e2mu" or channel=="2mu2e" : chan=3

    if useSuperMela:
        filename = "templates2D_smd_{1}TeV_20121023b_GOODvarbin/Dsignal_{0}.root".format(channel,sqrts)    
        ggHtemplate = superMela2Dtemplate(filename,KD,SMD,chan,sqrts,"ggH",False,"sig")
    else:
        filename = "templates2D/Dsignal_{0}.root".format(channel,sqrts)    
        ggHtemplate = mela2Dtemplate(filename,KD,mass,chan,sqrts,"ggH",False,"sig")

    if useSuperMela:
        filename = "templates2D_smd_{1}TeV_20121023b_GOODvarbin/Dbackground_qqZZ_{0}.root".format(channel,sqrts)
        qqZZtemplate = superMela2Dtemplate(filename,KD,SMD,chan,sqrts,"qqZZ",False,"bkg")

    else:
        filename = "templates2D/Dbackground_qqZZ_{0}.root".format(channel,sqrts)
        qqZZtemplate = mela2Dtemplate(filename,KD,mass,chan,sqrts,"qqZZ",False,"bkg")

    ggHtemplate.Pdf.SetNameTitle("ggH","ggH")
    getattr(w,'import')(ggHtemplate.Pdf)
    qqZZtemplate.Pdf.SetNameTitle("qqzz","qqzz")
    getattr(w,'import')(qqZZtemplate.Pdf)

    ## ------ generate fake dataset -------

    if useSuperMela:
        data = qqZZtemplate.Pdf.generate(ROOT.RooArgSet(KD,SMD),observation)
    else:
        data = qqZZtemplate.Pdf.generate(ROOT.RooArgSet(KD,mass),observation)

    data.SetNameTitle("data_obs","data_obs")
    
    getattr(w,'import')(data)

    outputFile.cd()
    w.Write("w")
    outputFile.Close()
    
def testSuperMela(useSuperMela=True):

    ##  loading junk ------

    ROOT.gSystem.AddIncludePath("-I$ROOFITSYS/include/")
    ROOT.gSystem.AddIncludePath("-Iinclude/")
    ROOT.gROOT.ProcessLine(".L include/tdrstyle.cc")
    ROOT.gSystem.Load("libRooFit")
    ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit.so")

    ##  writing cards/workspaces -----
                                 
    writeCards("4mu",8,10,11,10)
    writeWorkspace("4mu",8,10,useSuperMela)

    writeCards("4mu",7,9,10,11)
    writeWorkspace("4mu",7,9,useSuperMela)

    ##  combining cards and testing limit -----

    os.system("combineCards.py test_4mu_7TeV.txt test_4mu_8TeV.txt > test1.txt")
    os.system("combineCards.py test_4mu_8TeV.txt test_4mu_7TeV.txt > test2.txt")
    os.system("combine -M ProfileLikelihood test1.txt -m 125 --signif -t -1 --expectSignal=1")
    os.system("combine -M ProfileLikelihood test2.txt -m 125 --signif -t -1 --expectSignal=1")
