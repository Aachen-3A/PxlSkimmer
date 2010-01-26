runOnData = True
#run on Summer09 (ReReco'd or not)
runOnSummer09 = False
#run on ReReco'ed data or Summer09 MC
runOnReReco = False

if runOnData and runOnSummer09:
    print "runOnData and runOnSummer09 can't be true at the same time!"
    import sys
    sys.exit(1)

if runOnSummer09 and not runOnReReco:
    print 'Reco of CMSSW < 3.5.X is not supported anymore!'
    import sys
    sys.exit(1)


import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.limit = 100

import FWCore.Framework.test.cmsExceptionsFatalOption_cff
process.options   = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    #open file in NOMERGE mode to avoid a memory leak
    fileMode = cms.untracked.string( 'NOMERGE' ),
    #stop processing on each and every thrown exception
    Rethrow = FWCore.Framework.test.cmsExceptionsFatalOption_cff.Rethrow
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# source
process.source = cms.Source("PoolSource", 
     skipEvents = cms.untracked.uint32(0),
     fileNames = cms.untracked.vstring(
'/store/mc/Summer10/MinBias_TuneD6T_7TeV-pythia6/GEN-SIM-RECODEBUG/START36_V10_SP10-v1/0012/1CCD3461-D678-DF11-879E-001A4BA910A0.root'
	)
)


process.load("Configuration/StandardSequences/GeometryPilot2_cff")

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
if runOnData:
    if runOnReReco:
        process.GlobalTag.globaltag = cms.string('GR_R_37X_V6::All')
    else:
        process.GlobalTag.globaltag = cms.string('GR_R_37X_V6::All')
else:
    process.GlobalTag.globaltag = cms.string('START37_V5::All')

process.load("Configuration/StandardSequences/MagneticField_38T_cff")

# PAT Layer 0+1
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.content = cms.EDAnalyzer("EventContentAnalyzer")


import MUSiCProject.Skimming.Tools
MUSiCProject.Skimming.Tools.configurePAT( process, runOnData, runOnReReco, runOnSummer09 )
process.metJESCorAK5CaloJet.inputUncorMetLabel = 'metNoHF'


#filter on right BX in case of data
if runOnData:
    process.primaryVertexFilter = cms.EDFilter( "VertexSelector",
                                                src = cms.InputTag( "offlinePrimaryVertices" ),
                                                cut = cms.string( "!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2" ),
                                                filter = cms.bool( True ), # otherwise it won't filter the events, just produce an empty vertex collection.
                                                )                            

    process.scrapingFilter = cms.EDFilter( "FilterOutScraping",
                                           applyfilter = cms.untracked.bool( True ),
                                           debugOn = cms.untracked.bool( False ),
                                           numtrack = cms.untracked.uint32( 10 ),
                                           thresh = cms.untracked.double( 0.25 )
                                           )


    process.incompleteECALFilter = cms.EDFilter( "recHitFilter" )

    process.p = cms.Path( process.primaryVertexFilter * process.scrapingFilter * process.incompleteECALFilter * process.patDefaultSequence )

else:
    if runOnSummer09:
        process.load("RecoJets.Configuration.GenJetParticles_cff")
        process.load("RecoJets.JetProducers.ak5GenJets_cfi")
        process.p = cms.Path( process.genParticlesForJets * process.ak5GenJets * process.patDefaultSequence )
    else:
        process.p = cms.Path( process.patDefaultSequence )


process.load( "MUSiCProject.Skimming.MUSiCSkimmer_cfi" )

if not runOnData and runOnReReco:
    #use the re-digi trigger in re-reco
    if runOnSummer09:
        process.Skimmer.triggers.HLT.process = 'REDIGI36X'
    else:
        process.Skimmer.triggers.HLT.process = 'REDIGI36'


if not runOnData:
    MUSiCProject.Skimming.Tools.addFlavourMatching( process, process.Skimmer, process.p )

process.p += process.Skimmer
