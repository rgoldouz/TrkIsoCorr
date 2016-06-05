#include <vector>
#include <functional>
#include <Math/VectorUtil.h>
#include "TrkIsoCorr/CorrectedElectronTrkisoProducers/interface/ElectronTkIsolationCorr.h"

using namespace ROOT::Math::VectorUtil ;

ElectronTkIsolationCorr::ElectronTkIsolationCorr (double extRadius,
                                          const reco::TrackCollection* trackCollection,
                                          const reco::BeamSpot *beamPoint):
  extRadius_(extRadius),
  trackCollection_(trackCollection),
  beamPoint_(beamPoint)
  {
   setAlgosToReject();
  }

ElectronTkIsolationCorr::~ElectronTkIsolationCorr ()
{}

void ElectronTkIsolationCorr::setAlgosToReject()
{
  algosToReject_={reco::TrackBase::jetCoreRegionalStep};
  std::sort(algosToReject_.begin(),algosToReject_.end());
}

bool ElectronTkIsolationCorr::passAlgo(const reco::TrackBase& trk)const
{
  int algo = trk.algo();
  bool rejAlgo=std::binary_search(algosToReject_.begin(),algosToReject_.end(),algo);
  return rejAlgo==false;
}

double ElectronTkIsolationCorr::correctedTrkIso(const reco::GsfElectron* electron) const
{
  double correctedTrkIso03 = electron->dr03TkSumPt();
  for ( reco::TrackCollection::const_iterator itrTr  = trackCollection_->begin() ;     itrTr != trackCollection_->end();        ++itrTr ) {
    if (passAlgo(*itrTr)) continue;
    if (itrTr->pt()<0.7) continue;
    double dzCut = fabs (itrTr->vz() - electron->gsfTrack()->vz());
    if (dzCut > 0.2 ) continue;
    if (fabs(itrTr->dxy(beamPoint_->position()) ) > 999999999 ) continue;
    double dr = ROOT::Math::VectorUtil::DeltaR(itrTr->momentum(),electron->gsfTrack()->momentum ()) ;
    double deta = itrTr->eta() - electron->gsfTrack()->eta();
    if(dr < extRadius_ && dr>=0.015 && std::abs(deta) >=0.015 ) correctedTrkIso03 -= itrTr->pt();
  }
  return (correctedTrkIso03<=0) ? 0 : correctedTrkIso03;
}

double ElectronTkIsolationCorr::getCorrectedTrkIso(const reco::GsfElectron* electron) const
{
return correctedTrkIso(&(*(electron)));
}
