//
// Original Author:  Reza Goldouzian
//         Created:  Tue, 10 May 2016 20:33:32 GMT

#include "TrkIsoCorr/CorrectedElectronTrkisoProducers/interface/ElectronTkIsolationCorr.h"
#include "TrkIsoCorr/CorrectedElectronTrkisoProducers/interface/CorrectedElectronTrkisoProducers.h"

CorrectedElectronTrkisoProducers::CorrectedElectronTrkisoProducers(const edm::ParameterSet& iConfig)
{
    electronCollectionToken_ =  consumes<reco::GsfElectronCollection> (iConfig.getParameter<edm::InputTag>("electronsLabel"));
    generalTracksToken_ = consumes<reco::TrackCollection> (iConfig.getParameter<edm::InputTag>("generalTracksLabel"));
    beamSpotToken_ = consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamSpotLabel")) ;
    produces<std::vector<reco::GsfElectron>>("");
}


CorrectedElectronTrkisoProducers::~CorrectedElectronTrkisoProducers()
{
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CorrectedElectronTrkisoProducers::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<reco::TrackCollection> generalTracksHandle;
  iEvent.getByToken(generalTracksToken_,generalTracksHandle);
  const reco::TrackCollection * TrackCollection = generalTracksHandle.product();

  edm::Handle<reco::GsfElectronCollection> electronHandle ;
  iEvent.getByToken( electronCollectionToken_ , electronHandle) ;
  const reco::GsfElectronCollection * GsfElectronCollection = electronHandle.product();

  edm::Handle<reco::BeamSpot> beamspotHandle_ ;
  iEvent.getByToken(beamSpotToken_, beamspotHandle_) ;

  std::auto_ptr<std::vector<reco::GsfElectron> > prod(new std::vector<reco::GsfElectron>());

  ElectronTkIsolationCorr EleTkIsolationCorr(0.3,TrackCollection,&(*beamspotHandle_));

  for (reco::GsfElectronCollection::const_iterator gsfiter = GsfElectronCollection->begin(); gsfiter != GsfElectronCollection->end(); ++gsfiter) {	

  reco::GsfElectron CorrectedEle(*gsfiter);
/*
  double CorrectedTrkIso03 = gsfiter->dr03TkSumPt();
  for ( reco::TrackCollection::const_iterator itrTr  = TrackCollection->begin() ;     itrTr != TrackCollection->end();        ++itrTr ) {
    if (!(std::binary_search(algosToReject_.begin(),algosToReject_.end(),itrTr->algo()))) continue;
    if (itrTr->pt()<0.7) continue;
    double dzCut = fabs (itrTr->vz() - gsfiter->gsfTrack()->vz());
    if (dzCut > 0.2 ) continue;
    if (fabs(itrTr->dxy(beamspotHandle_->position()) ) > 999999999 ) continue;
    double dr = ROOT::Math::VectorUtil::DeltaR(itrTr->momentum(),gsfiter->gsfTrack()->momentum ()) ;
    double deta = itrTr->eta() - gsfiter->gsfTrack()->eta();
    if(dr < 0.3 && dr>=0.015 && std::abs(deta) >=0.015 ) CorrectedTrkIso03 -= itrTr->pt();
  }
*/
//  CorrectedTrkIso03 = (CorrectedTrkIso03<=0) ? 0 : CorrectedTrkIso03;
  reco::GsfElectron::IsolationVariables dr03;
  dr03.tkSumPt = EleTkIsolationCorr.getCorrectedTrkIso(&(*gsfiter));
  dr03.hcalDepth1TowerSumEt = gsfiter->dr03HcalDepth1TowerSumEt() ;
  dr03.hcalDepth2TowerSumEt = gsfiter->dr03HcalDepth2TowerSumEt();
  dr03.hcalDepth1TowerSumEtBc = gsfiter->dr03HcalDepth1TowerSumEtBc() ;
  dr03.hcalDepth2TowerSumEtBc = gsfiter->dr03HcalDepth2TowerSumEtBc() ;
  dr03.ecalRecHitSumEt = gsfiter->dr03EcalRecHitSumEt() ;
  CorrectedEle.setIsolation03(dr03);

  prod->push_back(reco::GsfElectron(CorrectedEle));

  }

  iEvent.put(prod);
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
CorrectedElectronTrkisoProducers::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
CorrectedElectronTrkisoProducers::endStream() {
}
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CorrectedElectronTrkisoProducers::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CorrectedElectronTrkisoProducers);
