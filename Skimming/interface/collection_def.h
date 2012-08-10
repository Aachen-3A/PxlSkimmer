#ifndef collection_def_H
#define collection_def_H

namespace pat {
	class Jet;
}

template< class T > class Selector;

#include <string>
#include <vector>

#include "FWCore/Utilities/interface/InputTag.h"

//holds everything used to analyze a collection
struct collection_def{
   std::string   name;
   edm::InputTag MCLabel;
   edm::InputTag RecoLabel;
};

struct jet_def{
   std::string   name;
   edm::InputTag MCLabel;
   edm::InputTag RecoLabel;
   bool          isPF;
   std::vector< std::pair< std::string, Selector<pat::Jet>* > >   IDs;
};
#endif
