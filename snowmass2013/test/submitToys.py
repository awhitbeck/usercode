import os, os.path
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-t", "--toys", dest="nToys",
                  default=1,
                  help="number of toys to run")

parser.add_option("-n", "--jobs", dest="nJobs",
                  default=1,
                  help="number of jobs to run")

parser.add_option("-d", "--dryRun", dest="submit",
                  default=False,
                  help="Does not submit toys to batch system")

parser.add_option("-p", "--pureToys", dest="pureToys",
                  default=True,
                  help="Throw pure toys to be fit or draw embedded toys")

(options, args) = parser.parse_args()

print "Submitting ", options.nJobs," jobs"
print "Submitting ", options.nToys," per job"

for i in range(0,int(options.nJobs)):

    if options.submit:
        os.system('qsub submitPBSJobTemplate.pbs -v "pureToys=%s,ntoysperjob=%i,seed_index=%i"'%(options.pureToys,int(options.nToys),i))
