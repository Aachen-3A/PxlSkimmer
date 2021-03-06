## MC simulation skimming config
#
# This config is meant to be used for MC files. This means that the sample is
# not a data sample and usually means that it contains more than just the GEN
# information. Thus both runOnData and runOnGen are False by default.
#
# The fileNames variable contains the explicit file names for local testing. One
# can either choose locally stored files or those that are kept on the CERN
# tiers.

# Run on data
runOnData = False
# Run on GEN sample
runOnGen = False

import FWCore.ParameterSet.Config as cms
import sys
# Choose the type of effective area correction you want to use.
# Possible values:
#     NoCorr
#     Data2011
#     Data2012
#     Summer11MC
#     Fall11MC
eleEffAreaTarget = cms.untracked.string( 'Fall11MC' )

# Verbosity: 0 = normal messaging, 1 = human readable, 2 = insane, 3 = INFO from all modules
verbosityLvl = 0

if runOnGen and runOnData :
    print "runOnData and runOnGen can't be true at the same time!"
    import sys
    sys.exit( 1 )

import PxlSkimmer.Skimming.Tools_miniAOD as Tools_miniAOD

print sys.argv
name="test"
datasetpath="dummy"
globalTag="74X_mcRun2_asymptotic_realisticBS_v1"
for option in sys.argv:
    splitoption=option.split('=')
    if "name" in option and len(splitoption) > 1:
        name = splitoption[1]
    if "datasetpath" in option and len(splitoption) > 1:
        datasetpath = splitoption[1]
    if "globalTag" in option and len(splitoption) > 1:
        globalTag = splitoption[1]

print "music_crab3 name %s"%name
print "music_crab3 datasetpath %s"%datasetpath
print "music_crab3 globalTag %s"%globalTag

process = Tools_miniAOD.prepare( runOnGen, runOnData, eleEffAreaTarget, name, datasetpath, globalTag, verbosityLvl )

# source
process.source = cms.Source(
    'PoolSource',
    skipEvents = cms.untracked.uint32( 0 ),
    fileNames = cms.untracked.vstring(
        # 'file://3056AD52-BAFB-E411-A8E7-0025905A612C.root'
        '/store/mc/RunIISpring15MiniAODv2/ZToMuMu_NNPDF30_13TeV-powheg_M_1400_2300/MINIAODSIM/74X_mcRun2_asymptotic_v2-v1/30000/EA2F4FC8-5771-E511-B1C4-6CC2173D9AB0.root'
        )
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32( -1 ) )
###FIXME for miniAOD!!!
#if not runOnGen:
    #if not runOnData:
        # This is used for QCD samples only, and the filters only write a flag in the
        # event if they fired or not. The actual selection must happen in the
        # classification, i.e. you have to set in the config file which flag you want to
        # consider.
        #
        # remove events with electrons that come from b or c hadrons
        #Tools_miniAOD.addBCtoEFilter( process )

        # remove events containing b quarks
        #Tools_miniAOD.addBFilter( process )

        # remove events with at least one potential electron candidate
        #Tools_miniAOD.addEMFilter( process )

        # remove events with a muon of more than 5 GeV
        #Tools_miniAOD.addMuGenFilter( process, pt = 5 )

        # remove events with a muon of more than 10 GeV
        #Tools_miniAOD.addMuGenFilter( process, pt = 10 )

        # remove events with a muon of more than 15 GeV
        #Tools_miniAOD.addMuGenFilter( process, pt = 15 )


print 'INFO: Using global tag:', process.GlobalTag.globaltag
