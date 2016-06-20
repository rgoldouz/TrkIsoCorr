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
  const reco::TrackCollection * generalTrackCollection = generalTracksHandle.product();

  edm::Handle<reco::GsfElectronCollection> electronHandle ;
  iEvent.getByToken( electronCollectionToken_ , electronHandle) ;
  const reco::GsfElectronCollection * gsfElectronCollection = electronHandle.product();

  edm::Handle<reco::BeamSpot> beamSpotHandle ;
  iEvent.getByToken(beamSpotToken_, beamSpotHandle) ;

  std::auto_ptr<std::vector<reco::GsfElectron> > prod(new std::vector<reco::GsfElectron>());

  ElectronTkIsolationCorr electronTkIsolationCorr(0.3,generalTrackCollection,&(*beamSpotHandle));

  for (reco::GsfElectronCollection::const_iterator gsfiter = gsfElectronCollection->begin(); gsfiter != gsfElectronCollection->end(); ++gsfiter) {	

    reco::GsfElectron correctedElectron(*gsfiter);
    reco::GsfElectron::IsolationVariables dr03;
    dr03.tkSumPt = electronTkIsolationCorr.getCorrectedTrkIso(&(*gsfiter));
    dr03.hcalDepth1TowerSumEt = gsfiter->dr03HcalDepth1TowerSumEt() ;
    dr03.hcalDepth2TowerSumEt = gsfiter->dr03HcalDepth2TowerSumEt();
    dr03.hcalDepth1TowerSumEtBc = gsfiter->dr03HcalDepth1TowerSumEtBc() ;
    dr03.hcalDepth2TowerSumEtBc = gsfiter->dr03HcalDepth2TowerSumEtBc() ;
    dr03.ecalRecHitSumEt = gsfiter->dr03EcalRecHitSumEt() ;
    correctedElectron.setIsolation03(dr03);

    prod->push_back(reco::GsfElectron(correctedElectron));
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
