#ifndef ePaxAnalyzer_H
#define ePaxAnalyzer_H
//
// class decleration
//

// CMSSW includes
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
 
// ROOT stuff

#include "TFile.h"
#include "TTree.h"
#include "TMatrixT.h"

// ePax stuff
// Has to be included as the last header otherwise there will be a warning concerning the 
// zlib. According to Steffen there are two different zlib and ROOT can only deal with one of them
// but ePax can deal with both of them
#include "ePaxPxl/ePax/interface/ePax.h"
#include "ePaxDemo/ePaxAnalyzer/interface/ParticleMatcher.hh"

class ePaxAnalyzer : public edm::EDAnalyzer {
public:

   // why explicit?
   explicit ePaxAnalyzer(const edm::ParameterSet&);
   ~ePaxAnalyzer();
  

private:

   virtual void beginJob(const edm::EventSetup&);
   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   virtual void endJob();
   virtual void analyzeGenInfo(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeGenJets(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeGenMET(const edm::Event&, pxl::EventViewRef);
   
   virtual void analyzeTrigger(const edm::Event&, pxl::EventViewRef);
   virtual void saveHLTobjects(const edm::Event&, pxl::EventViewRef, string&);
   virtual void analyzeRecVertices(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeRecMuons(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeRecElectrons(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeRecJets(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeRecMET(const edm::Event&, pxl::EventViewRef);
   virtual void analyzeRecGammas(const edm::Event&, pxl::EventViewRef);

   bool MuonMC_cuts(const GenParticleCandidate* MCmuon) const;
   bool EleMC_cuts(const GenParticleCandidate* MCele) const;
   bool GammaMC_cuts(const GenParticleCandidate* MCgamma) const;
   bool JetMC_cuts(reco::GenJetCollection::const_iterator MCjet) const;
   bool METMC_cuts(const pxl::ParticleRef MCmet) const;
   bool Vertex_cuts(reco::VertexCollection::const_iterator vertex) const; 
   bool Muon_cuts(reco::MuonCollection::const_iterator muon) const;
   bool Ele_cuts(SiStripElectronCollection::const_iterator ele) const;
   bool Ele_cuts(PixelMatchGsfElectronCollection::const_iterator ele) const;
   bool Gamma_cuts(reco::PhotonCollection::const_iterator photon) const;
   bool Jet_cuts(reco::CaloJetCollection::const_iterator jet) const;
   bool MET_cuts(const pxl::ParticleRef met) const;
   std::string getEventClass(pxl::EventViewRef EvtView);
   // TEMPORARY STUFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   double IsoCalSum(const edm::Event& iEvent, double ParticleCalPt, double ParticleCalEta, double ParticleCalPhi, double iso_DR, double iso_Seed);
   double IsoTrkSum(const edm::Event& iEvent, double ParticleTrkPt, double ParticleTrkEta, double ParticleTrkPhi, double iso_DR, double iso_Seed);
   double IsoGenSum (const edm::Event& iEvent, double ParticleGenPt, double ParticleGenEta, double ParticleGenPhi, double iso_DR, double iso_Seed);

   void catchParticlesWithStatus3Daughters(std::vector<const reco::Candidate*>& cand, const reco::Candidate* p);   
   /*void matchObjects(pxl::EventViewRef GenView, pxl::EventViewRef RecView);
   void makeMatching(pxl::ParticleFilter& GenFilter, pxl::ParticleFilter& RecFilter);
   int SmallestColumnElement(TMatrixT<double>* matrix, int col);
   int SmallestRowElement(TMatrixT<double>* matrix, int col);
*/

   // ----------member data ---------------------------

   int fNumEvt;// used to count the number of events
   int fDebug; 
   std::string fFileName; 
   std::string fProcess;
   bool fGenOnly;
   // The labels used in cfg-file 
   bool fIsCSASoup;
   std::string fTruthVertexLabel;
   std::string fgenParticleCandidatesLabel;
   std::string fKtJetMCLabel;
   std::string fItCone5JetMCLabel; 
   std::string fMETMCLabel;
   std::string fVertexRecoLabel;
   std::string fMuonRecoLabel;
   std::string fSAMuonRecoLabel;
   std::string fElectronRecoLabel;
   std::string fPixelMatchElectronRecoLabel;
   std::string fElectronIDAssocProducer;
   std::string fElectronHcalIsolationProducer;
   std::string fElectronEcalIsolationProducer;
   std::string fElectronTrackIsolationProducer;
   std::string fElectronTrackNumProducer;
   std::string fGammaRecoLabel;
   std::string fGammaHcalIsolationProducer;
   std::string fGammaEcalIsolationProducer;
   std::string fGammaTrackIsolationProducer;
   std::string fGammaTrackNumProducer;
   std::string fKtJetRecoLabel;
   std::string fItCone5JetRecoLabel;
   std::string fL2L3JESic5JetRecoLabel;
   std::string fMETRecoLabel;
   std::string fMETCorrRecoLabel;
   edm::InputTag fBarrelClusterShapeAssocProducer;
   edm::InputTag fEndcapClusterShapeAssocProducer;
   std::string fHBHELabel;
   std::string fHBHEInstanceName;
   
   edm::ESHandle<CaloGeometry>  theCaloGeom;
   
   ParticleMatcher* Matcher;

  
   // to be used for ePax output 
   pxl::oDiskFile fePaxFile;

};
#endif
