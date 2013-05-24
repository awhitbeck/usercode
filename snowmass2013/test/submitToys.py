import os, os.path
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-t", "--toys", dest="nToys",
                  default=1,type="int",
                  help="number of toys to run")

parser.add_option("-n", "--jobs", dest="nJobs",
                  default=1,type="int",
                  help="number of jobs to run")

parser.add_option("-d", "--dryRun", dest="submit",
                  default=False,action="store_true",
                  help="Does not submit toys to batch system")

parser.add_option("-p", "--pureToys", dest="pureToys",
                  default=False,action="store_true",
                  help="Throw pure toys to be fit or draw embedded toys")

(options, args) = parser.parse_args()

print "=============================="
print "Submitting ", options.nJobs," jobs"
print "Submitting ", options.nToys," per job"
print "dry run? ", options.submit
print "pure toys? ", options.pureToys
print "=============================="

if not options.submit :

    for i in range(0,options.nJobs):

        if options.pureToys:
            os.system('qsub submitPBSJobTemplate.pbs -v "pureToys=true,ntoysperjob=%i,seed_index=%i"'%(options.nToys,i))
        else :
            os.system('qsub submitPBSJobTemplate.pbs -v "pureToys=false,ntoysperjob=%i,seed_index=%i"'%(options.nToys,i))
