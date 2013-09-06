This file documents the technical details of running the ee->ZH analysis. 
For each of the scripts mentioned in the following, please read the top 
part to get the instrcutions on the file setup and brief instructions.  (YY)


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
- The nominal analysis uses 100 bins in the D0-
- It can be configured to run 1D analysis based on 250 GeV KDInt
- The default code runs 1 embeded toys for the f3 = 0.1 with bkg, Here is the expected output

root -l toyresults_eezh/toyresults_pseudoMELA_embd_250GeV_accfalse_new.root
root [3] fittree->Scan("fa3:fa3_err:fa3_pull")
************************************************
*    Row   *       fa3 *   fa3_err *  fa3_pull *
************************************************
*        0 * 0.1441990 * 0.0445728 * 0.9916136 *
************************************************


--------------------------------
Running the 2D analysis based on D_{0-} and Dint
--------------------------------
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/testfitkd_eezh.C

- The current code runs one puretoy for f3 = 0.1 (without bkg), here is an example output

root -l -b toyresults_eezh/toyresults_KDInt_pure_250GeV_accfalse_f3p1_sigonly_test.root 
root [1] fittree->Scan("fa3:fa3_err:fa3_pull")
************************************************
*    Row   *       fa3 *   fa3_err *  fa3_pull *
************************************************
*        0 * 0.1050534 * 0.0521391 * 0.0969218 *
************************************************


--------------------------------
Running the 3D fit results 
--------------------------------
https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/testfitilc.C


- The default code performs one embeded toy for f3 = 0.1 with bkg, output is like this,

root -l -b  toyresults_eezh/toyresults_embd_accfalse_487563.root
root [2] fittree->Scan("fa3:fa3_err:fa3_pull")
************************************************
*    Row   *       fa3 *   fa3_err *  fa3_pull *
************************************************
*        0 * 0.1258980 * 0.0341362 * 0.7586675 *
************************************************

- For run many jobs, you need to use the cluster

https://github.com/awhitbeck/usercode/blob/master/snowmass2013/test/submitilctoy.sh
