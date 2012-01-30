hep.pha.jhu.edu:/scratch/hep/ntran/HZZ_materials

to setup root: source /scratch/hep/ntran/ROOTdistributions/myROOT528_fromSource/root/bin/thisroot.csh


plotAngularDistributions
----------------------------
To plot variables:
plotAngularDistributions_signal() -> for signal
plotVariablesZZ_background() -> for background


build1DNASAModel
----------------------
To build the D variables (LD from 8D PDF template)

build8DTemplate() -> to build background template
plotmy8DTemplate() -> to plot it
my8DTemplate -> to evaluate it
plotPBackground() -> to plot the PDFs from the template


likelihoodDiscriminant() -> to evaluate D = Psig/(Psig+Pbkg)
LDDistributionSignal() -> running on signal and filling histos with D in mZZ slices
LDDistributionBackground() -> running on background and filling histos with D in mZZ slices
plotLDDistribution() -> to plot LD in mZZ slices

storeLDDistribution -> to store the D variables in mZZ slices in a file
storeLDInfo -> to store D value event by event in the trees
(addDtoTree should do the same...)

separationLikelihoodDiscriminant() -> to evaluate LD for 2 signals PS vs SM

To run the full analysis you sould do
build8DTemplate()
storeLDDistribution(1) //for signal
storeLDDistribution(0) //for background
storeLDInfo(1) //for signla
storeLDInfo(0) // for background