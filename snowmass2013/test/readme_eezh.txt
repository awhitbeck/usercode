--------------------------------
Preparation the files with KD
--------------------------------

1. Existing files

I believe all the 250 GeV files are done in 
/export/d1/scratch/ygao/samples/ee_ZH/

2. The files above all contained the precalcualted KD and KDInt

The script to make those are here,
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/addKD_eezh.sh

- The files under 250 GeV analysis are listed in Line 18-27
- The .cc file for adding the KD are in 
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/src/KDcalcZH.cc


--------------------------------
Running the 1D analysis based on D_{0-}
--------------------------------

The script is this one,
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/testfitilckd.C

- This code uses the  new TH1F based PDF, not on the HistPdf for the signal. 
- The nominal analysis uses 100 bins in the D0-. 

--------------------------------
Running the 2D analysis based on D_{0-} and Dbkg
--------------------------------
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/testfitkd_eezh.C


--------------------------------
Running the 3D fit results 
--------------------------------
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/testfitilc.C

This one you need the cluster to run many jobs..
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/submitilctoy.sh