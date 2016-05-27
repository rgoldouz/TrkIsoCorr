#ifndef TrkIsoCorr_CorrectedElectronTrkisoProducers
#define TrkIsoCorr_CorrectedElectronTrkisoProducers

#include <memory>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include <Math/VectorUtil.h>

// class declaration

class CorrectedElectronTrkisoProducers : public edm::stream::EDProducer<> {
   public:
      explicit CorrectedElectronTrkisoProducers(const edm::ParameterSet&);
      ~CorrectedElectronTrkisoProducers();
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginStream(edm::StreamID) override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endStream() override;
      edm::EDGetTokenT<reco::TrackCollection>       generalTracksToken_;
      edm::EDGetTokenT<reco::GsfElectronCollection> electronCollectionToken_;
      edm::EDGetTokenT<reco::BeamSpot> beamSpotToken_ ;
};

#endif
