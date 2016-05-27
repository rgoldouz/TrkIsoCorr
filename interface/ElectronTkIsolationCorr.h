#ifndef TrkIsoCorr_ElectronTkIsolationCorr_h
#define TrkIsoCorr_ElectronTkIsolationCorr_h

#include <vector>
#include <functional>

//Root includes
#include "TObjArray.h"
//
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTrackSelector.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"

#include<string>

class ElectronTkIsolationCorr {
  public:
  ElectronTkIsolationCorr (double extRadius,
			   const reco::TrackCollection*,
			   const reco::BeamSpot*);

 ~ElectronTkIsolationCorr() ;
 double getCorrectedTrkIso (const reco::GsfElectron*) const;
 private:

  bool passAlgo(const reco::TrackBase&)const;
  void setAlgosToReject();
  double correctedTrkIso(const reco::GsfElectron*) const;
  double extRadius_ ;

  std::vector<int> algosToReject_; //vector is sorted
  const reco::TrackCollection *trackCollection_ ;
  const reco::BeamSpot *beamPoint_;
};

#endif



