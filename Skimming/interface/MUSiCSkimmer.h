#ifndef MUSiCSkimmer_H
#define MUSiCSkimmer_H

// LHAPDF stuff
extern "C" {
   void initpdfset_ (char *, int len);
   void initpdfsetm_(int &, char *);
   void initpdf_(int &);
   void evolvepdf_(double &, double &, double *);
   void numberpdf_(int &);
}


//standard library
#include <map>


// CMSSW includes
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//BeamSpot
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/SiStripElectron.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "SimDataFormats/GeneratorProducts/interface/PdfInfo.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

 
// ROOT stuff

#include "TFile.h"
#include "TTree.h"
#include "TMatrixT.h"

//PAT related stuff
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

//JetID
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/SelectorUtils/interface/Selector.h"
#include "PhysicsTools/SelectorUtils/interface/JetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"

//for ClusterShape variables
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

// ePax stuff
// Has to be included as the last header otherwise there will be a warning concerning the 
// zlib. According to Steffen there are two different zlib and ROOT can only deal with one of them
// but ePax can deal with both of them
#include "MUSiCProject/Pxl/interface/Pxl.h"
#include "MUSiCProject/Skimming/interface/ParticleMatcher.hh"
#include "MUSiCProject/Skimming/interface/collection_def.h"


class MUSiCSkimmer : public edm::EDAnalyzer {
public:

   // why explicit?
   explicit MUSiCSkimmer(const edm::ParameterSet&);
   ~MUSiCSkimmer();
  

 private:
   //information about one single trigger
   struct trigger_def {
      std::string name;
      unsigned int ID;
      bool active;
   };
   //information about one trigger group
   struct trigger_group {
      std::string   name;
      std::string   process;
      edm::InputTag L1_result;
      edm::InputTag results;
      edm::InputTag event;
      HLTConfigProvider config;
      std::vector< std::string > triggers_names;
      std::vector< trigger_def > trigger_infos;
   };


   virtual void analyze( const edm::Event &iEvent, const edm::EventSetup &iSetup );
   virtual void endJob();
   virtual void analyzeGenInfo( const edm::Event &iEvent, pxl::EventView *EvtView, std::map< const reco::Candidate*, pxl::Particle* > &genmap );
   virtual void analyzeGenRelatedInfo(const edm::Event&, pxl::EventView*);
   virtual void analyzeGenJets( const edm::Event &iEvent, pxl::EventView *GenEvtView, std::map< const reco::Candidate*, pxl::Particle* > &genjetmap, const jet_def &jet_info );
   virtual void analyzeGenMET(const edm::Event&, pxl::EventView*, const collection_def &MET_info );

   virtual void analyzeSIM(const edm::Event&, pxl::EventView*);
   
   virtual void initializeTrigger( const edm::Event &event,
                                   const edm::EventSetup &setup,
                                   trigger_group &trigger,
                                   const std::string &process
                                   );

   virtual void analyzeFilter( const edm::Event &iEvent,
                               const edm::EventSetup &iSetup,
                               pxl::EventView *EvtView,
                               trigger_group &filter
                               );

   virtual void analyzeTrigger( const edm::Event &iEvent,
                                const edm::EventSetup &iSetup,
                                pxl::EventView *EvtView,
                                trigger_group &trigger
                                );
   virtual void analyzeRecVertices(const edm::Event&, pxl::EventView*);
   virtual void analyzeRecTaus( const edm::Event &iEvent, pxl::EventView *RecView, const bool &MC, std::map< const reco::Candidate*, pxl::Particle*> &genmap );
   virtual void analyzeRecMuons( const edm::Event &iEvent, pxl::EventView *RecView, const bool &MC, std::map< const reco::Candidate*, pxl::Particle* > &genmap );
   virtual void analyzeRecElectrons( const edm::Event &iEvent,
                                     pxl::EventView *RecView,
                                     const bool &MC,
                                     EcalClusterLazyTools &lazyTools,
                                     std::map< const reco::Candidate*, pxl::Particle* > &genmap,
                                     const edm::ESHandle< CaloGeometry > &geo
                                     );
   virtual void analyzeRecJets( const edm::Event &iEvent, pxl::EventView *RecView, bool &MC, std::map< const reco::Candidate*, pxl::Particle* > &genjetmap, const jet_def &jet_info );
   virtual void analyzeRecMET(const edm::Event&, pxl::EventView*, const collection_def &MET_info);
   virtual void analyzeRecGammas( const edm::Event &iEvent,
                                  pxl::EventView *RecView,
                                  const bool &MC,
                                  EcalClusterLazyTools &lazyTools,
                                  std::map< const reco::Candidate*, pxl::Particle* > &genmap,
                                  const edm::ESHandle< CaloGeometry > &geo
                                  );
   virtual void analyzeHCALNoise(const edm::Event&, pxl::EventView*);

   bool TauMC_cuts( const reco::GenParticle *MCtau ) const;
   bool MuonMC_cuts( const reco::GenParticle* MCmuon ) const;
   bool EleMC_cuts( const reco::GenParticle* MCele ) const;
   bool GammaMC_cuts( const reco::GenParticle* MCgamma ) const;
   bool JetMC_cuts(reco::GenJetCollection::const_iterator MCjet) const;
   bool METMC_cuts(const pxl::Particle* MCmet) const;
   bool Vertex_cuts(reco::VertexCollection::const_iterator vertex) const;
   bool PV_vertex_cuts( const reco::Vertex &vertex) const;
   bool Tau_cuts (const pat::Tau &tau) const;
   bool Muon_cuts(const pat::Muon& muon) const;
   bool Ele_cuts(std::vector<pat::Electron>::const_iterator ele) const;
   bool Gamma_cuts(std::vector<pat::Photon>::const_iterator photon) const;
   bool Jet_cuts(std::vector<pat::Jet>::const_iterator jet) const;
   bool MET_cuts(const pxl::Particle* met) const;
   std::string getEventClass(pxl::EventView* EvtView);

   double IsoGenSum (const edm::Event& iEvent, double ParticleGenPt, double ParticleGenEta, double ParticleGenPhi, double iso_DR, double iso_Seed);

   // ----------member data ---------------------------

   int fNumEvt;// used to count the number of events
   int fDebug; 
   std::string fFileName; 
   std::string fProcess;
   bool fGenOnly;
   bool fUseSIM;
   std::string fLHgridName;
   int fNumLHgridErrorSets;
   // The labels used in cfg-file 
   // Generator 
   std::string fgenParticleCandidatesLabel;
   std::string fMETMCLabel;
   double m_rhoFastJet;
   std::string fVertexRecoLabel;
   //Tau
   std::string fTauRecoLabel;
   std::string fPFTauDiscriminator;
   // Muon
   std::string fMuonRecoLabel;
   // Electron
   std::string fElectronRecoLabel;
   // Photon
   std::string fGammaRecoLabel;
   // Jets
   std::vector< jet_def > jet_infos;
   //JetIDs
   typedef std::vector< std::pair< std::string, Selector<pat::Jet>* > > jet_id_list;
   // MET labels
   std::vector< collection_def > MET_infos;
   // Cluster
   edm::InputTag freducedBarrelRecHitCollection;
   edm::InputTag freducedEndcapRecHitCollection;
   //HCAL noise
   edm::InputTag hcal_noise_label;

   //all triggers
   std::vector< trigger_group > triggers;
   std::vector< trigger_group > filters;

   bool fStoreL3Objects;
 
   ParticleMatcher* Matcher;
   // to be used for ePax output 
   pxl::OutputFile fePaxFile;
   std::vector<gen::PdfInfo> fpdf_vec;
   double xfx(const double &x, const double &Q, int fl) {  
      double f[13], mx = x, mQ = Q;
      evolvepdf_(mx, mQ, f);
      return f[fl+6];
   };

   //cuts
   double min_muon_pt,
      min_ele_pt,
      min_gamma_pt,
      min_jet_pt,
      min_met,
      min_tau_pt,
      max_eta,
      min_rechit_energy,
      min_rechit_swiss_cross,
      min_rechit_R19,
      vertex_minNDOF,
      vertex_maxZ,
      vertex_maxR,
      PV_minNDOF,
      PV_maxZ,
      PV_maxR;

   //vertex for physics eta, phi, pt
   reco::BeamSpot::Point the_vertex;
   reco::BeamSpot::Point the_beamspot;
};
#endif
