/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 ************************************v**************************************/

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TList.h"
#include "TMath.h"
#include "Riostream.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskCorrelationhCascDATA.h"
#include "AliPIDResponse.h"
#include "AliMultiplicity.h"
#include "AliMultSelection.h"
#include "AliAODMCParticle.h"
#include "AliAnalysisCorrelationEventCollection.h"
#include "AliAODVertex.h"
#include "AliEventCuts.h"
#include "AliAODTrack.h"
#include "AliAODv0.h"
#include "AliAODcascade.h"
#include "AliVVertex.h"
#include "AliESDtrack.h"
#include "AliESDEvent.h"
#include "AliAnalysisDataSlot.h"
#include "AliAnalysisDataContainer.h"

class AliAnalysisTaskCorrelationhCascDATA;   
using namespace std;          
ClassImp(AliAnalysisTaskCorrelationhCascDATA) 

AliAnalysisTaskCorrelationhCascDATA::AliAnalysisTaskCorrelationhCascDATA() :AliAnalysisTaskSE(), 
  fAnalysisType("AOD"), 
  fCollidingSystem("pp"), 
  fAOD(0), 
  fPIDResponse(0),
  fEventCuts(0), 			  			
  fOutputList(0), 
  fSignalTree(0), 
  fBkgTree(0), 
  fOutputList2(0),
  fOutputList3(0),  
  fOutputList4(0),  
  fMCEvent(0), 
  fReadMCTruth(0),
  isEfficiency(0),
  isHybridMCTruth(0),
  fEventColl(0x0), 
  fEvt(0x0), 
  fzVertexBins(10), 
  fnMultBins(20),	 
  fMaxFirstMult(50),
								    fMaxSecondMult(150),
								    fnEventsToMix(50),
								    fEtaTrigger(0.8),
  fEtahAssoc(0.8),
  fEtaV0Assoc(0.8),
  fFilterBitValue(128),
  fYear(2016),
  fisInclusiveINELgt0(0),
  lPercentilesMin(0),
  lPercentilesMax(0),
  fisHM(0),
  fHistPt(0), 
  fHistDCAxym1(0),
  fHistDCAzm1(0),
  fHistDCAxym2(0),
  fHistDCAzm2(0),
  fHistPtV0(0),
  fHistPhi(0),
  fHistTheta(0),
  fHistEta(0), 
  fHistPtTMaxBefAllCfrDataMC(0),
  fHistPtTMinBefAll(0),
  fHistPtTMinBefAllMC(0),
  fHistPtMaxvsMultBefRSelection(0),
  fHistPtMaxvsMultAfterRSelection(0),
  fHistPtvsMult(0), 
  fHistPtvsMultBefAll(0), 
  fHistPtMaxvsMult(0), 
  fHistPtMaxvsMultKeepV0(0), 
  fHistPtMaxvsMultSkipV0(0), 
  fHistPtMaxvsMultBefAll(0),
  fHistPtMaxvsMultBefAllReco(0),
  fHistPtMaxvsMultBefAllGen(0), 
  fHistZvertex(0),  
  fHistFractionSharedTPCClusters(0),
  fHistGoldenCut(0),
  fHistNumberChargedAllEvents(0),
  fHistNumberChargedNoTrigger(0),
  fHistNumberChargedTrigger(0),
  fHist_eta_phi(0),  
  fHist_eta_phi_PtMax(0),
  fHist_multiplicityAllSelEvents(0),  
  fHist_multiplicity(0),
  fHist_multiplicity_EvwTrigger(0),
  fHistEventMult(0),
  fHistTriggerFractionNum(0), 
  fHistTriggerFractionDenom(0), 
  fHistEventV0(0), 
  fHistTrack(0), 
  fHistLengthvsCrossedRowsAfterSel(0),
  fHistLengthvsCrossedRows(0),
  fHistIsCommonParton(0),
  fHistCommonParton(0),
  fHistCommonPartonTrueCasc(0),
  fHistAllGenParticleOrigin(0),
  fHistAllGenParticleMOrigin(0),
  fHistAllGenParticleGMOrigin(0),
  fHistTriggerComposition(0), 
  fHistTriggerCompositionMCTruth(0), 
  fHistPDG(0),
  fHistTrackBufferOverflow(0), 
  fHistSecondParticleAll(0),
  fHistSecondParticleTruthAll(0),
  fHistSecondParticle(0),
  fHistSecondParticleTruth(0),
  fMassV0(0), 
  fHistMultvsV0All(0),
  fHistMultvsV0AllTruth(0),
  fHistMultvsV0MCAll(0),
  fHistMultvsV0(0),
  fHistMultvsV0Truth(0),
  fHistMultvsV0MC(0),
  fHistTriggerNotLeading(0),
  fHistTriggerNotLeadingMC(0),
  fHistMassvsPt_tagli(0x0),
  fHistMultvsTriggerBefAll(0),
  fHistMultvsTriggerMCTruthBefAll(0),
  fHistMultvsTriggerAll(0),
  fHistMultvsTriggerMCTruthAll(0),
  fHistMultvsTrigger(0),
  fHistMultvsTriggerMCTruth(0),
  fHistTrigger(0),
  fHistTriggerMCTruth(0),
  fHistTriggerwV0(0),
  fHistTriggerwV0MCTruth(0),
  fHistMultiplicityVsVertexZ(0),
  fHistTriggervsMult(0),
  fHistTriggervsMultMC(0),
  fHistMultiplicityOfMixedEvent(0),
  fminPtj(2), 
  fmaxPtj(10), 
  fV0("kK0s"),  
  fminPtV0(0), 
  fmaxPtV0(30),  
  Evcounter(0), 
  Evcounterczero(0),
  fmolt(5),
  farrGT(0), 
  fTrackBufferSize(20200),
  fTreeVariablePtTrigger(0),		      
  fTreeVariableChargeTrigger(0),		      
  fTreeVariableEtaTrigger(0), 		      
  fTreeVariablePhiTrigger(0),		      
  fTreeVariableDCAz(0),			      
  fTreeVariableDCAxy(0),
  fTreeVariableChargeAssoc(0),     
  fTreeVariableAssocDCAz(0),
  fTreeVariableAssocDCAxy(0),  
  fTreeVariableisPrimaryTrigger(0),  
  fTreeVariableisPrimaryV0(0),  
  fTreeVariableRapAssoc(0),		      	      
  fTreeVariableDcaXiDaughters(0),	      	      
//  fTreeVariableDcaPosToPrimVertex(0),	      	      
//  fTreeVariableDcaNegToPrimVertex(0),	      	      
  fTreeVariableV0CosineOfPointingAngle(0),      	      
  fTreeVariableXiCosineOfPointingAngle(0),      	      
  fTreeVariablePtV0(0),			      
  fTreeVariablectau(0),			      
  fTreeVariableInvMassLambda(0),		      
  fTreeVariableInvMassXi(0),		      
  fTreeVariableInvMassOmega(0),		      
  fTreeVariableEtaV0(0),			      
  fTreeVariablePhiV0(0),			      
  fTreeVariableSkipAssoc(0),			      
  fTreeVariableIsCommonParton(0),
  fTreeVariablePdgCommonParton(0),
  fTreeVariableDeltaEta(0),			       
  fTreeVariableDeltaPhi(0),
  fTreeVariableDeltaTheta(0),			       
  fTreeVariableMultiplicity(0),                   
  fTreeVariableZvertex(0),
  fTreeVariableTriggerIndex(0),
  fTreeVariablePDGCodeTrigger(0),
  fTreeVariablePDGCodeAssoc(0),
  FifoShiftok(kFALSE)
{
  // default constructor, don't allocate memory here!
  // this is used by root for IO purpos, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskCorrelationhCascDATA::AliAnalysisTaskCorrelationhCascDATA(const char* name) : AliAnalysisTaskSE(name),
										     fAnalysisType("AOD"), 
										     fCollidingSystem("pp"), 
										     fAOD(0), 
										     fPIDResponse(0),
										     fEventCuts(0),								 
										     fOutputList(0), 
										     fSignalTree(0), 
										     fBkgTree(0), 
										     fOutputList2(0), 
										     fOutputList3(0), 
										     fOutputList4(0), 
										     fMCEvent(0), 
										     fReadMCTruth(0),
										     isEfficiency(0),
  isHybridMCTruth(0),
  fEventColl(0x0), 
  fEvt(0x0), 
  fzVertexBins(10), 
  fnMultBins(20),	 
  fMaxFirstMult(50),
  fMaxSecondMult(150),
  fnEventsToMix(50),
  fEtaTrigger(0.8),
  fEtahAssoc(0.8),
  fEtaV0Assoc(0.8),
  fFilterBitValue(128),
  fYear(2016),
  fisInclusiveINELgt0(0),
  lPercentilesMin(0),
  lPercentilesMax(0),
  fisHM(0),
  fHistPt(0), 
  fHistDCAxym1(0),
  fHistDCAzm1(0),
  fHistDCAxym2(0),
  fHistDCAzm2(0),
  fHistPtV0(0), 
  fHistPhi(0),
  fHistTheta(0),
  fHistEta(0),
  fHistPtTMaxBefAllCfrDataMC(0), 
  fHistPtTMinBefAll(0),
  fHistPtTMinBefAllMC(0),
  fHistPtMaxvsMultBefRSelection(0),
  fHistPtMaxvsMultAfterRSelection(0),
  fHistPtvsMult(0), 
  fHistPtvsMultBefAll(0), 
  fHistPtMaxvsMult(0), 
  fHistPtMaxvsMultKeepV0(0), 
  fHistPtMaxvsMultSkipV0(0), 
  fHistPtMaxvsMultBefAll(0), 
  fHistPtMaxvsMultBefAllReco(0),
  fHistPtMaxvsMultBefAllGen(0),
  fHistZvertex(0),  
  fHistFractionSharedTPCClusters(0),
  fHistGoldenCut(0),
  fHistNumberChargedAllEvents(0),
  fHistNumberChargedNoTrigger(0),
  fHistNumberChargedTrigger(0),
  fHist_eta_phi(0),  
  fHist_eta_phi_PtMax(0),  
  fHist_multiplicityAllSelEvents(0),
  fHist_multiplicity(0),
  fHist_multiplicity_EvwTrigger(0),
  fHistEventMult(0),
  fHistTriggerFractionNum(0),
  fHistTriggerFractionDenom(0),   
  fHistEventV0(0), 
  fHistTrack(0), 
  fHistLengthvsCrossedRowsAfterSel(0),
  fHistLengthvsCrossedRows(0),
  fHistIsCommonParton(0),
  fHistCommonParton(0),
  fHistCommonPartonTrueCasc(0),
  fHistAllGenParticleOrigin(0),
  fHistAllGenParticleMOrigin(0),
  fHistAllGenParticleGMOrigin(0),
  fHistTriggerComposition(0), 
  fHistTriggerCompositionMCTruth(0), 
  fHistPDG(0), 
  fHistTrackBufferOverflow(0), 
  fHistSecondParticleAll(0),
  fHistSecondParticleTruthAll(0),
  fHistSecondParticle(0),
  fHistSecondParticleTruth(0),
  fMassV0(0),
  fHistMultvsV0All(0),
  fHistMultvsV0AllTruth(0),
  fHistMultvsV0MCAll(0), 
  fHistMultvsV0(0),
  fHistMultvsV0Truth(0),
  fHistMultvsV0MC(0),
  fHistTriggerNotLeading(0),
  fHistTriggerNotLeadingMC(0),
  fHistMassvsPt_tagli(0x0),
  fHistMultvsTriggerBefAll(0),
  fHistMultvsTriggerMCTruthBefAll(0),
  fHistMultvsTriggerAll(0),
  fHistMultvsTriggerMCTruthAll(0),
  fHistMultvsTrigger(0),
  fHistMultvsTriggerMCTruth(0),
  fHistTrigger(0),
  fHistTriggerMCTruth(0),
  fHistTriggerwV0(0),
  fHistTriggerwV0MCTruth(0),
  fHistMultiplicityVsVertexZ(0),
  fHistTriggervsMult(0),
  fHistTriggervsMultMC(0),
  fHistMultiplicityOfMixedEvent(0),
  fminPtj(2), 
  fmaxPtj(10), 
  fV0("kK0s"),  
  fminPtV0(0), 
  fmaxPtV0(30),  
  Evcounter(0), 
  Evcounterczero(0),
  fmolt(5),
  farrGT(0), 
  fTrackBufferSize(20200),
  fTreeVariablePtTrigger(0),		      
  fTreeVariableChargeTrigger(0),		      
  fTreeVariableEtaTrigger(0), 		      
  fTreeVariablePhiTrigger(0),		      
  fTreeVariableDCAz(0),			      
  fTreeVariableDCAxy(0),		
  fTreeVariableChargeAssoc(0),     
  fTreeVariableAssocDCAz(0),
  fTreeVariableAssocDCAxy(0),  	      
  fTreeVariableisPrimaryTrigger(0),
  fTreeVariableisPrimaryV0(0),
  fTreeVariableRapAssoc(0),		      	      
  fTreeVariableDcaXiDaughters(0),	      	      
//  fTreeVariableDcaPosToPrimVertex(0),	      	      
//  fTreeVariableDcaNegToPrimVertex(0),	      	      
  fTreeVariableV0CosineOfPointingAngle(0),      	      
  fTreeVariableXiCosineOfPointingAngle(0),      	      
  fTreeVariablePtV0(0),			      
  fTreeVariablectau(0),			      
  fTreeVariableInvMassLambda(0),		      
  fTreeVariableInvMassXi(0),		      
  fTreeVariableInvMassOmega(0),		      
  fTreeVariableEtaV0(0),			      
  fTreeVariablePhiV0(0),			      
  fTreeVariableSkipAssoc(0),			      
  fTreeVariableIsCommonParton(0),
  fTreeVariablePdgCommonParton(0),
  fTreeVariableDeltaEta(0),			       
  fTreeVariableDeltaPhi(0),
  fTreeVariableDeltaTheta(0),			       
  fTreeVariableMultiplicity(0),                   
  fTreeVariableZvertex(0),
  fTreeVariableTriggerIndex(0),
  fTreeVariablePDGCodeTrigger(0),
  fTreeVariablePDGCodeAssoc(0),
  FifoShiftok(kFALSE)
{
                      
  DefineInput(0, TChain::Class()); 
  DefineOutput(1, TList::Class());
  DefineOutput(2, TTree::Class());  
  DefineOutput(3, TTree::Class());
  DefineOutput(4, TList::Class());  
  DefineOutput(5, TList::Class());  
  DefineOutput(6, TList::Class());  
}
//_____________________________________________________________________________
AliAnalysisTaskCorrelationhCascDATA::~AliAnalysisTaskCorrelationhCascDATA()
{

  if(fOutputList) {
    delete fOutputList;  
  }
  if(fSignalTree) {
    delete fSignalTree;
  }
  if(fBkgTree) {
    delete fBkgTree;   
  }
  if(fOutputList2) {
    delete fOutputList2;
  }
  if(fOutputList3) {
    delete fOutputList3;
  }
  if(fOutputList4) {
    delete fOutputList4;
  }
  if (farrGT)
    delete[] farrGT;
  farrGT=0;

  for(unsigned short i=0; i < fzVertexBins; i++){
    for(unsigned short j=0; j < fnMultBins; j++){
      delete fEventColl[i][j];
    }
    delete[] fEventColl[i];
  }
  delete[] fEventColl;
  
}
/*  
    void AliAnalysisTaskCorrelationhCascDATA::Propagate( Double_t vv[3],  Double_t x[3], Double_t p[3], Double_t bz, Double_t sign) const{
    //Propagation to the primary vertex to determine the px and py
    //x, p are the position and momentum as input and output
    //bz is the magnetic field along z direction
    //sign is the charge of particle for propagation

    Double_t pp = TMath::Sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]); //total momentum
    Double_t len = (vv[2]-x[2])*pp/p[2];  
    Double_t a = -kB2C*bz*sign;

    Double_t rho = a/pp;
    x[0] += p[0]*TMath::Sin(rho*len)/a - p[1]*(1-TMath::Cos(rho*len))/a;
    x[1] += p[1]*TMath::Sin(rho*len)/a + p[0]*(1-TMath::Cos(rho*len))/a;
    x[2] += p[2]*len/pp;

    Double_t p0=p[0];
    p[0] = p0  *TMath::Cos(rho*len) - p[1]*TMath::Sin(rho*len);
    p[1] = p[1]*TMath::Cos(rho*len) + p0  *TMath::Sin(rho*len); 
  
    //  p[1] = p[1]*TMath::Cos(rho*len) + p0  *TMath::Sin(rho*len);
    }
*/

void AliAnalysisTaskCorrelationhCascDATA::Propagate( Double_t vv[3],  Double_t x[3], Double_t p[3], Double_t bz, Double_t sign) {
  //Propagation to the primary vertex to determine the px and py
  //x, p are the position and momentum as input and output
  //bz is the magnetic field along z direction
  //sign is the charge of particle for propagation

  const Double_t kB2C=0.299792458e-3; //light speed in m/ps
  Double_t pp = TMath::Sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]); //total momentum
  Double_t len = (vv[2]-x[2])*pp/p[2];  
  Double_t a = -kB2C*bz*sign;

  Double_t rho = a/pp;
  Double_t p0=p[0];
  p[0] = p0  *TMath::Cos(rho*len) - p[1]*TMath::Sin(rho*len);
  p[1] = p[1]*TMath::Cos(rho*len) + p0  *TMath::Sin(rho*len); 
  
}


//_____________________________________________________________________________
void AliAnalysisTaskCorrelationhCascDATA::UserCreateOutputObjects()
{
  Int_t NumBinsMult=100;
  if (fisHM) NumBinsMult=100;
  Float_t UpperLimitMult =100;
  if (fisHM) UpperLimitMult =0.1;

  Float_t lLimInfMass =0;
  Float_t lLimSupMass =0;
  Float_t lLimInfMassTight =0;
  Float_t lLimSupMassTight =0;
  if (fV0 == "Xi"){
    lLimInfMass=1.1;
    lLimSupMass=1.5;
    lLimInfMassTight=1.3;
    lLimSupMassTight=1.345;
  }
  if (fV0 == "Omega") {
    lLimInfMass=1.47;
    lLimSupMass=1.87;
    lLimInfMassTight=1.47;
    lLimSupMassTight=1.87;
  }

  fEventColl = new AliAnalysisCorrelationEventCollection **[fzVertexBins]; 
  
  for (unsigned short i=0; i<fzVertexBins; i++) {
    fEventColl[i] = new AliAnalysisCorrelationEventCollection *[fnMultBins];
    for (unsigned short j=0; j<fnMultBins; j++) {
      fEventColl[i][j] = new AliAnalysisCorrelationEventCollection(fnEventsToMix+1, fMaxFirstMult, fMaxSecondMult);
    }
  }

  // Store pointer to global tracks
  farrGT = new Int_t[fTrackBufferSize];

  fOutputList = new TList();         
  fOutputList->SetOwner(kTRUE);     
  fOutputList2 = new TList();         
  fOutputList2->SetOwner(kTRUE);     
  fOutputList3 = new TList();         
  fOutputList3->SetOwner(kTRUE);     
  fOutputList4 = new TList();         
  fOutputList4->SetOwner(kTRUE);     
  
  const char* nameoutputSignalTree = GetOutputSlot(2)->GetContainer()->GetName();
  fSignalTree= new TTree(nameoutputSignalTree,"fSignalTree");
  fSignalTree->Branch("fTreeVariablePtTrigger",          &fTreeVariablePtTrigger   , "fTreeVariablePtTrigger/D");
  fSignalTree->Branch("fTreeVariableChargeTrigger",      &fTreeVariableChargeTrigger, "fTreeVariableChargeTrigger/I");
  fSignalTree->Branch("fTreeVariableEtaTrigger",         &fTreeVariableEtaTrigger  , "fTreeVariableEtaTrigger/D");
  fSignalTree->Branch("fTreeVariablePhiTrigger",         &fTreeVariablePhiTrigger, "fTreeVariablePhiTrigger/D");
  fSignalTree->Branch("fTreeVariableDCAz",               &fTreeVariableDCAz  , "fTreeVariableDCAz/D");
  fSignalTree->Branch("fTreeVariableDCAxy",              &fTreeVariableDCAxy  , "fTreeVariableDCAxy/D");
  fSignalTree->Branch("fTreeVariableChargeAssoc",        &fTreeVariableChargeAssoc  , "fTreeVariableChargeAssoc/I");
  fSignalTree->Branch("fTreeVariableisPrimaryTrigger",   &fTreeVariableisPrimaryTrigger  , "fTreeVariableisPrimaryTrigger/I");
  fSignalTree->Branch("fTreeVariableisPrimaryV0",        &fTreeVariableisPrimaryV0  , "fTreeVariableisPrimaryV0/I");
  fSignalTree->Branch("fTreeVariableRapAssoc",         &fTreeVariableRapAssoc               ,"fTreeVariableRapAssoc/D");
  fSignalTree->Branch("fTreeVariableDcaXiDaughters",  &fTreeVariableDcaXiDaughters 	, "fTreeVariableDcaXiDaughters/D");
  //  fSignalTree->Branch("fTreeVariableDcaPosToPrimVertex", &fTreeVariableDcaPosToPrimVertex	, "fTreeVariableDcaPosToPrimVertex/D");
  //  fSignalTree->Branch("fTreeVariableDcaNegToPrimVertex", &fTreeVariableDcaNegToPrimVertex	, "fTreeVariableDcaNegToPrimVertex/D");
  fSignalTree->Branch("fTreeVariableV0CosineOfPointingAngle", &fTreeVariableV0CosineOfPointingAngle	, "fTreeVariableV0CosineOfPointingAngle/D");
  fSignalTree->Branch("fTreeVariableXiCosineOfPointingAngle", &fTreeVariableXiCosineOfPointingAngle	, "fTreeVariableXiCosineOfPointingAngle/D");
  fSignalTree->Branch("fTreeVariablePtV0",               &fTreeVariablePtV0   , "fTreeVariablePtV0/D");
  fSignalTree->Branch("fTreeVariablectau",               &fTreeVariablectau   , "fTreeVariablectau/D");
  fSignalTree->Branch("fTreeVariableInvMassLambda",      &fTreeVariableInvMassLambda, "fTreeVariableInvMassLambda/D");
  fSignalTree->Branch("fTreeVariableInvMassXi",          &fTreeVariableInvMassXi, "fTreeVariableInvMassXi/D");
  fSignalTree->Branch("fTreeVariableInvMassOmega",          &fTreeVariableInvMassOmega, "fTreeVariableInvMassOmega/D");
  fSignalTree->Branch("fTreeVariableEtaV0",              &fTreeVariableEtaV0  , "fTreeVariableEtaV0/D");
  fSignalTree->Branch("fTreeVariablePhiV0",              &fTreeVariablePhiV0, "fTreeVariablePhiV0/D");
  fSignalTree->Branch("fTreeVariableSkipAssoc",          &fTreeVariableSkipAssoc, "fTreeVariableSkipAssoc/O");
  fSignalTree->Branch("fTreeVariableIsCommonParton",          &fTreeVariableIsCommonParton, "fTreeVariableIsCommonParton/O");
  fSignalTree->Branch("fTreeVariablePdgCommonParton",          &fTreeVariablePdgCommonParton, "fTreeVariablePdgCommonParton/I");
  fSignalTree->Branch("fTreeVariableDeltaEta",           &fTreeVariableDeltaEta  , "fTreeVariableDeltaEta/D");
  fSignalTree->Branch("fTreeVariableDeltaPhi",           &fTreeVariableDeltaPhi, "fTreeVariableDeltaPhi/D");
  fSignalTree->Branch("fTreeVariableDeltaTheta",         &fTreeVariableDeltaTheta, "fTreeVariableDeltaTheta/D");
  fSignalTree->Branch("fTreeVariableMultiplicity",       &fTreeVariableMultiplicity , "fTreeVariableMultiplicity/D");
  fSignalTree->Branch("fTreeVariableZvertex",            &fTreeVariableZvertex  , "fTreeVariableZvertex/D");
  fSignalTree->Branch("fTreeVariableTriggerIndex",            &fTreeVariableTriggerIndex  , "fTreeVariableTriggerIndex/I");
  fSignalTree->Branch("fTreeVariablePDGCodeTrigger",     &fTreeVariablePDGCodeTrigger  , "fTreeVariablePDGCodeTrigger/I");
  fSignalTree->Branch("fTreeVariablePDGCodeAssoc",       &fTreeVariablePDGCodeAssoc  , "fTreeVariablePDGCodeAssoc/I");

  const char* nameoutputBkgTree = GetOutputSlot(3)->GetContainer()->GetName();
  fBkgTree= new TTree(nameoutputBkgTree, "fBkgTree");
  fBkgTree->Branch("fTreeVariablePtTrigger",                  &fTreeVariablePtTrigger   ,  "fTreeVariablePtTrigger/D");
  fBkgTree->Branch("fTreeVariableChargeTrigger",              &fTreeVariableChargeTrigger, "fTreeVariableChargeTrigger/I");
  fBkgTree->Branch("fTreeVariableEtaTrigger",                 &fTreeVariableEtaTrigger  , "fTreeVariableEtaTrigger/D");
  fBkgTree->Branch("fTreeVariablePhiTrigger",                 &fTreeVariablePhiTrigger, "fTreeVariablePhiTrigger/D");
  fBkgTree->Branch("fTreeVariableDCAz",                       &fTreeVariableDCAz  , "fTreeVariableDCAz/D");
  fBkgTree->Branch("fTreeVariableDCAxy",                      &fTreeVariableDCAxy  , "fTreeVariableDCAxy/D");
  fBkgTree->Branch("fTreeVariableChargeAssoc",                &fTreeVariableChargeAssoc  , "fTreeVariableChargeAssoc/I");
  fBkgTree->Branch("fTreeVariableisPrimaryTrigger",           &fTreeVariableisPrimaryTrigger  , "fTreeVariableisPrimaryTrigger/I");  
  fBkgTree->Branch("fTreeVariableisPrimaryV0",                &fTreeVariableisPrimaryV0  , "fTreeVariableisPrimaryV0/I");  
  fBkgTree->Branch("fTreeVariableRapAssoc",                   &fTreeVariableRapAssoc               ,"fTreeVariableRapAssoc/D");
  fBkgTree->Branch("fTreeVariableDcaXiDaughters",             &fTreeVariableDcaXiDaughters 	, "fTreeVariableDcaXiDaughters/D");
  //  fBkgTree->Branch("fTreeVariableDcaPosToPrimVertex",     &fTreeVariableDcaPosToPrimVertex	, "fTreeVariableDcaPosToPrimVertex/D");
  //  fBkgTree->Branch("fTreeVariableDcaNegToPrimVertex",     &fTreeVariableDcaNegToPrimVertex	, "fTreeVariableDcaNegToPrimVertex/D");
  fBkgTree->Branch("fTreeVariableV0CosineOfPointingAngle",    &fTreeVariableV0CosineOfPointingAngle	, "fTreeVariableV0CosineOfPointingAngle/D");
  fBkgTree->Branch("fTreeVariableXiCosineOfPointingAngle",    &fTreeVariableXiCosineOfPointingAngle	, "fTreeVariableXiCosineOfPointingAngle/D");
  fBkgTree->Branch("fTreeVariablePtV0",               &fTreeVariablePtV0   , "fTreeVariablePtV0/D");
  fBkgTree->Branch("fTreeVariablectau",               &fTreeVariablectau   , "fTreeVariablectau/D");
  fBkgTree->Branch("fTreeVariableInvMassLambda",      &fTreeVariableInvMassLambda, "fTreeVariableInvMassLambda/D");
  fBkgTree->Branch("fTreeVariableInvMassXi",          &fTreeVariableInvMassXi, "fTreeVariableInvMassXi/D");
  fBkgTree->Branch("fTreeVariableInvMassOmega",       &fTreeVariableInvMassOmega, "fTreeVariableInvMassOmega/D");
  fBkgTree->Branch("fTreeVariableEtaV0",              &fTreeVariableEtaV0  , "fTreeVariableEtaV0/D");
  fBkgTree->Branch("fTreeVariablePhiV0",              &fTreeVariablePhiV0, "fTreeVariablePhiV0/D");
  fBkgTree->Branch("fTreeVariableSkipAssoc",          &fTreeVariableSkipAssoc, "fTreeVariableSkipAssoc/O");
  fBkgTree->Branch("fTreeVariableIsCommonParton",          &fTreeVariableIsCommonParton, "fTreeVariableIsCommonParton/O");
  fBkgTree->Branch("fTreeVariablePdgCommonParton",          &fTreeVariablePdgCommonParton, "fTreeVariablePdgCommonParton/I");
  fBkgTree->Branch("fTreeVariableDeltaEta",           &fTreeVariableDeltaEta  , "fTreeVariableDeltaEta/D");
  fBkgTree->Branch("fTreeVariableDeltaPhi",           &fTreeVariableDeltaPhi  , "fTreeVariableDeltaPhi/D");
  fBkgTree->Branch("fTreeVariableDeltaTheta",         &fTreeVariableDeltaTheta, "fTreeVariableDeltaTheta/D");
  fBkgTree->Branch("fTreeVariableMultiplicity",       &fTreeVariableMultiplicity , "fTreeVariableMultiplicity/D");
  fBkgTree->Branch("fTreeVariableZvertex",            &fTreeVariableZvertex  , "fTreeVariableZvertex/D");
  fBkgTree->Branch("fTreeVariableTriggerIndex",            &fTreeVariableTriggerIndex  , "fTreeVariableTriggerIndex/I");
  fBkgTree->Branch("fTreeVariablePDGCodeTrigger",     &fTreeVariablePDGCodeTrigger  , "fTreeVariablePDGCodeTrigger/I");
  fBkgTree->Branch("fTreeVariablePDGCodeAssoc",       &fTreeVariablePDGCodeAssoc  , "fTreeVariablePDGCodeAssoc/I");
  
  fHistPt = new TH1F("fHistPt", "p_{T} distribution of selected charged tracks in events used for AC", 300, 0, 30); 
  fHistPt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
 
  fHistDCAxym1 = new TH1F("fHistDCAxym1", "DCAxy method 1 before DCA cuts", 1000, -10, 10); 
  fHistDCAxym1->GetXaxis()->SetTitle("DCAxy method 1 (cm)");
  fHistDCAxym2 = new TH1F("fHistDCAxym2", "DCAxy method 2 before DCA cuts", 1000, -10, 10); 
  fHistDCAxym2->GetXaxis()->SetTitle("DCAxy method 2 (cm)");
  fHistDCAzm1 = new TH1F("fHistDCAzm1", "DCAz method 1 before DCA cuts", 1000, -10, 10); 
  fHistDCAzm1->GetXaxis()->SetTitle("DCAz method 1 (cm)");
  fHistDCAzm2 = new TH1F("fHistDCAzm2", "DCAz method 2 before DCA cuts", 1000, -10, 10); 
  fHistDCAzm2->GetXaxis()->SetTitle("DCAz method 2 (cm)");
 
  fHistPtV0 = new TH1F("fHistPtV0", "p_{T} distribution of selected V0 in events used for AC", 300, 0, 30); 
  fHistPtV0->GetXaxis()->SetTitle("p_{T} (GeV/c)");

  fHistPtTMaxBefAllCfrDataMC= new TH2F("fHistPtTMaxBefAllCfrDataMC", "p_{T} distribution of trigger particle with Pt maximum in the event", 300, 0, 30, 300, 0,30); 
  fHistPtTMaxBefAllCfrDataMC->GetXaxis()->SetTitle("p^{Trigger, Max}_{T} (reco)(GeV/c)");
  fHistPtTMaxBefAllCfrDataMC->GetYaxis()->SetTitle("p^{Trigger, Max}_{T} (MC)(GeV/c)");

  fHistPtTMinBefAll = new TH1F("fHistPtTMinBefAll", "p_{T} distribution of reco trigger particle with Pt minimum in the event", 300, 0, 30); 
  fHistPtTMinBefAll->GetXaxis()->SetTitle("p_{T} (GeV/c)");

  fHistPtTMinBefAllMC = new TH1F("fHistPtTMinBefAllMC", "p_{T} distribution of true trigger particle with Pt minimum in the event", 300, 0, 30); 
  fHistPtTMinBefAllMC->GetXaxis()->SetTitle("p_{T} (GeV/c)");

  fHistPtvsMultBefAll= new TH2F("fHistPtvsMultBefAll", "p_{T} and centrality distribution of charged tracks in events w T>0", 300, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtvsMultBefAll->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtvsMultBefAll->GetYaxis()->SetTitle("Centrality");

  fHistPtvsMult= new TH2F("fHistPtvsMult", "p_{T} and centrality distribution of charged tracks in events used for AC", 300, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtvsMult->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtvsMult->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMultBefRSelection = new TH2F("fHistPtMaxvsMultBefRSelection", "p_{T} distribution of selected reco triggers (possibly also K0s daughters)", 300, 0, 30,   NumBinsMult, 0, UpperLimitMult);
  fHistPtMaxvsMultBefRSelection->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultBefRSelection->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMultAfterRSelection = new TH2F("fHistPtMaxvsMultAfterRSelection", "p_{T} distribution of selected reco triggers (no K0s daughters)", 300, 0, 30,  NumBinsMult, 0, UpperLimitMult);
  fHistPtMaxvsMultAfterRSelection->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultAfterRSelection->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMultBefAll= new TH2F("fHistPtMaxvsMultBefAll", "p_{T} and centrality distribution of charged tracks with maxiumum pt in events w T>0", 600, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtMaxvsMultBefAll->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultBefAll->GetYaxis()->SetTitle("Centrality");


  fHistPtMaxvsMultBefAllReco= new TH2F("fHistPtMaxvsMultBefAllReco", "p_{T} and centrality distribution of charged tracks with maxiumum pt in events w T>0", 600, 0, 30, NumBinsMult, 0, UpperLimitMult);
  fHistPtMaxvsMultBefAllReco->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultBefAllReco->GetYaxis()->SetTitle("Centrality");


  fHistPtMaxvsMultBefAllGen= new TH2F("fHistPtMaxvsMultBefAllGen", "p_{T} and centrality distribution of charged tracks with maxiumum pt in events w T>0", 600, 0, 30, NumBinsMult, 0, UpperLimitMult);
  fHistPtMaxvsMultBefAllGen->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultBefAllGen->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMult= new TH2F("fHistPtMaxvsMult", "p_{T} and centrality distribution of charged tracks with maximum pT in events used for AC)", 600, 0, 30, NumBinsMult, 0, UpperLimitMult);
  fHistPtMaxvsMult->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMult->GetYaxis()->SetTitle("Centrality");
  fHistPtMaxvsMult= new TH2F("fHistPtMaxvsMult", "p_{T} and centrality distribution of charged tracks with maximum pT in events used for AC)", 600, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtMaxvsMult->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMult->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMultKeepV0= new TH2F("fHistPtMaxvsMultKeepV0", "p_{T} and centrality distribution of charged tracks with maximum pT in events used for AC (with at least one Casc pt<pT,Trig)", 600, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtMaxvsMultKeepV0->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultKeepV0->GetYaxis()->SetTitle("Centrality");

  fHistPtMaxvsMultSkipV0= new TH2F("fHistPtMaxvsMultSkipV0", "p_{T} and centrality distribution of charged tracks with maximum pT in events used for AC (with at least one Casc pt>pT,Trig)", 600, 0, 30, NumBinsMult, 0, UpperLimitMult); 
  fHistPtMaxvsMultSkipV0->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistPtMaxvsMultSkipV0->GetYaxis()->SetTitle("Centrality");

  fHistZvertex= new TH1F("fHistZvertex", "Z vertex distribution of selected events used for AC", 40,-20,20);

  fHistFractionSharedTPCClusters = new TH1F ("fHistFractionSharedTPCClusters", "fHistFractionSharedTPCClusters",100, 0,1);

  fHistGoldenCut= new TH1F ("fHistGoldenCut", "fHistGoldenCut", 100, 0, 100);

  fHistNumberChargedAllEvents=new TH3F("fHistNumberChargedAllEvents", "fHistNumberChargedAllEvents", NumBinsMult,0,UpperLimitMult, 100,0,100, 60, 0,30);
  fHistNumberChargedAllEvents->GetXaxis()->SetTitle("Multiplicity class");
  fHistNumberChargedAllEvents->GetYaxis()->SetTitle("Number of charged primary particles");
  fHistNumberChargedAllEvents->GetZaxis()->SetTitle("p^{Trigg, Max}_{T} (GeV/c)");

  fHistNumberChargedNoTrigger=new TH3F("fHistNumberChargedNoTrigger", "fHistNumberChargedNoTrigger", NumBinsMult,0,UpperLimitMult, 100,0,100, 60, 0,30);
  fHistNumberChargedNoTrigger->GetXaxis()->SetTitle("Multiplicity class");
  fHistNumberChargedNoTrigger->GetYaxis()->SetTitle("Number of charged primary particles");
  fHistNumberChargedNoTrigger->GetZaxis()->SetTitle("p^{Trigg, Max}_{T} (GeV/c)");

  fHistNumberChargedTrigger=new TH3F("fHistNumberChargedTrigger", "fHistNumberChargedTrigger", NumBinsMult,0,UpperLimitMult, 100,0,100, 60, 0,30);
  fHistNumberChargedTrigger->GetXaxis()->SetTitle("Multiplicity class");
  fHistNumberChargedTrigger->GetYaxis()->SetTitle("Number of charged primary particles");
  fHistNumberChargedTrigger->GetZaxis()->SetTitle("p^{Trigg, Max}_{T} (GeV/c)");

  fHist_eta_phi= new TH2F("fHist_eta_phi", "Distribution of charged tracks in events used for AC", 400, 0,2*TMath::Pi(), 400,-0.8, 0.8);
  fHist_eta_phi->GetYaxis()->SetTitle("Eta");
  fHist_eta_phi->GetXaxis()->SetTitle("Phi (radians)"); 

  fHist_eta_phi_PtMax= new TH2F("fHist_eta_phi_PtMax", "Distribution of charged tracks with maximum Pt in events used for AC", 400, 0,2*TMath::Pi(), 400,-0.8, 0.8);
  fHist_eta_phi_PtMax->GetYaxis()->SetTitle("Eta");
  fHist_eta_phi_PtMax->GetXaxis()->SetTitle("Phi (radians)"); 

  fHist_multiplicityAllSelEvents=new TH1F("fHist_multiplicityAllSelEvents", "fHist_multiplicityAllSelEvents", NumBinsMult, 0, UpperLimitMult);
  fHist_multiplicityAllSelEvents->SetTitle("Centrality distribution of selected INT7/HM events");
  fHist_multiplicity=new TH1F("fHist_multiplicity", "fHist_multiplicity", NumBinsMult, 0, UpperLimitMult); 
  fHist_multiplicity->SetTitle("Centrality distribution of events used for AC");
  fHist_multiplicity_EvwTrigger= new TH1F("fHist_multiplicity_EvwTrigger", "fHist_multiplicity_EvwTrigger", NumBinsMult, 0, UpperLimitMult); 
  fHist_multiplicity_EvwTrigger->SetTitle("Centrality distribution of events with NT>0");

  fHistPDG=new TH1F("fHistPDG", "fHistPDG",6400, -3200, 3200);

  fHistTrackBufferOverflow = new TH1F("fHistTrackBufferOverflow","",2,0,2);
  
  fHistEventMult=new TH1F("fHistEventMult", "fHistEventMult", 24, 0.5, 24.5);
  fHistEventMult->GetXaxis()->SetBinLabel(1,"Event cuts");
  fHistEventMult->GetXaxis()->SetBinLabel(2,"Events w/PV and AOD");
  fHistEventMult->GetXaxis()->SetBinLabel(3,"Events w/|Vx|<10 cm"); 
  fHistEventMult->GetXaxis()->SetBinLabel(4,"Events w/ PID"); 
  fHistEventMult->GetXaxis()->SetBinLabel(5,"centrality <= 199"); 
  fHistEventMult->GetXaxis()->SetBinLabel(6,"NO PILE UP"); 
  fHistEventMult->GetXaxis()->SetBinLabel(7,"INT7 or HM"); 
  fHistEventMult->GetXaxis()->SetBinLabel(8,"INEL>0");
  fHistEventMult->GetXaxis()->SetBinLabel(9,"isSelected"); 
  fHistEventMult->GetXaxis()->SetBinLabel(10,"Ntrigger>0"); 
  fHistEventMult->GetXaxis()->SetBinLabel(11,"Ntrigger>0 (MC)"); 
  fHistEventMult->GetXaxis()->SetBinLabel(12,"NTrigger>1");
  fHistEventMult->GetXaxis()->SetBinLabel(13,"NTrigger>1 (MC)");
  fHistEventMult->GetXaxis()->SetBinLabel(14,"NFirstPartMC < NFirstReco");
  fHistEventMult->GetXaxis()->SetBinLabel(15,"NFirstPartMC=0 && NFirstPart=1");
  fHistEventMult->GetXaxis()->SetBinLabel(16,"NFirstPartMC!=0 && NFirstPart!=0");
  fHistEventMult->GetXaxis()->SetBinLabel(17,"NSecondPartMC==0 && NSecondPart!=0 (NT>0)");
  fHistEventMult->GetXaxis()->SetBinLabel(18,"NSecondPartMC<NSecondRecoTrue (NT>0)");
  fHistEventMult->GetXaxis()->SetBinLabel(19,"NTrigger>0 && DoubleCounted");
  fHistEventMult->GetXaxis()->SetBinLabel(20,"");
  fHistEventMult->GetXaxis()->SetBinLabel(21,"");
  fHistEventMult->GetXaxis()->SetBinLabel(22,"SelEv (ACEvents)"); //all the events used for angular correlation
  fHistEventMult->GetXaxis()->SetBinLabel(23,"All events");
  fHistEventMult->GetXaxis()->SetBinLabel(24,"AOD event");

  fHistTriggerFractionNum=new TH1F("fHistTriggerFractionNum", "fHistTriggerFractionNum", 5, 0.5, 5.5); 
  fHistTriggerFractionDenom=new TH1F("fHistTriggerFractionDenom", "fHistTriggerFractionDenom", 5, 0.5, 5.5); 

  fHistEventV0=new TH1F("fHistEventV0", "fHistEventV0",32, 0.5, 32.5);
  fHistEventV0->SetTitle("Number of V0 which progressively pass the listed selections");
  fHistEventV0->GetXaxis()->SetBinLabel(1,"All Cascades");
  fHistEventV0->GetXaxis()->SetBinLabel(2,"Cascades ok");
  fHistEventV0->GetXaxis()->SetBinLabel(3,"Daughter tracks available"); 
  fHistEventV0->GetXaxis()->SetBinLabel(4,"Chis daughter tracks"); 
  fHistEventV0->GetXaxis()->SetBinLabel(5,"TPC Refit"); 
  fHistEventV0->GetXaxis()->SetBinLabel(6,"NClusters>70"); 
  fHistEventV0->GetXaxis()->SetBinLabel(7,"CrossedRows>80"); 
  fHistEventV0->GetXaxis()->SetBinLabel(8,"CrossedRows/Findable>0.8"); 
  fHistEventV0->GetXaxis()->SetBinLabel(9,"Length DTracks>90cm"); 
  fHistEventV0->GetXaxis()->SetBinLabel(10,"CrossedRows/Length>0.8"); 
  fHistEventV0->GetXaxis()->SetBinLabel(11,"Reject kink daughter"); 
  fHistEventV0->GetXaxis()->SetBinLabel(12,"PID daughter"); 
  fHistEventV0->GetXaxis()->SetBinLabel(13,"TOF or SPD hit for pileup"); 
  fHistEventV0->GetXaxis()->SetBinLabel(14,"|eta daughters|<0.8"); 
  fHistEventV0->GetXaxis()->SetBinLabel(15,"Lambda inv.mass cut"); 
  fHistEventV0->GetXaxis()->SetBinLabel(16,"DCA cuts"); 
  fHistEventV0->GetXaxis()->SetBinLabel(17,"Radii cut"); 
  fHistEventV0->GetXaxis()->SetBinLabel(18,"Cosine point.angles cut)"); 
  fHistEventV0->GetXaxis()->SetBinLabel(19,"V0 and Casc lifetime"); 
  fHistEventV0->GetXaxis()->SetBinLabel(20,"Mass cuts"); 
  fHistEventV0->GetXaxis()->SetBinLabel(21,"0<pT<pTV0max (reco up to here)"); 
  fHistEventV0->GetXaxis()->SetBinLabel(22,"NV0(reco) in ev wNT>0"); 
  fHistEventV0->GetXaxis()->SetBinLabel(23,"NV0(reco true) in ev wNT>0");
  fHistEventV0->GetXaxis()->SetBinLabel(24,"NV0(MC) in ev wNT>0");
  fHistEventV0->GetXaxis()->SetBinLabel(25,"N V0 pT>pTTrig(reco) in ev wNT>0");
  fHistEventV0->GetXaxis()->SetBinLabel(26,"N V0 pT>pTTrig(reco true) in ev wNT>0");
  fHistEventV0->GetXaxis()->SetBinLabel(27,"N V0 pT>pTTrig(MC) in ev wNT>0");
  fHistEventV0->GetXaxis()->SetBinLabel(28,"NV0(reco) in SelEv");
  fHistEventV0->GetXaxis()->SetBinLabel(29,"NV0(reco true) in SelEv");
  fHistEventV0->GetXaxis()->SetBinLabel(30,"NV0(MC) in SelEv"); 

  fHistTrack=new TH1F("fHistTrack", "fHistTrack", 19, 0.5, 19.5);
  fHistTrack->GetXaxis()->SetBinLabel(1,"All tracks");
  fHistTrack->GetXaxis()->SetBinLabel(2,"Tracks after filterbit");
  fHistTrack->GetXaxis()->SetBinLabel(3,"Tracks with |eta| < 0.8"); 
  fHistTrack->GetXaxis()->SetBinLabel(4,"Track quality");
  fHistTrack->GetXaxis()->SetBinLabel(5,"TPCCrossedRows>80");
  fHistTrack->GetXaxis()->SetBinLabel(6, " Crossed rows/findable >0.8");
  fHistTrack->GetXaxis()->SetBinLabel(7,"TrackLength>90");
  fHistTrack->GetXaxis()->SetBinLabel(8, "Crossed rows/track length> 0.8");
  fHistTrack->GetXaxis()->SetBinLabel(9,"Charged tracks");
  fHistTrack->GetXaxis()->SetBinLabel(10,"DCAxy < 0.010+0.035/pt**1.1");
  fHistTrack->GetXaxis()->SetBinLabel(11,"DCAz <2");
  fHistTrack->GetXaxis()->SetBinLabel(12,"N.trigger"); //NumberPrimary in all selected events 
  fHistTrack->GetXaxis()->SetBinLabel(13,"N.trigger MC");
  fHistTrack->GetXaxis()->SetBinLabel(14,"N.trigger (NT>0)"); //NumberPrimary in all selected events 
  fHistTrack->GetXaxis()->SetBinLabel(15,"N.trigger MC (NT>0)");
  fHistTrack->GetXaxis()->SetBinLabel(16,"N.trigger (NV0>0)"); //NumberPrimary in events with at least one V0 (one reco V0 for data, one true V0 for MC)
  fHistTrack->GetXaxis()->SetBinLabel(17,"N.trigger MC (NV0>0)");


  fHistLengthvsCrossedRows = new TH2F("fHistLengthvsCrossedRows", "fHistLengthvsCrossedRows",  160, 0, 160, 160, 0, 160);
  fHistLengthvsCrossedRows->GetXaxis()->SetTitle("Number of Crossed rows");
  fHistLengthvsCrossedRows->GetYaxis()->SetTitle("Track length");

  fHistLengthvsCrossedRowsAfterSel = new TH2F("fHistLengthvsCrossedRowsAfterSel", "fHistLengthvsCrossedRowsAfterSel",  160, 0, 160, 160, 0, 160);
  fHistLengthvsCrossedRowsAfterSel->GetXaxis()->SetTitle("Number of Crossed rows");
  fHistLengthvsCrossedRowsAfterSel->GetYaxis()->SetTitle("Track length");

  fHistIsCommonParton = new TH1F("fHistIsCommonParton", "fHistIsCommonParton", 2, -0.5,1.5);

  fHistCommonParton = new TH3F("fHistCommonParton", "fHistCommonParton", 50, -25,25, 10, 0, 10, 10, 0, 10);
  fHistCommonParton->GetXaxis()->SetTitle("PdgCode");
  fHistCommonParton->GetYaxis()->SetTitle("TriggerLevel");
  fHistCommonParton->GetZaxis()->SetTitle("CascLevel");

  fHistCommonPartonTrueCasc = new TH3F("fHistCommonPartonTrueCasc", "fHistCommonPartonTrueCasc", 50, -25,25, 10, 0, 10, 10, 0, 10);
  fHistCommonPartonTrueCasc->GetXaxis()->SetTitle("PdgCode");
  fHistCommonPartonTrueCasc->GetYaxis()->SetTitle("TriggerLevel");
  fHistCommonPartonTrueCasc->GetZaxis()->SetTitle("CascLevel");

  fHistAllGenParticleOrigin=new TH3F("fHistAllGenParticleOrigin", "fHistAllGenParticleOrigin", 600, -300,300, 300, -150, 150, 300, -150, 150);
  fHistAllGenParticleOrigin->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistAllGenParticleOrigin->GetYaxis()->SetTitle("PdgCode");
  fHistAllGenParticleOrigin->GetZaxis()->SetTitle("MCStatusCode");

  fHistAllGenParticleMOrigin=new TH3F("fHistAllGenParticleMOrigin", "fHistAllGenParticleMOrigin", 600, -300,300, 300, -150, 150, 300, -150, 150);
  fHistAllGenParticleMOrigin->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistAllGenParticleMOrigin->GetYaxis()->SetTitle("PdgCode");
  fHistAllGenParticleMOrigin->GetZaxis()->SetTitle("MCStatusCode");

  fHistAllGenParticleGMOrigin=new TH3F("fHistAllGenParticleGMOrigin", "fHistAllGenParticleGMOrigin", 600, -300,300, 300, -150, 150, 300, -150, 150);
  fHistAllGenParticleGMOrigin->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  fHistAllGenParticleGMOrigin->GetYaxis()->SetTitle("PdgCode");
  fHistAllGenParticleGMOrigin->GetZaxis()->SetTitle("MCStatusCode");

  fHistTriggerComposition=new TH3F("fHistTriggerComposition", "fHistTriggerComposition",10000 , -5000, 5000, 2, 0,2, 300, 0, 30);
  fHistTriggerComposition->GetYaxis()->SetTitle("0=NotPrim, 1=Primary");
  fHistTriggerComposition->GetZaxis()->SetTitle("p_{T} Trigger (GeV/c)");

  fHistTriggerCompositionMCTruth=new TH3F("fHistTriggerCompositionMCTruth", "fHistTriggerCompositionMCTruth",10000 , -5000, 5000, 2, 0,2, 300, 0, 30);
  fHistTriggerCompositionMCTruth->GetYaxis()->SetTitle("0=NotPrim, 1=Primary");
  fHistTriggerCompositionMCTruth->GetZaxis()->SetTitle("p_{T} Trigger (GeV/c)");

  fMassV0= new TH1F("fMassV0", "Invariant mass of V0 candidates", 100, lLimInfMassTight, lLimSupMassTight);
  fMassV0->GetXaxis()->SetTitle("Inv mass casc");

  fHistSecondParticleAll= new TH2F("fHistSecondParticleAll", "Number of V0 MCTrue vs number V0 reco (T>0) ", 60,-0.5,59.5,60,-0.5,59.5);
  fHistSecondParticleAll->GetXaxis()->SetTitle("Number (reco)");
  fHistSecondParticleAll->GetYaxis()->SetTitle("Number (MC)");

  fHistSecondParticleTruthAll= new TH2F("fHistSecondParticleTruthAll", "Number of V0 MCTrue vs number V0 reco (true) (T>0)", 60,-0.5,59.5,60,-0.5,59.5);
  fHistSecondParticleTruthAll->GetXaxis()->SetTitle("Number (reco true)");
  fHistSecondParticleTruthAll->GetYaxis()->SetTitle("Number (MC)");

  fHistSecondParticle= new TH2F("fHistSecondParticle", "Number of V0 MCTrue vs number V0 reco (T>0, V>0)", 60,-0.5,59.5,60,-0.5,59.5);
  fHistSecondParticle->GetXaxis()->SetTitle("Number (reco)");
  fHistSecondParticle->GetYaxis()->SetTitle("Number (MC)");

  fHistSecondParticleTruth= new TH2F("fHistSecondParticleTruth", "Number of V0 MCTrue vs number V0 reco (true) (T>0, V>0)", 60,-0.5,59.5,60,-0.5,59.5);
  fHistSecondParticleTruth->GetXaxis()->SetTitle("Number (reco true)");
  fHistSecondParticleTruth->GetYaxis()->SetTitle("Number (MC)");

  fHistMassvsPt_tagli = new TH2F *[6];

  for(Int_t j=0; j<6; j++){
    fHistMassvsPt_tagli[j] = new TH2F(Form("fHistMassvsPt_" +fV0+ "_tagli_%i",j),Form("fHistMassvsPt_" +fV0+ "_tagli_%i" + " (all selections on V0 applied)",j),400,lLimInfMass,lLimSupMass,160,-16,16);
    fHistMassvsPt_tagli[j]->GetXaxis()->SetTitle("Invariant mass of V0 candidate");
    fHistMassvsPt_tagli[j]->GetYaxis()->SetTitle("p_{T} of V0 candidate");   
  }
  
  fHistMultvsTrigger=new TH2F("fHistMultvsTrigger", "Centrality of selected events (T>0, V>0) vs number of trigger particles", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTrigger->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTrigger->GetYaxis()->SetTitle("Centrality");
  

  fHistMultvsTriggerMCTruth=new TH2F("fHistMultvsTriggerMCTruth", "Centrality of selected events (T>0, V>0) vs number of trigger particles, MC Truth", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTriggerMCTruth->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTriggerMCTruth->GetYaxis()->SetTitle("Centrality");
  

  fHistMultvsTriggerAll=new TH2F("fHistMultvsTriggerAll", "Centrality of events w T>0 vs number of trigger particles", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTriggerAll->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTriggerAll->GetYaxis()->SetTitle("Centrality");
  

  fHistMultvsTriggerMCTruthAll=new TH2F("fHistMultvsTriggerMCTruthAll", "Centrality of events w T>0 vs number of trigger particles, MC Truth", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTriggerMCTruthAll->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTriggerMCTruthAll->GetYaxis()->SetTitle("Centrality");

  fHistMultvsTriggerBefAll=new TH2F("fHistMultvsTriggerBefAll", "Centrality of events vs number of trigger particles", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTriggerBefAll->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTriggerBefAll->GetYaxis()->SetTitle("Centrality");
  
 
  fHistMultvsTriggerMCTruthBefAll=new TH2F("fHistMultvsTriggerMCTruthBefAll", "Centrality of events vs number of trigger particles, MC Truth", 50, -0.5, 49.5, NumBinsMult, 0, UpperLimitMult);
  fHistMultvsTriggerMCTruthBefAll->GetXaxis()->SetTitle("Number of Trigger particles");
  fHistMultvsTriggerMCTruthBefAll->GetYaxis()->SetTitle("Centrality");
  

  fHistMultvsV0=new TH2F("fHistMultvsV0", "Centrality of selected events (T>0, V0>0) vs number of reco V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0->GetXaxis()->SetTitle("Number of reco V0 particles");
  fHistMultvsV0->GetYaxis()->SetTitle("Centrality");

  fHistMultvsV0Truth=new TH2F("fHistMultvsV0Truth", "Centrality of selected events (T>0, V0>0) vs number of reco true V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0Truth->GetXaxis()->SetTitle("Number of reco true V0 particles");
  fHistMultvsV0Truth->GetYaxis()->SetTitle("Centrality");

  fHistMultvsV0MC=new TH2F("fHistMultvsV0MC", "Centrality of selected events (T>0, V0>0) vs number of true V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0MC->GetXaxis()->SetTitle("Number of V0 true particles");
  fHistMultvsV0MC->GetYaxis()->SetTitle("Centrality");

  fHistMultvsV0All=new TH2F("fHistMultvsV0All", "Centrality of events w T>0 vs number of reco V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0All->GetXaxis()->SetTitle("Number of V0 particles");
  fHistMultvsV0All->GetYaxis()->SetTitle("Centrality");

  fHistMultvsV0AllTruth=new TH2F("fHistMultvsV0AllTruth", "Centrality of events w T>0 vs number of reco true V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0AllTruth->GetXaxis()->SetTitle("Number of V0 reco particles");
  fHistMultvsV0AllTruth->GetYaxis()->SetTitle("Centrality");

  fHistMultvsV0MCAll=new TH2F("fHistMultvsV0MCAll", "Centrality of events w T>0 vs number of true V0s",20, -0.5, 19.5,NumBinsMult, 0, UpperLimitMult );
  fHistMultvsV0MCAll->GetXaxis()->SetTitle("Number of V0 true particles");
  fHistMultvsV0MCAll->GetYaxis()->SetTitle("Centrality");

  fHistTriggerNotLeading=new TH3F("fHistTriggerNotLeading", "Events with trigger not leading in all events with NT>0",60, -0.5, 59.5,NumBinsMult, 0, UpperLimitMult, 60, 0, 30 );
  fHistTriggerNotLeading->GetXaxis()->SetTitle("Number of V0 with p_{T}> p_{T} Trigger");
  fHistTriggerNotLeading->GetYaxis()->SetTitle("Multiplicity class");
  fHistTriggerNotLeading->GetZaxis()->SetTitle("p^{Trigg, Max}_{T}");

  fHistTriggerNotLeadingMC=new TH3F("fHistTriggerNotLeadingMC", "Events with trigger not leading in all events with NT>0 (MC Truth)",60, -0.5, 59.5,NumBinsMult, 0, UpperLimitMult, 60, 0, 30 );
  fHistTriggerNotLeadingMC->GetXaxis()->SetTitle("Number of V0 with p_{T}> p_{T} Trigger");
  fHistTriggerNotLeadingMC->GetYaxis()->SetTitle("Multiplicity class");
  fHistTriggerNotLeadingMC->GetZaxis()->SetTitle("p^{Trigg, Max}_{T}");
  
  fHistTrigger=new TH1F("fHistTrigger", "Number of reco trigger particle distribution for selected events (also T=0)", 60, -0.5, 59.5); // each entry is an event

  fHistTriggerwV0=new TH1F("fHistTriggerwV0", "Number of reco trigger particle distribution for events used for AC", 60, -0.5, 59.5); // each entry is an event

  fHistTriggerMCTruth=new TH1F("fHistTriggerMCTruth", "Number of true trigger particle distribution for selected events (also T=0)", 60, -0.5, 59.5); // each entry is an event

  fHistTriggerwV0MCTruth=new TH1F("fHistTriggerwV0MCTruth", "Number of true trigger particle distribution for events used for AC", 60, -0.5, 59.5); // each entry is an event

  fHistMultiplicityVsVertexZ=new TH2F("fHistMultiplicityVsVertexZ", "Centrality vs Z vertex of selected events with NT>0 and NV0>0 ",  20, -10, 10,NumBinsMult, 0, UpperLimitMult);
      
  fHistTriggervsMult=new TH1F("fHistTriggervsMult", "Numero di particelle di trigger nei vari intervalli di centralita'", NumBinsMult, 0, UpperLimitMult);
  fHistTriggervsMult->GetXaxis()->SetTitle("Centrality");

  fHistTriggervsMultMC=new TH1F("fHistTriggervsMultMC", "Numero di particelle di trigger (MCtruth) nei vari intervalli di centralita'", NumBinsMult, 0, UpperLimitMult);
  fHistTriggervsMultMC->GetXaxis()->SetTitle("Centrality");


  fHistPhi= new TH2F ("fHistPhi", "casc->Phi() vs Phi calculated in my way" , 100, 0, 2*TMath::Pi(), 100, 0, 2*TMath::Pi());
  fHistPhi->GetYaxis()->SetTitle("#phi my way");
  fHistPhi->GetXaxis()->SetTitle("casc->Phi");

  fHistTheta= new TH2F ("fHistTheta", "casc->Theta() vs Theta calculated in my way" , 100,0, TMath::Pi(), 100, 0, TMath::Pi());
  fHistTheta->GetYaxis()->SetTitle("#theta my way");
  fHistTheta->GetXaxis()->SetTitle("casc->Theta");

  fHistEta= new TH2F ("fHistEta", "casc->Eta() vs Eta calculated in my way" , 100, -1, 1, 100, -1, 1);
  fHistEta->GetYaxis()->SetTitle("#eta my way");
  fHistEta->GetXaxis()->SetTitle("casc->Eta");


  fHistMultiplicityOfMixedEvent=new TH2F("fHistMultiplicityOfMixedEvent", "Distribution of number of events used for the mixing", 100, 0.5, 100.5, NumBinsMult, 0, UpperLimitMult);

  fEventCuts.AddQAplotsToList(fOutputList);
  
  fOutputList->Add(fHistPDG);
  fOutputList->Add(fHistTrackBufferOverflow);
  fOutputList->Add(fHistEventMult);
  fOutputList->Add(fHistTriggerFractionNum);
  fOutputList->Add(fHistTriggerFractionDenom);
  fOutputList->Add(fHistEventV0);
  fOutputList->Add(fHistTrack); 
  fOutputList->Add(fHistLengthvsCrossedRowsAfterSel);
  fOutputList->Add(fHistLengthvsCrossedRows);
  fOutputList->Add(fHistIsCommonParton);
  fOutputList->Add(fHistCommonParton);
  fOutputList->Add(fHistCommonPartonTrueCasc);
  fOutputList->Add(fHistAllGenParticleOrigin);
  fOutputList->Add(fHistAllGenParticleMOrigin);
  fOutputList->Add(fHistAllGenParticleGMOrigin);
  fOutputList->Add(fHistTriggerComposition); 
  fOutputList->Add(fHistTriggerCompositionMCTruth); 
  
  //istogrammi riempiti ad ogni evento selezionato (ossia utilizzato per AC) con una entry
  fOutputList->Add(fHistZvertex);
  fOutputList->Add(fHistNumberChargedAllEvents);
  fOutputList->Add(fHistNumberChargedTrigger);
  fOutputList->Add(fHistNumberChargedNoTrigger);
  fOutputList->Add(fHist_multiplicityAllSelEvents);
  fOutputList->Add(fHist_multiplicity); 
  fOutputList->Add(fHist_multiplicity_EvwTrigger);
  fOutputList->Add(fHistMultiplicityVsVertexZ);
  //  fOutputList->Add(fHistFractionSharedTPCClusters);
  //  fOutputList->Add(fHistGoldenCut);
  fOutputList->Add(fHistMultvsTriggerBefAll);
  fOutputList->Add(fHistMultvsTriggerMCTruthBefAll);
  fOutputList->Add(fHistMultvsTriggerAll);
  fOutputList->Add(fHistMultvsTriggerMCTruthAll);
  fOutputList->Add(fHistMultvsTrigger);
  fOutputList->Add(fHistMultvsTriggerMCTruth);
  fOutputList->Add(fHistMultvsV0All);
  fOutputList->Add(fHistMultvsV0AllTruth);
  fOutputList->Add(fHistMultvsV0MCAll);
  fOutputList->Add(fHistMultvsV0);
  fOutputList->Add(fHistMultvsV0Truth);
  fOutputList->Add(fHistMultvsV0MC);
  fOutputList->Add(fHistTriggerNotLeading);
  fOutputList->Add(fHistTriggerNotLeadingMC);
  fOutputList->Add(fHistSecondParticleAll);
  fOutputList->Add(fHistSecondParticleTruthAll);
  fOutputList->Add(fHistSecondParticle);  
  fOutputList->Add(fHistSecondParticleTruth);
  fOutputList->Add(fHistDCAxym1);     
  fOutputList->Add(fHistDCAxym2);    
  fOutputList->Add(fHistDCAzm1);      
  fOutputList->Add(fHistDCAzm2);     
  fOutputList->Add(fHistPt);   
  fOutputList->Add(fHistPtV0);     
  fOutputList->Add(fHistPtTMaxBefAllCfrDataMC);
  fOutputList->Add(fHistPtTMinBefAll);     
  fOutputList->Add(fHistPtTMinBefAllMC);     
  fOutputList->Add(fHistPtMaxvsMultBefRSelection);
  fOutputList->Add(fHistPtMaxvsMultAfterRSelection);
  fOutputList->Add(fHistPtvsMult);       
  fOutputList->Add(fHistPtvsMultBefAll);       
  fOutputList->Add(fHistPtMaxvsMult);       
  fOutputList->Add(fHistPtMaxvsMultSkipV0);       
  fOutputList->Add(fHistPtMaxvsMultKeepV0);       
  fOutputList->Add(fHistPtMaxvsMultBefAll);       
  fOutputList->Add(fHistPtMaxvsMultBefAllReco);       
  fOutputList->Add(fHistPtMaxvsMultBefAllGen);       
  fOutputList->Add(fHist_eta_phi);
  fOutputList->Add(fHist_eta_phi_PtMax);
  fOutputList->Add(fHistTriggervsMult);
  fOutputList->Add(fHistTriggervsMultMC);
  fOutputList->Add(fHistTrigger);
  fOutputList->Add(fHistTriggerwV0);
  fOutputList->Add(fHistTriggerMCTruth);
  fOutputList->Add(fHistTriggerwV0MCTruth);
  fOutputList->Add(fMassV0);


  for(Int_t j=0; j < 6; j++){
    fOutputList->Add(fHistMassvsPt_tagli[j]);    
  } 
  
  fOutputList->Add(fHistMultiplicityOfMixedEvent);
   

  PostData(1, fOutputList);  
  PostData(2, fSignalTree);       
  PostData(3, fBkgTree); 
  PostData(4, fOutputList2);     
  PostData(5, fOutputList3);     
  PostData(6, fOutputList4);     
     
}
//_____________________________________________________________________________
void AliAnalysisTaskCorrelationhCascDATA::UserExec(Option_t *)
{

  Float_t moltep[6]={0,5,10,30,50,100};  
  Float_t moltepHM[6]={0,0.001, 0.005, 0.01, 0.05, 0.1};  //V0M multiplicity intervals                          
  if (fisHM){
    for (Int_t i=0; i<=5; i++){
      moltep[i] = moltepHM[i];
    }
  }

  Float_t LastzBin;
  Float_t LastcentralityBin;

  fHistEventMult->Fill(23);   
  fAOD = dynamic_cast<AliAODEvent*>(InputEvent());  
  if(!fAOD) {
    AliWarning("Error: AOD event not available \n");
    PostData(1, fOutputList);       
    PostData(2, fSignalTree);       
    PostData(3,fBkgTree); 
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);     
    return;
  }        
  fHistEventMult->Fill(24);
  
  // fEventCuts.SetManualMode(true);
  // fEventCuts.SetupRun2pp();

  if(fisHM == kTRUE){ //modify trigger in event selection                                                      
    fEventCuts.OverrideAutomaticTriggerSelection(AliVEvent::kHighMultV0);
    //fEventCuts.OverrideAutomaticTriggerSelection(AliVEvent::kAnyINT);                                        
  }

  /// Use the event cut class to apply the required selections
  if (!fEventCuts.AcceptEvent(fAOD)) {   
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);     
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);     
    return;
  }


  fHistEventMult->Fill(1);
  
  Int_t iTracks(fAOD->GetNumberOfTracks());         
  Int_t iCascades(fAOD->GetNumberOfCascades());     
  Double_t b = fAOD->GetMagneticField(); 
  Evcounter++;  
  // cout << "\n \n \n ********************************************************* "<< endl;
  // cout << "event number "<<  Evcounter << endl; 
  // cout << "number of tracks before any cut " << iTracks << endl;
  // cout << "number of V0 before any cut " << V0Tracks << endl;
  
  //VERTEX SELECTION AND TRIGGER
  Double_t lBestPrimaryVtxPos[3] = {-100.0, -100.0, -100.0};
  const AliAODVertex *lPrimaryBestAODVtx = fAOD->GetPrimaryVertex();
  if (!lPrimaryBestAODVtx){
    AliWarning("No prim. vertex in AOD... return!");
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree); 
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);  
    PostData(6, fOutputList4);     
    return;
  }
  fHistEventMult->Fill(2);


  AliVVertex *vertexmain =0x0;
  vertexmain = (AliVVertex*) lPrimaryBestAODVtx;
  lPrimaryBestAODVtx->GetXYZ(lBestPrimaryVtxPos);

  if (TMath::Abs(lBestPrimaryVtxPos[2])>10.){
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3, fBkgTree); 
    PostData(4, fOutputList2); 
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);     
    return;
  }
  fHistEventMult->Fill(3);

  //PID RESPONSE
  AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
  if (inputHandler)   fPIDResponse = inputHandler->GetPIDResponse();
  UInt_t mask = inputHandler->IsEventSelected();

  if (!fPIDResponse){
    AliWarning("cannot get pid response");
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree); 
    PostData(4, fOutputList2); 
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);     
    return;
  }
  fHistEventMult->Fill(4);

  Float_t lPercentiles = 0;
 
  
  
  //This will work for both ESDs and AODs
  AliMultSelection *MultSelection = (AliMultSelection*) fAOD -> FindListObject("MultSelection");
  
  if ( MultSelection ){
    //c cout << "mult sel ok" << endl;
    lPercentiles= MultSelection->GetMultiplicityPercentile("V0M");
    //  cout << lPercentiles << endl;
  }else{
    AliInfo("Didn't find MultSelection!"); 
  }
  
        
  if ( lPercentiles > 199 ){
    PostData(1,fOutputList );
    PostData(2, fSignalTree );
    PostData(3,fBkgTree); 
    PostData(4, fOutputList2); 
    PostData(5, fOutputList3);
    PostData(6, fOutputList4);     
    return;  
  }

  if (fisHM && lPercentiles > 0.1){
    PostData(1,fOutputList );
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);
    PostData(5, fOutputList3);
    PostData(6, fOutputList4);
    return;
  }


  fHistEventMult->Fill(5);

  //event must not be tagged as pileup
  Bool_t isPileUpSpd=kFALSE;
  isPileUpSpd=fAOD->IsPileupFromSPD();
  if(isPileUpSpd){ 
    PostData(1,fOutputList );
    PostData(2, fSignalTree );
    PostData(3,fBkgTree); 
    PostData(4, fOutputList2); 
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);     
    return;
  }
  fHistEventMult->Fill(6);

  Bool_t isSelectedInt7        = kFALSE;
  Bool_t isSelectedAny         = kFALSE;
  Bool_t isSelected            = kFALSE;
  Bool_t isSelectedMB          = kFALSE;
  Bool_t isSelectedHM          = kFALSE;
 
  if(fCollidingSystem == "pp"){
    isSelectedInt7        = (mask & AliVEvent::kINT7);
    isSelectedAny         = (mask & AliVEvent::kAnyINT);
    isSelectedMB          = (mask & AliVEvent::kMB);
    isSelectedHM          = (mask & AliVEvent::kHighMultV0);
    
    if(isSelectedInt7 ) isSelected = kTRUE;
    if(fYear == 2010 && isSelectedMB) isSelected = kTRUE;
    if (fisHM){
      if(isSelectedHM) isSelected=kTRUE;
      else isSelected=kFALSE;
    }
  }
  else if(fCollidingSystem == "pPb"){
    isSelectedInt7        = (mask & AliVEvent::kINT7);
    isSelectedAny         = (mask & AliVEvent::kAny);

    if(isSelectedInt7 )
      isSelected = kTRUE;
  }
  
  if(isSelectedInt7 && !fisHM)
    fHistEventMult->Fill(7);
  if(isSelectedHM && fisHM)
    fHistEventMult->Fill(7);

 //to select INEL>0 events for inclusive analysis
  const AliMultiplicity* mult = (AliMultiplicity*)fAOD->GetMultiplicity();
  Bool_t inelgt0 = kFALSE;
  Int_t       counterTracklets =0;
  for (Int_t i = 0; i < mult->GetNumberOfTracklets(); ++i) {
    if (TMath::Abs(mult->GetEta(i)) < 1.) {
      inelgt0 = kTRUE;
      counterTracklets++;
    }
  }
   
  if (inelgt0)  fHistEventMult->Fill(8) ;
  if (fisInclusiveINELgt0 && !inelgt0){
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);
    PostData(5, fOutputList3);
    PostData(6, fOutputList4);
    return;
  }

  if (isSelected)
    fHistEventMult->Fill(9) ; 
   
  if(!isSelected){   
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);
    // cout << "event does not fulfil centrality selection criteria " << endl;      
    return;
  }
  
  if (lPercentiles < lPercentilesMin || lPercentiles > lPercentilesMax){
    PostData(1, fOutputList );
    PostData(2, fSignalTree );
    PostData(3, fBkgTree);
    PostData(4, fOutputList2);
    PostData(5, fOutputList3);
    PostData(6, fOutputList4);
    return;
  }

  fHist_multiplicityAllSelEvents->Fill(lPercentiles);
  //  cout << "event has passed selection criteria.... first and second particles to be analyzed ...."<< endl;

  if (lPercentiles<5)fHistTriggerFractionDenom->Fill(1);
  else  if (lPercentiles<10)fHistTriggerFractionDenom->Fill(2);
  else  if (lPercentiles<30)fHistTriggerFractionDenom->Fill(3);
  else  if (lPercentiles<50)fHistTriggerFractionDenom->Fill(4);
  else  fHistTriggerFractionDenom->Fill(5);

  Bool_t Generated=kTRUE; //TRUE if generated particles are analyzed
  Int_t labelPrimOrSec=0; 
  Int_t label = 0; //track label
  Float_t TrackLength=-999;
  AliAODTrack* track=0x0;
  AliAODTrack* trackPtTMax=0x0;
  AliAODTrack* globaltrackPtTMax=0x0;
  AliAODTrack *globaltrack = 0x0;
  Bool_t isV0=kFALSE;

  //Collect events according to their centrality and z coordinate of PV
  const Float_t bfield = (InputEvent())->GetMagneticField();
  int fieldsign;
  if (bfield >=0.) fieldsign = 1;
  else fieldsign = -1;
 
  int zBin=0;
  int centralityBin=0;
    
  double zStep=2*10/double(fzVertexBins), zStart=-10.;
    
  for (int i=0; i<fzVertexBins; i++) {
    if ((lBestPrimaryVtxPos[2] > zStart+i*zStep) && (lBestPrimaryVtxPos[2] < zStart+(i+1)*zStep)) {
      zBin=i;
      break;
    }
  } 
  if (fisHM){
    if(lPercentiles < 0.001) centralityBin=19; 
    else if(lPercentiles < 0.005) centralityBin=18;
    else if(lPercentiles < 0.01) centralityBin=17;
    else if(lPercentiles < 0.05) centralityBin=16;
    else if(lPercentiles < 0.1) centralityBin=15;
    else centralityBin = 14;
  }
  else{
    if(lPercentiles < 5.0) centralityBin=19;  // changed <= with < to be consistent with histogram binning, except last bin 
    else if(lPercentiles < 10.0) centralityBin=18;
    else if(lPercentiles < 30.0) centralityBin=17;
    else if(lPercentiles < 50.) centralityBin=16;
    else if(lPercentiles <= 100.) centralityBin=15;
  }

  if (((centralityBin+1) >fnMultBins) || ((zBin+1) > fzVertexBins)){ 
    //c cout<<" ##################  WARNING: I'm going to break bacause of dimensional issues ########################"<<endl;
  }

  fEventColl[zBin][centralityBin]->FifoClear();
  fEvt = fEventColl[zBin][centralityBin]->fEvt;

  Int_t ParticleType=-999;
  if(fV0=="Xi") ParticleType =0;
  else  if(fV0=="Omega") ParticleType=1;
  else {
    //    cout << " the particle type selected is not valid " << endl;
    return;
  }


  //*****************create a global track to get the correct DCA*******************************
  for (Int_t igt = 0; igt < fTrackBufferSize; igt++) farrGT[igt] = -1;
 
  if (fFilterBitValue==128 || fFilterBitValue==512){
    // Read and store global tracks to retrieve PID information for TPC only tracks
    for (Int_t igt = 0; igt < iTracks; igt++) {
      globaltrack = (AliAODTrack*) fAOD->GetTrack(igt);

      if (!globaltrack) continue; 
      if (globaltrack->GetID()<0 ) continue;
      if (!globaltrack->IsOn(AliAODTrack::kTPCrefit)) continue; // there are such tracks with no TPC clusters

      // Check id is not too big for buffer
      if (globaltrack->GetID()>=fTrackBufferSize) {
	printf("Warning: track ID too big for buffer: ID: %d, buffer %d\n",globaltrack->GetID(),fTrackBufferSize);
	fHistTrackBufferOverflow->Fill(1);
	continue;
      }

      //    if ( !(globaltrack->GetFilterMap()) ) { 
      //        cout<<" No filter map for this global track!!  "<<globaltrack->GetFilterMap()<<endl;
      //        continue;
      //    }

      if ( globaltrack->GetTPCNcls()<=0   ) { // such tracks do not have the TPC refit either, filter map is 2 --> ITS constrained
	//      cout<<" No TPC cl for this global track!!  "<<igt<<endl;
	//      if (!globaltrack->IsOn(AliAODTrack::kTPCrefit)) cout<<" ... and also no tpc refit  "<<globaltrack->GetFilterMap()<<endl;
	continue;
      }
      //cout<<" The array will contain "<<igt<<" , it contains "<<farrGT[globaltrack->GetID()]<<endl; 

      // Warn if we overwrite a track
      if (farrGT[globaltrack->GetID()]>=0) { // Two tracks same id  --> checked that it never happens
	//      cout<<" Array already filled "<<farrGT[globaltrack->GetID()]<<endl;
      } else { 
	farrGT[globaltrack->GetID()] = igt;           // solution adopted in the femto framework
	//    cout<<" Set array, now it contains  "<<farrGT[globaltrack->GetID()]<<endl;
      }
    }
  } //end if on filterbit value
  globaltrack = 0x0; 

  //***********************************************************************************
  //-----------------------------------LOOP OVER THE TRACKS
  //***********************************************************************************
  AliVTrack *vtrackg = 0x0;
  AliVTrack *vtrack = 0x0;
  AliVTrack *vtrackgPos = 0x0;
  AliVTrack *vtrackgNeg = 0x0;
  AliExternalTrackParam etp1;
  AliExternalTrackParam etpPos;
  AliExternalTrackParam etpNeg;

  Float_t nTPCCrossedRows=0.;
  float rationCrnFind=0;
  Float_t  FractionSharedTPCCluster=0;
  Float_t nsigmaTOFj=3;
  Float_t nsigmaTPCj=3;
  Int_t charge=0;
  Int_t NumberCharged=0;
  Int_t NumberFirstParticleAll=0;
  Int_t NumberFirstParticle=0;
  Int_t NumberFirstParticleAllPt=0;
  Int_t NumberFirstParticleAllPtMC=0;
  Int_t NumberFirstParticleMC=0;
  Int_t NumberFirstParticle_finale=0;
  Int_t NumberSecondParticleRecoTrue=0;
  Int_t NumberSecondParticle=0;
  Int_t NumberSecondParticleMC=0;
  Int_t NumberSecondParticleAll=0;
  Int_t NumberSecondParticleMCNoAssoc=0;
  Int_t NumberSecondParticleNoAssoc=0;
  Int_t NumberSecondParticleTrueNoAssoc=0;
  Double_t Ptintermediate=0;
  Double_t selectedtrackID=0;
  Int_t pos0or1=0;
  Int_t neg0or1=0;
  Int_t CharegFirstParticle=0;
  Double_t dzglobal[2] = {-999.,-999.};
  Double_t dz[2] = {-999.,-999.};
  Double_t d[2] = {-999.,-999.};
  Double_t dzg[2]= {-999.,-999.}; Double_t covarg[3]={-999.,-999.,-999.};
  Double_t dzgPos[2]= {-999.,-999.}; Double_t covargPos[3]={-999.,-999.,-999.};
  Double_t dzgNeg[2]= {-999.,-999.}; Double_t covargNeg[3]={-999.,-999.,-999.};
  Double_t dzgPtTMax=-999;
  Float_t DCAxy=-999.;
  Float_t DCAz=-999.;
  Float_t labelPtTMax=0;
  Float_t ptTriggerMinimoDati=10000;
  Double_t ptTriggerMassimoDati=0;
  Double_t ptTriggerMassimoAll=0;
  Double_t ptTriggerMassimoDatiBis=0;
  Float_t etaTriggerMassimoDati=0;
  Float_t phiTriggerMassimoDati=0;
  Int_t   PdgCodeTrackPtMax=0;
  Int_t TriggerPdgCode = 0;
  //begin loop for trigger particles   
  if (!(fReadMCTruth && !isEfficiency && !isHybridMCTruth)){
    for(Int_t i=0; i < iTracks; i++) {
      if (fisInclusiveINELgt0){
        track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
        trackPtTMax = static_cast<AliAODTrack*>(fAOD->GetTrack(i));
        NumberFirstParticleAllPt++;
        NumberFirstParticle++;
        if (track && trackPtTMax) break;
        if (!track) continue;
        if (!trackPtTMax) continue;
      }
      else {
	track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));        
	fHistTrack->Fill(1);
	if(!track) continue;

	//to know if trigger particle is primary, secondary,...
	if(fReadMCTruth){
	  if (fMCEvent){
	    TClonesArray* AODMCTrackArrayTrigg =0x0;  
	    AODMCTrackArrayTrigg = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
	    if (AODMCTrackArrayTrigg == NULL){
	      return;
	      Printf("ERROR: stack not available");
	    }
      
	    AliAODMCParticle* particleTrig = static_cast<AliAODMCParticle*>(AODMCTrackArrayTrigg->At(TMath::Abs(track->GetLabel())));
	    if (!particleTrig) continue;
	    if(particleTrig->IsPhysicalPrimary())     labelPrimOrSec=1;
	    else if(particleTrig->IsSecondaryFromWeakDecay())      labelPrimOrSec=2;
	    else if(particleTrig->IsSecondaryFromMaterial())      labelPrimOrSec=3;
	    else labelPrimOrSec=4;
	    TriggerPdgCode = particleTrig->GetPdgCode();

	  }
	}
  
	if(!track->TestFilterBit(fFilterBitValue)) continue; 
	fHistTrack->Fill(2);
    
	if(TMath::Abs(track->Eta())>fEtaTrigger)  continue;
	fHistTrack->Fill(3);

	NumberCharged++; //all charged particles
    
	if(track->Chi2perNDF()>4.)continue;
	fHistTrack->Fill(4);
    
	nTPCCrossedRows=track->GetTPCNCrossedRows();
	if(nTPCCrossedRows<80) continue;
	fHistTrack->Fill(5); 
        
	rationCrnFind=nTPCCrossedRows/track->GetTPCNclsF();
	if(rationCrnFind<0.8)  continue;
	fHistTrack->Fill(6); 

	Float_t lTrackLength = -1;
	Float_t lTrackLengthBis = -1;
	lTrackLength = GetLengthInActiveZone( track, /*1,*/ 2.0, 220.0, fAOD->GetMagneticField());
	lTrackLengthBis = GetLengthInActiveZone( track, /*1,*/ 0., 250.0, fAOD->GetMagneticField()); //test value

	Float_t     CrossedRowsOverLengthBis=    track->GetTPCClusterInfo(2,1)/lTrackLength;
	Float_t     CrossedRowsOverLength=(Float_t)nTPCCrossedRows/lTrackLength; //->both work in the same way

	fHistLengthvsCrossedRows ->Fill(  (Float_t)nTPCCrossedRows, lTrackLength );

	if (lTrackLength<90) continue;
	fHistTrack->Fill(7);  

	if (CrossedRowsOverLength< 0.8) continue;
	fHistTrack->Fill(8); 
	fHistLengthvsCrossedRowsAfterSel ->Fill(  (Float_t)nTPCCrossedRows, lTrackLength);

	//cluster selection (not applied)
	Int_t nClustersTPC = -1;
	//if(fCutRequireTPCStandAlone) {
	// nClustersTPC = track->GetTPCNclsIter1();
	//  }
	//  else {
	nClustersTPC = track->GetTPCncls(0);
	//}
	Int_t nClustersTPCShared = track->GetTPCnclsS();
	Float_t fracClustersTPCShared = -1.;
	if (nClustersTPC!=0) {
	  fracClustersTPCShared = Float_t(nClustersTPCShared)/Float_t(nClustersTPC);
	}
	//    fHistFractionSharedTPCClusters->Fill(fracClustersTPCShared);
	//  if (fracClustersTPCShared > 0.2) continue;
	//    fHistTrack->Fill(9);


	//Golden cut (not applied)
	//    fHistGoldenCut->Fill(track->GetChi2TPCConstrainedVsGlobal());
	//  if ((track->GetChi2TPCConstrainedVsGlobal())>36) continue;
	//    fHistTrack->Fill(7);

	if((track->Charge())==0) continue;
	fHistTrack->Fill(9);
	  
	if (fFilterBitValue==128 || fFilterBitValue==512){ 
	  //!!!!!!!!!information on DCA is correct only if taken in this way for tracks with FB128!!!!!!!!
	  if (-track->GetID()-1 >= fTrackBufferSize) {
	    printf ("Exceeding buffer size!!");
	    continue;
	  }

	  vtrackg = fAOD->GetTrack(farrGT[-track->GetID()-1]);
	  if (!vtrackg) {
	    printf ("No global info! iTrack %d, ID %d\n",i,track->GetID());
	    continue;
	  }
	  if (farrGT[-track->GetID()-1]>=iTracks || farrGT[-track->GetID()-1]<0) { /*cout<<"This index is out of range!!"<<farrGT[-track->GetID()-1]<<endl;*/ continue;}
	  globaltrack = dynamic_cast<AliAODTrack*>(vtrackg); 
	  if(!globaltrack) AliFatal("Not a standard AOD");

	  dzg[0]= -999.; dzg[1] = -999.;
	  etp1.CopyFromVTrack(vtrackg); 
	  etp1.PropagateToDCA(vertexmain,(InputEvent())->GetMagneticField(),100.,dzg,covarg);    
	  //      cout << "Propagate to DCA method : DCAxy " << dzg[0] << " DCAz " << dzg[1] << endl;

	  /* does not seem to work properly 
	     vtrackg->GetImpactParameters(&DCAxy, &DCAz);
	     dz[0] = DCAxy;   
	     dz[1] = DCAz; 
	     cout << "GetImpactParamter method : DCAxy " << dz[0] << " DCAz " << dz[1] << endl;
	  */
	
	  /* The following method gives equal results to propagate to DCA     
	     Float_t fPosition[2]={-999.};
	     static_cast<AliAODTrack*>(fAOD->GetTrack(farrGT[-track->GetID()-1]))->GetPosition(fPosition);
	     cout << "z position pos " << fPosition[2] << " z position primary vertex " << lBestPrimaryVtxPos[2] << endl;
	     cout << "y position pos " << fPosition[1] << " y position primary vertex " << lBestPrimaryVtxPos[1] << endl;
	     cout << "x position pos " << fPosition[0] << " x position primary vertex " << lBestPrimaryVtxPos[0] << endl;
	     cout << fPosition[2]- lBestPrimaryVtxPos[2] << endl; //this value is = to dzg[1]
	     cout << sqrt(pow (fPosition[0]- lBestPrimaryVtxPos[0],2) + pow(fPosition[1]- lBestPrimaryVtxPos[1],2)) << endl; //this value is = to dzg[0]
	  */
	} //end if on filterbit value

	//    cout << "DCAxy trigger particle " << dzg[0] << " DCAz trigger particle " << dzg[1] << endl;	  

	if (fFilterBitValue!=128 && fFilterBitValue!=512){

	  /* does not seem to work properly
	     DCAxy=-999.;      DCAz =-999.;
	     track->GetImpactParameters(&DCAxy, &DCAz);
	     dz[0] = DCAxy;   
	     dz[1] = DCAz; 
	     cout << "GetImpactParamter method : DCAxy " << dz[0] << " DCAz " << dz[1] << endl;
	  */

	  //It seems that for FB 256 the two following methods (my method and propagate t DCA) give the same results (which seem reasonable: both DCAz and DCA yx are < 0.2 and have the correct shape), while the previous method (GetImpactParameter) give smaller results. 
	  dzg[0]= -999.; dzg[1] = -999.;
	  etp1.CopyFromVTrack(track); 
	  etp1.PropagateToDCA(vertexmain,(InputEvent())->GetMagneticField(),100.,dzg,covarg);    
	  //      cout << "Propagate to DCA method : DCAxy " << dzg[0] << " DCAz " << dzg[1] << endl;

	  /*      Float_t fPosition[2]={-999.};
		  track->GetPosition(fPosition);
		  cout << "z position pos " << fPosition[2] << " z position primary vertex " << lBestPrimaryVtxPos[2] << endl;
		  cout << "y position pos " << fPosition[1] << " y position primary vertex " << lBestPrimaryVtxPos[1] << endl;
		  cout << "x position pos " << fPosition[0] << " x position primary vertex " << lBestPrimaryVtxPos[0] << endl;
		  cout << "DCAz my method " << fPosition[2]- lBestPrimaryVtxPos[2] << endl; //this value is = to dzg[1]
		  cout << "|DCAxy| my method " << sqrt(pow (fPosition[0]- lBestPrimaryVtxPos[0],2) + pow(fPosition[1]- lBestPrimaryVtxPos[1],2)) << endl; //this value is = to dzg[0]
	  */
	}

	//    fHistDCAxym1->Fill(dz[0]);
	//    fHistDCAzm1->Fill(dz[1]);

	fHistDCAxym2->Fill(dzg[0]);
	fHistDCAzm2 ->Fill(dzg[1]);

	//it seems that Propagate to DCA give the best results for all FB:
	//    if (fFilterBitValue==128){
	dzglobal[0]=dzg[0];
	dzglobal[1]=dzg[1];
	/*
	  }
	  else{
	  dzglobal[0]=dz[0];
	  dzglobal[1]=dz[1];
	  }
	*/

	if(TMath::Abs(dzglobal[0])> (0.0105 + 0.0350/pow(track->Pt(),1.1))) continue;
	fHistTrack->Fill(10);
	if(TMath::Abs(dzglobal[1])> 0.04) continue;
	fHistTrack->Fill(11);

	NumberFirstParticleAllPt++; 

	if(track->Pt()< ptTriggerMinimoDati) ptTriggerMinimoDati=track->Pt(); 
	if(track->Pt()> ptTriggerMassimoDati){
	  ptTriggerMassimoDati =track->Pt(); 
	  etaTriggerMassimoDati=track->Eta(); 
	  phiTriggerMassimoDati=track->Phi(); 
	  labelPtTMax=track->GetLabel();
	  dzgPtTMax = dzglobal[1];
	  trackPtTMax = static_cast<AliAODTrack*>(fAOD->GetTrack(i));        
	  globaltrackPtTMax = dynamic_cast<AliAODTrack*>(vtrackg); 
	  ptTriggerMassimoDatiBis=	trackPtTMax->Pt();
	  PdgCodeTrackPtMax = labelPrimOrSec; 
	}

	if (track->Pt()>Ptintermediate){
	  Ptintermediate=track->Pt();
	  selectedtrackID= track->GetID();
	  NumberFirstParticle_finale= NumberFirstParticle;     
	}
      }
      if(track->Pt()> fminPtj && track->Pt()<fmaxPtj){ //Here I select trigger particle with pT>fminPtj and save their characteristics
	if (!fisInclusiveINELgt0) 	NumberFirstParticle++;   
	if((!fReadMCTruth)|| (fReadMCTruth && isEfficiency) || (fReadMCTruth && isHybridMCTruth) ){
	  //save first particle information (leading particle)
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fCharge       = track->Charge();
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fPt           = track->Pt();
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fEta          = track->Eta();
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fPhi          = track->Phi();
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fTheta        = track->Theta();
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fDCAz         = dzglobal[1];
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fDCAxy        = dzglobal[0];
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fMultiplicity = lPercentiles;
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fZvertex      = lBestPrimaryVtxPos[2];
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].isP           = labelPrimOrSec;
	  fEvt->fReconstructedFirst[NumberFirstParticle-1].fPDGcode      = TriggerPdgCode;
	  //	fEvt->fReconstructedFirst[NumberFirstParticle-1].fLabel        = track->GetLabel();
	}
	fHistPtvsMultBefAll->Fill(track->Pt(), lPercentiles);
      }

    }//end loop for trigger particles

    if(fReadMCTruth && isHybridMCTruth){
      if (fMCEvent){
	TClonesArray* AODMCTrackArrayTrigger =0x0;
	AODMCTrackArrayTrigger = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
	AliAODMCParticle* particleTrigger = static_cast<AliAODMCParticle*>(AODMCTrackArrayTrigger->At(TMath::Abs(labelPtTMax)));
	Int_t labelTriggerMother = particleTrigger->GetMother();
	AliAODMCParticle* particleTriggerMother = static_cast<AliAODMCParticle*>(AODMCTrackArrayTrigger->At(TMath::Abs(labelTriggerMother)));
	Int_t labelTriggerGMother = particleTriggerMother->GetMother();
	AliAODMCParticle* particleTriggerGMother = static_cast<AliAODMCParticle*>(AODMCTrackArrayTrigger->At(TMath::Abs(labelTriggerGMother)));

	Bool_t isBachelor=TMath::Abs(particleTriggerMother->GetPdgCode()) == 3312;
	Bool_t isDaughterLambda= TMath::Abs(particleTriggerGMother->GetPdgCode()) == 3312  && TMath::Abs(particleTriggerMother->GetPdgCode()) == 3122; 
	Bool_t Condition2OnTrigger = (!(particleTrigger->IsPhysicalPrimary()) && (isBachelor || isDaughterLambda));

	//	cout << "pdg mother " << particleTriggerMother->GetPdgCode() << " pdg gmother " << particleTriggerGMother->GetPdgCode() << endl;
	//remove trigger particles (and therefore events) where the trigger is the daugther of a K0s              
	fHistPtMaxvsMultBefRSelection->Fill(ptTriggerMassimoDati, lPercentiles);
	if (Condition2OnTrigger){
	  PostData(1, fOutputList);
	  PostData(2, fSignalTree );
	  PostData(3, fBkgTree);
	  PostData(4, fOutputList2);
	  PostData(5, fOutputList3);
	  PostData(6, fOutputList4);
	  // cout  << "event does not have Trigger particles " << endl;                                      
	  return;
	}
	fHistPtMaxvsMultAfterRSelection->Fill(ptTriggerMassimoDati, lPercentiles);
      }
    }
  }

  TClonesArray* AODMCTrackArray =0x0;  
  Float_t ptTriggerMinimoMC=10000;
  Double_t ptTriggerMassimoMC=0;
  Double_t etaTriggerMassimoMC=0;
  Double_t phiTriggerMassimoMC=0;
  TriggerPdgCode=0;
  
  //begin loop for trigger particles (MC truth analysis)
  if(fReadMCTruth){
    if (fMCEvent){
      AODMCTrackArray = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
      if (AODMCTrackArray == NULL){
	return;
	Printf("ERROR: stack not available");
      }
      for(Long_t i = 0; i < AODMCTrackArray->GetEntriesFast(); i++) {
       
	AliAODMCParticle* trParticle = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(i));
       
	if (!trParticle) continue;       
	if((trParticle->Charge())==0)continue;
	if(TMath::Abs(trParticle->Eta())>fEtaTrigger)continue; //I need to select particles within this eta range!
	if (!(trParticle->IsPhysicalPrimary()))continue; 
	if (!(TMath::Abs(trParticle ->GetPdgCode()) == 211 || TMath::Abs(trParticle ->GetPdgCode()) == 321|| TMath::Abs(trParticle ->GetPdgCode()) ==2212 || TMath::Abs(trParticle ->GetPdgCode()) == 11|| TMath::Abs(trParticle ->GetPdgCode()) == 13)) continue;
	NumberFirstParticleAllPtMC++;
	if(trParticle->Pt()<= fminPtj || trParticle->Pt()>=fmaxPtj)continue;
	if(trParticle->Pt()< ptTriggerMinimoMC) ptTriggerMinimoMC=trParticle->Pt();
	if(trParticle->Pt()> ptTriggerMassimoMC){
	  ptTriggerMassimoMC=trParticle->Pt();
	  etaTriggerMassimoMC=trParticle->Eta();
	  phiTriggerMassimoMC=trParticle->Phi();
	  TriggerPdgCode =trParticle ->GetPdgCode();
	}
	NumberFirstParticleMC++;
	if (isEfficiency || isHybridMCTruth) continue;
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fCharge       = trParticle->Charge();
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fPt           = trParticle->Pt();
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fEta          = trParticle->Eta();
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fPhi          = trParticle->Phi();
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fTheta        = trParticle->Theta();
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fDCAz         = 0;
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fDCAxy        = 0;
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fMultiplicity = lPercentiles;
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fZvertex      = lBestPrimaryVtxPos[2];
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].isP           = 1;
	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fPDGcode      = trParticle->GetPdgCode();
	//	fEvt->fReconstructedFirst[NumberFirstParticleMC-1].fLabel          = trParticle->GetLabel();
      }
    }
  }   //end loop for trigger particles (MC truth analysis)

  if ((!fReadMCTruth || (fReadMCTruth &&isEfficiency) || (fReadMCTruth && isHybridMCTruth))&&  ptTriggerMassimoDati!=0 )  fHistPtMaxvsMultBefAll->Fill(ptTriggerMassimoDati, lPercentiles);
  if (fReadMCTruth && !isEfficiency &&  !isHybridMCTruth && ptTriggerMassimoMC!=0)  fHistPtMaxvsMultBefAll->Fill(ptTriggerMassimoMC, lPercentiles);  
  if (fisInclusiveINELgt0) fHistPtMaxvsMultBefAll->Fill(0.0001, lPercentiles); //fake pt

  fHistPtTMinBefAll->Fill(ptTriggerMinimoDati);
  fHistPtTMinBefAllMC->Fill(ptTriggerMinimoMC);

  if(fReadMCTruth){ //to determine "event loss"                                                          
    if(ptTriggerMassimoDati!=0)  fHistPtMaxvsMultBefAllReco->Fill(ptTriggerMassimoDati, lPercentiles);
    if(ptTriggerMassimoMC!=0)    fHistPtMaxvsMultBefAllGen ->Fill(ptTriggerMassimoMC, lPercentiles);
  }

  fHistPtTMaxBefAllCfrDataMC->Fill(ptTriggerMassimoDati,ptTriggerMassimoMC);
  fHistTrigger->Fill(NumberFirstParticle);
  fHistTriggerMCTruth->Fill(NumberFirstParticleMC);
  fHistMultvsTriggerBefAll->Fill(NumberFirstParticle, lPercentiles);
  fHistMultvsTriggerMCTruthBefAll->Fill(NumberFirstParticleMC, lPercentiles);

  fHistTrack->AddBinContent(12, NumberFirstParticle);
  fHistTrack->AddBinContent(13, NumberFirstParticleMC);
  if (NumberFirstParticle>0) fHistEventMult->Fill(10);   
  if (NumberFirstParticleMC>0) fHistEventMult->Fill(11);   
  if(NumberFirstParticle>1)fHistEventMult->Fill(12); 
  if(NumberFirstParticleMC>1)fHistEventMult->Fill(13); 
  if(NumberFirstParticleMC<NumberFirstParticle) fHistEventMult->Fill(14); 
  if(NumberFirstParticleMC==0 && NumberFirstParticle==1) fHistEventMult->Fill(15); 
  if(NumberFirstParticleMC!=0 && NumberFirstParticle!=0) fHistEventMult->Fill(16); 

  
  if( (fReadMCTruth && isHybridMCTruth) || (fReadMCTruth && isEfficiency) || (!fReadMCTruth)) {NumberFirstParticleAll=NumberFirstParticle; ptTriggerMassimoAll=ptTriggerMassimoDati;}
  else if (fReadMCTruth && !isEfficiency && !isHybridMCTruth) {NumberFirstParticleAll=NumberFirstParticleMC; ptTriggerMassimoAll=ptTriggerMassimoMC; NumberFirstParticleAllPt = NumberFirstParticleAllPtMC;}
  if(NumberFirstParticleAll==0) fHistNumberChargedNoTrigger->Fill(lPercentiles, NumberCharged,0 );
  if(NumberFirstParticleAll!=0) fHistNumberChargedTrigger->Fill(lPercentiles, NumberCharged,ptTriggerMassimoAll );
  fHistNumberChargedAllEvents->Fill(lPercentiles, NumberCharged,ptTriggerMassimoAll );

  if(NumberFirstParticleAllPt==0){   
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);
    // cout  << "event does not have Trigger particles " << endl;          
    return;
  }
 
  //  cout <<  "labelPrimOrSec before " <<  PdgCodeTrackPtMax << endl;
  //************Filling selected histograms for trigger particle efficiency calculation  ***************
  //only the highest pT trigger particle in each event is used, since this is the definition of trigger particle used in the angular correlation
  //! The selected histograms are filled also by trigger particles with pT< fminPtj, a cut on pT is therefore required in post processing phase
  Generated = kFALSE;     
  isV0=kFALSE;
  Int_t VPdgTrig[50]={0};
  Int_t VParticleTrigLabel[50]={0};

  //***********************************************************************************************************
  //  cout <<  "labelPrimOrSec after" << labelPrimOrSec << endl; 

  if(NumberFirstParticleAll==0){   
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);
    //    cout  << "event does not have Trigger particles with pT> 3 GeV/c " << endl;          
    return;
  }
 
  fHist_multiplicity_EvwTrigger->Fill(lPercentiles);
  fHistTrack->AddBinContent(14, NumberFirstParticle);
  fHistTrack->AddBinContent(15, NumberFirstParticleMC);
  if (fReadMCTruth && !isEfficiency)  fHistTriggerCompositionMCTruth->Fill(TriggerPdgCode,1,ptTriggerMassimoMC);
  if (lPercentiles<5)fHistTriggerFractionNum->Fill(1);
  else  if (lPercentiles<10)fHistTriggerFractionNum->Fill(2);
  else  if (lPercentiles<30)fHistTriggerFractionNum->Fill(3);
  else  if (lPercentiles<50)fHistTriggerFractionNum->Fill(4);
  else  fHistTriggerFractionNum->Fill(5);

  //defining variables for Cascades
  Int_t labelPrimOrSecV0=0;

  AliAODMCParticle* particlePos;
  //TParticle* particlePos;
  AliAODMCParticle* particleNeg;
  AliAODMCParticle* particleBach;
  Int_t PdgPos=0;
  Int_t PdgNeg=0;
  Int_t PdgBach=0;
  Int_t labelMotherPos=0;
  Int_t labelMotherNeg=0;
  Int_t labelGMotherPos=0;
  Int_t labelGMotherNeg=0;
  Int_t labelMotherBach=0;
  Int_t labelGMotherBach=0;
  AliAODMCParticle* MotherPos;
  AliAODMCParticle* MotherNeg;
  AliAODMCParticle* GMotherPos;
  AliAODMCParticle* GMotherNeg;
  AliAODMCParticle* MotherBach;
  AliAODMCParticle* GMotherBach;
  Int_t PdgMotherPos=0;
  Int_t PdgMotherNeg=0;
  Int_t PdgMotherLambda=0;
  Int_t PdgGMotherPos=0;
  Int_t PdgGMotherNeg=0;
  Int_t PdgMotherBach=0;
  Int_t V0PDGCode=0;

  Double_t MassElectron= 0.0005109989461;
  Double_t massLambda = 1.115683;
  Double_t MassPion = 0.13957061;
  Double_t MassProton=0.938272081;
  Double_t MassKaon=0.493677;
  Double_t massXi = 1.32171;
  Double_t massOmega = 1.67245;
  Float_t  NominalMassCasc[2] ={1.32171,1.67245 };
  Float_t ctauCasc[2] = {4.91, 2.461}; //cm , average ctau of Xi and Omega respectively

  Int_t lVariableIsPrimaryXi=0;
  Int_t lVariableIsPrimaryOmega=0;
  Int_t lVariablePDGCode=-999;

  //MC generated V0
  isV0=kTRUE;
  Generated=kTRUE;

  
  //********************************************************************************************
  // cout << "\n \n  here I start the loop on Casc " << endl;

  //selections applied in task (the default ones applied by Fiorella + my selection for V0 lifetime)
  Float_t DcaMesonToPrimVertex=0.04; //cm
  Float_t DcaBaryonToPrimVertex=0.03; //cm
  Float_t DcaV0ToPrimVertex=0.06; //cm
  Float_t DcaV0Daughters=1.5; //sigma
  Float_t DcaBachToPrimVertex=0.04; //cm
  Float_t XiRadius[2] ={0.5, 0.6};
  Float_t V0RadiusXi[2]={1.1, 1.2};
  Float_t V0LifetimeXiPlus =30; //cm
  Float_t V0LifetimeXiMinus=80; //cm
  Float_t CascRejectionWindow[2]= {0.005, 0.008}; //GeV, omega rejection and Xi rejection respectively
  Int_t PDGCodeCasc[2] = {3312, 3334};

  //selections kept loose in the task; the corresponding variables are saved in the tree so tighter selections can be done offline
  Float_t DcaCascDaughters=1.8; //cm //default is 0.8 (optimized)
  Float_t V0CosineOfPointingAngleSpecial=0.95; //default is 0.97
  Float_t CascCosineOfPointingAngle=0.95; //default is 0.995 (optimized)
  Float_t InvMassLambda = 0.009; //GeV //default is 0.006 (optimized)
  Float_t NumberCtau =5; //default is 3 

  Bool_t KeepV0Global=kFALSE;
  Bool_t SkipV0Global=kFALSE;
  Bool_t KeepV0GlobalMC=kFALSE;
  Bool_t SkipV0GlobalMC=kFALSE;

  if (!(fReadMCTruth && !isEfficiency)){
    for (Int_t iXi = 0; iXi < iCascades; iXi++) {
      fHistEventV0->Fill(1);
    

      Double_t lXiCosineOfPointingAngle = -1. ;
      Double_t lPosXi[3] = { -1000.0, -1000.0, -1000.0 };
      Double_t lXiRadius = -1000. ;
      Double_t lXiDecayLength = -1000. ;
      Double_t kctauXi = -1000. ;

      Int_t    lPosTPCClusters    = -1; 
      Int_t    lNegTPCClusters    = -1; 
      Int_t    lBachTPCClusters   = -1; 
   
      Double_t lInvMassLambdaAsCascDghter = 0.;
      Double_t lInvMassK0sAsCascDghter = 0.;

      Double_t lDcaV0DaughtersXi = -1.;
      Double_t lDcaXiDaughters = -1. ;
      Double_t lDcaBachToPrimVertexXi = -1.;
      Double_t lDcaV0ToPrimVertexXi = -1.;
      Double_t lDcaPosToPrimVertexXi  = -1.;
      Double_t lDcaNegToPrimVertexXi  = -1.;
      Double_t lDcaXiToPrimVertex= -1.;

      Double_t lV0CosineOfPointingAngleXi = -1. ;
      Double_t lV0CosineOfPointingAngleXiSpecial = -1. ;
      Double_t lPosV0Xi[3] = { -1000. , -1000., -1000. }; 

      Double_t lV0RadiusXi = -1000.0;

      Double_t lInvMassXi         = 0.;
      Double_t lInvMassOmega      = 0.;

      Double_t lXiMomX = 0. , lXiMomY = 0., lXiMomZ = 0.;
      Double_t lXiTransvMom = 0. ;
      Double_t lXiTotMom  = 0. ;
      Double_t lBachMomX       = 0., lBachMomY  = 0., lBachMomZ   = 0.;

      Float_t lVariableNegNSigmaPion   = -100;
      Float_t lVariableNegNSigmaProton = -100;
      Float_t lVariablePosNSigmaPion   = -100;
      Float_t lVariablePosNSigmaProton = -100;
      Float_t lVariableBachNSigmaPion  = -100;
      Float_t lVariableBachNSigmaKaon  = -100;

      Short_t  lChargeXi = -2;
      Double_t  lEtaXi = -999;
      Double_t  lEtaXiDef = -999;
      Double_t  lThetaXi = -999;
      Double_t  lThetaXiDef = -999;
      Double_t  lPhiXi = -999;
      Double_t  lPhiXiMy = -999;
      Double_t  lPhiXiDef = -999;

      Double_t lRapXi   = -20.0, lRapOmega = -20.0; 
      // -------------------------------------                                                            

      AliAODcascade *xi = fAOD->GetCascade(iXi);
      if (!xi) continue;
      fHistEventV0->Fill(2);

      //Xi decay vertex                                                                                                                       
      lPosXi[0] = xi->DecayVertexXiX();
      lPosXi[1] = xi->DecayVertexXiY();
      lPosXi[2] = xi->DecayVertexXiZ();

      //Xi decay length and radius                                                                                                                               
      lXiRadius                   = TMath::Sqrt( lPosXi[0]*lPosXi[0]  +  lPosXi[1]*lPosXi[1] ); //calculated wrt 0 since PVx/PVy approx = 0                      
      lXiDecayLength = TMath::Sqrt(
				   TMath::Power( lPosXi[0] - lBestPrimaryVtxPos[0] , 2) +
				   TMath::Power( lPosXi[1] - lBestPrimaryVtxPos[1] , 2) +
				   TMath::Power( lPosXi[2] - lBestPrimaryVtxPos[2] , 2)
				   );

      Double_t lXiMom[3];      
      lXiMom[0]=xi->MomXiX();
      lXiMom[1]=xi->MomXiY();
      lXiMom[2]=xi->MomXiZ();
      lXiMomX=lXiMom[0];
      lXiMomY=lXiMom[1];
      lXiMomZ=lXiMom[2];
      lXiTransvMom        = TMath::Sqrt( lXiMomX*lXiMomX   + lXiMomY*lXiMomY );
      lXiTotMom   = TMath::Sqrt( lXiMomX*lXiMomX   + lXiMomY*lXiMomY   + lXiMomZ*lXiMomZ );

      lChargeXi = xi->ChargeXi();
      lThetaXiDef  = xi->Theta();
      lEtaXiDef = xi->Eta();
      lPhiXiDef    = xi->Phi();
      lThetaXi  = 0.5*TMath::Pi()- TMath::ATan(xi->MomXiZ()/lXiTransvMom);
      lEtaXi    = -TMath::Log(TMath::Tan(lThetaXi/2));
      lPhiXiMy  = TMath::Pi()+TMath::ATan2(-(xi->MomXiY()),-(xi->MomXiX()));
      lRapXi    = xi->RapXi();
      lRapOmega = xi->RapOmega();

      Double_t XiP[3] = {-999., -999., -999.};
      XiP[0] = lXiMom[0];
      XiP[1] = lXiMom[1];
      XiP[2] = lXiMom[2];
      Double_t posXi[3] = {-999., -999., -999.};
      posXi[0] = xi->DecayVertexXiX();
      posXi[1] = xi->DecayVertexXiY();
      posXi[2] = xi->DecayVertexXiZ();

      //    cout << "charge  "<< xi->ChargeXi()<< endl;
      //    cout << "SV: px "  <<XiP[0] << " py " << XiP[1] <<" pz "<< XiP[2] << endl;
      Propagate(lBestPrimaryVtxPos, posXi, XiP, b, xi->ChargeXi());
      //    cout << "PV: px "  <<XiP[0] << " py " << XiP[1] <<" pz "<< XiP[2] << endl;
      lPhiXi  = TMath::Pi()+TMath::ATan2(-XiP[1],-XiP[0]);

      if (xi->ChargeXi()>0)    fHistPhi->Fill(lPhiXiMy, lPhiXi);
      fHistTheta->Fill(lThetaXiDef, lThetaXi);
      fHistEta->Fill(lEtaXiDef, lEtaXi);

      //info about daughter tracks
      AliAODTrack *pTrackXi    = dynamic_cast<AliAODTrack*>( xi->GetDaughter(0) );
      AliAODTrack *nTrackXi    = dynamic_cast<AliAODTrack*>( xi->GetDaughter(1) );
      AliAODTrack *bachTrackXi = dynamic_cast<AliAODTrack*>( xi->GetDecayVertexXi()->GetDaughter(0) );
      if (!pTrackXi || !nTrackXi || !bachTrackXi ) {
	AliWarning("ERROR: Could not retrieve one of the 3 AOD daughter tracks of the cascade ...");
	continue;
      }
      fHistEventV0->Fill(3);

      Int_t labelPos    = pTrackXi->GetLabel();
      Int_t labelNeg    = nTrackXi->GetLabel();
      Int_t labelBach   = bachTrackXi->GetLabel();

      //-------------------------------------------------------                                                                                                  
      //---------MC information--------------------------------                                                                                                  
      //-------------------------------------------------------                                                                       

      AliAODMCParticle *VParticleCasc[50]={0};
      Int_t VPdgCasc[50]={0};
      Int_t VParticleCascLabel[50]={0};
      Bool_t IsCommonParton =0;                           
      Int_t TrigLabel=0;
      Int_t CascLabel=0;
      Int_t PdgCodeCommon=-10;   

      TClonesArray* AODMCTrackArray =0x0;
      if(fReadMCTruth){
	fMCEvent= MCEvent();
	//      cout << "hey there I'm getting pdg info! " << endl;
	if (fMCEvent){
	  //	cout << "hey there I'm getting pdg info! (2)" << endl;
	  AODMCTrackArray = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
	  if (AODMCTrackArray == NULL){
	    return;
	    Printf("ERROR: stack not available");
	  }
	  particlePos = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelPos)));
	  //	particlePos = static_cast<TParticle*>(AODMCTrackArray->At(TMath::Abs(labelPos)));
	  particleNeg = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelNeg)));
	  particleBach = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelBach)));
	  //	cout << "label 3 daughters (pos, neg, bach) " << labelPos << "  " << labelNeg << "  " << labelBach << endl;

	  PdgPos = particlePos->GetPdgCode();
	  PdgNeg = particleNeg->GetPdgCode();
	  PdgBach = particleBach->GetPdgCode();

	  labelMotherPos=particlePos->GetMother();
	  labelMotherNeg=particleNeg->GetMother();
	  labelMotherBach=particleBach->GetMother();

	  MotherPos = static_cast<AliAODMCParticle*>(AODMCTrackArray-> At(TMath::Abs(labelMotherPos)));
	  MotherNeg = static_cast<AliAODMCParticle*>(AODMCTrackArray-> At(TMath::Abs(labelMotherNeg)));
	  MotherBach = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelMotherBach)));
	  //	cout << "label 3 mothers (pos, neg, bach) " << labelMotherPos << "  " << labelMotherNeg << "  " << labelMotherBach << endl;

	  PdgMotherPos = MotherPos->GetPdgCode();
	  PdgMotherNeg = MotherNeg->GetPdgCode();
	  PdgMotherBach = MotherBach->GetPdgCode();

	  labelGMotherPos=MotherPos->GetMother();
	  labelGMotherNeg=MotherNeg->GetMother();

	  GMotherPos = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelGMotherPos)));
	  GMotherNeg = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelGMotherNeg)));
	  //	cout << "label 2GMothers (pos, neg, bach) " << labelGMotherPos << "  " << labelGMotherNeg << endl;
	  PdgGMotherPos = GMotherPos->GetPdgCode();
	  PdgGMotherNeg = GMotherNeg->GetPdgCode();

	  labelGMotherBach=MotherBach->GetMother();
	  GMotherBach = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(TMath::Abs(labelGMotherBach)));

	  VParticleCasc[0]=MotherBach; //the cascade candidate
	  VPdgCasc[0]=       VParticleCasc[0]->GetPdgCode();
	  VParticleCascLabel[0]=       VParticleCasc[0]->GetLabel();

	  //	cout << "\n trigger: ";
	  for (Int_t i=0; i<50; i++){
	    //	  cout <<  VPdgTrig[i] << " (" << VParticleTrigLabel[i] << ") " << "<-";
	    if ((VParticleTrigLabel[i] ==1 || VParticleTrigLabel[i] ==-1) && (VPdgTrig[i]==2212) && (VParticleTrigLabel[i] == VParticleTrigLabel[i+1] )) {
	      //	  cout << endl;
	      break;
	    }

	  }

	  //	cout << "casc: " ;
	  for (Int_t i=0; i<50; i++){
	    VParticleCascLabel[i+1]=VParticleCasc[i]->GetMother();
	    VParticleCasc[i+1] = static_cast<AliAODMCParticle*>(AODMCTrackArray-> At(TMath::Abs(VParticleCascLabel[i+1])));
	    VPdgCasc[i+1] = VParticleCasc[i+1]->GetPdgCode();
	    //	    cout << VPdgCasc[i] << " (" << VParticleCascLabel[i] << ") " << "<-" ; 
	    if ((VParticleCascLabel[i] ==1 || VParticleCascLabel[i] ==-1) && (VPdgCasc[i]==2212) && (VParticleCascLabel[i] == VParticleCascLabel[i+1] )) {
	      //cout << endl;
	      break;
	    }
	  }
	
	  for (Int_t i=1; i<50; i++){ //I start from one since last element cannot be a parton but is a hadron
	    if (IsCommonParton==1) break;
	    for (Int_t j=1; j<50; j++){
	      if ((VParticleCascLabel[i] == VParticleTrigLabel[j] ) &&  VParticleTrigLabel[j]!=0 && ( TMath::Abs(VPdgCasc[i]) <=8 ||  TMath::Abs(VPdgCasc[i]) ==21)) { //both Xi and Trigger particle have a common ancestor which has to be a quark or a gluon-> therefore te cascade comes form the jet defined by the trigger particle
		IsCommonParton =1;
		TrigLabel=j;
		CascLabel=i;
		PdgCodeCommon =VPdgCasc[CascLabel];
		break;
	      }
	    }
	  }
	  if (IsCommonParton){/*
				cout << "casc index " << CascLabel;
				cout << "trig index " << TrigLabel;
				cout << " common label " << VParticleCascLabel[CascLabel] << endl;
				cout << "common pdg " <<  PdgCodeCommon << " mother of the common parton " << VPdgCasc[CascLabel+1]<< " (" <<VParticleCascLabel[CascLabel+1]  << ") " << endl;
				cout << "common pdg " <<  PdgCodeCommon << " mother of the common parton (from trigger array) " << VPdgTrig[TrigLabel+1]<< " (" <<VParticleTrigLabel[TrigLabel+1]  << ") " << endl;
				cout << "daughter of common parton " << " for casc " << VPdgCasc[CascLabel-1] << " and ofr trigger " << VPdgTrig[TrigLabel-1]<< endl;*/
	  }
	
	  if (IsCommonParton)	 {
	    fHistCommonParton->Fill(PdgCodeCommon, TrigLabel, CascLabel);
	    fHistIsCommonParton->Fill(1);
	  }
	  else {
	    fHistIsCommonParton->Fill(0);
	  }
	  /* these are always 0 or 1
	     cout << "Pos origin " << particlePos->MCStatusCode() << endl;
	     cout << "Neg origin " << particleNeg->MCStatusCode() << endl;
	     cout << "Bach origin " << particleBach->MCStatusCode() << endl;
	     cout << "MotherPos origin " << MotherPos->MCStatusCode() << endl;
	     cout << "MotherNeg origin " << MotherNeg->MCStatusCode() << endl;
	     cout << "MotherBach origin " << MotherBach->MCStatusCode() << endl;
	     cout << "GMotherBach origin " << GMotherBach->MCStatusCode() << endl;
	  */
	}
      }

      Bool_t isXiNeg=kFALSE;
      Bool_t isXiPos=kFALSE;
      Bool_t isOmegaNeg=kFALSE;
      Bool_t isOmegaPos=kFALSE;
      Float_t isXi=-999;
      if(fReadMCTruth){
	if (fMCEvent){
	  isXiNeg = (PdgPos==2212 && PdgNeg==-211 && PdgMotherPos == 3122 && PdgMotherNeg == 3122 && labelMotherPos==labelMotherNeg  && !MotherPos->IsPhysicalPrimary() && PdgBach==-211 && PdgMotherBach==3312 && PdgGMotherPos==3312 &&PdgGMotherNeg==3312  && labelGMotherPos==labelGMotherNeg && labelGMotherNeg==labelMotherBach);
	  isXiPos = (PdgPos==211 && PdgNeg==-2212 && PdgMotherPos == -3122 &&  PdgMotherNeg == -3122 && labelMotherPos==labelMotherNeg  && !MotherPos->IsPhysicalPrimary() && PdgBach==211 && PdgMotherBach==-3312 && PdgGMotherPos==-3312 && PdgGMotherNeg==-3312  && labelGMotherPos==labelGMotherNeg && labelGMotherNeg==labelMotherBach);
	  isOmegaNeg = (PdgPos==2212 && PdgNeg==-211 && PdgMotherPos == 3122 && PdgMotherNeg == 3122 && labelMotherPos==labelMotherNeg  && !MotherPos->IsPhysicalPrimary() && PdgBach==-321 && PdgMotherBach==3334 && PdgGMotherPos==3334 &&PdgGMotherNeg==3334 && labelGMotherPos==labelGMotherNeg	&& labelGMotherNeg==labelMotherBach);
	  isOmegaPos = (PdgPos==211 && PdgNeg==-2212 && PdgMotherPos == -3122 &&  PdgMotherNeg == -3122 && labelMotherPos==labelMotherNeg  && !MotherPos->IsPhysicalPrimary() && PdgBach==321 && PdgMotherBach==-3334 && PdgGMotherPos==-3334 && PdgGMotherNeg==-3334 && labelGMotherPos==labelGMotherNeg && labelGMotherNeg==labelMotherBach);


	  if(isXiPos || isXiNeg){
	    if (GMotherPos->IsPhysicalPrimary()) lVariableIsPrimaryXi=1;
	    else if(GMotherPos->IsSecondaryFromWeakDecay())     lVariableIsPrimaryXi=2;
	    else if(GMotherPos->IsSecondaryFromMaterial())      lVariableIsPrimaryXi=3;
	    else lVariableIsPrimaryXi=4;
	    lVariableIsPrimaryOmega=0;
	    lVariablePDGCode=PdgMotherBach;
	  }
	  else if(isOmegaPos || isOmegaNeg){
	    if (GMotherPos->IsPhysicalPrimary()) lVariableIsPrimaryOmega=1;
	    else if(GMotherPos->IsSecondaryFromWeakDecay())     lVariableIsPrimaryOmega=2;
	    else if(GMotherPos->IsSecondaryFromMaterial())      lVariableIsPrimaryOmega=3;
	    else lVariableIsPrimaryOmega=4;
	    lVariableIsPrimaryXi=0;
	    lVariablePDGCode=PdgMotherBach;
	  }

	  else {
	    lVariablePDGCode=0;
	    lVariableIsPrimaryXi=0;
	    lVariableIsPrimaryOmega=0;
	  }
	}
      }

      Bool_t isCascadePos=kFALSE;
      Bool_t isCascadeNeg=kFALSE;
      Bool_t isCascade=kFALSE;
      Int_t  isPrimaryCasc=0;

      if (ParticleType==0){
	isCascade = (isXiPos || isXiNeg);
	isCascadePos = isXiPos;
	isCascadeNeg = isXiNeg;
	isPrimaryCasc=lVariableIsPrimaryXi;
      }
      else if (ParticleType==1){
	isCascade = (isOmegaPos || isOmegaNeg);
	isCascadePos = isOmegaPos;
	isCascadeNeg = isOmegaNeg;
	isPrimaryCasc=lVariableIsPrimaryOmega;
      }

      if(fReadMCTruth){
	if (fMCEvent){
	  //cout << "\n this particle has been reconstructed: let's fill the mass Pt histo for true reco K0s "<< endl;
	  if(isCascade && isPrimaryCasc){ 
	    //if (IsCommonParton) 	
	    fHistCommonPartonTrueCasc->Fill(PdgCodeCommon, TrigLabel, CascLabel);
	    //	  fHistReconstructedV0PtMass->Fill(lInvMassCasc, lXiTransvMom, lPercentiles);
	  }
	}
      }

      //daughter track quality cuts------------                                                                                                                  
      if(pTrackXi->Chi2perNDF()>4.)continue;
      if(nTrackXi->Chi2perNDF()>4.)continue;
      if(bachTrackXi->Chi2perNDF()>4.)continue;
      //--------------------------------------             
      fHistEventV0->Fill(4);

      Double_t lBMom[3]={0};
      Double_t  lNMom[3]={0};
      Double_t lPMom[3]={0};
      pTrackXi->GetPxPyPz( lBMom);
      nTrackXi->GetPxPyPz( lPMom);
      bachTrackXi->GetPxPyPz( lNMom);

      Float_t lBachTransMom = TMath::Sqrt( lBMom[0]*lBMom[0] + lBMom[1]*lBMom[1] );
      Float_t lPosTransMom  = TMath::Sqrt( lPMom[0]*lPMom[0] + lPMom[1]*lPMom[1] );
      Float_t lNegTransMom  = TMath::Sqrt( lNMom[0]*lNMom[0] + lNMom[1]*lNMom[1] );


      // 1 - Poor quality related to TPCrefit                                                                                                                   
      ULong_t pStatus    = pTrackXi->GetStatus();
      ULong_t nStatus    = nTrackXi->GetStatus();
      ULong_t bachStatus = bachTrackXi->GetStatus();

      if ((pStatus&AliAODTrack::kTPCrefit)    == 0) {
	//      AliWarning("Pb / V0 Pos. track has no TPCrefit ... continue!");
	continue;
      }
      if ((nStatus&AliAODTrack::kTPCrefit)    == 0) {
	//      AliWarning("Pb / V0 Neg. track has no TPCrefit ... continue!");
	continue;
      }
      if ((bachStatus&AliAODTrack::kTPCrefit) == 0) {
	//      AliWarning("Pb / Bach.   track has no TPCrefit ... continue!");
	continue;
      }
      fHistEventV0->Fill(5);

      //Calculate V0 lifetime for adaptive decay radius cut                                                                                                      
      xi->GetXYZ( lPosV0Xi );
      lV0RadiusXi         = TMath::Sqrt( lPosV0Xi[0]*lPosV0Xi[0]  +  lPosV0Xi[1]*lPosV0Xi[1] );


      // 2 - Poor quality related to TPC clusters: lowest cut of 70 clusters                                                                                   
      lPosTPCClusters   = pTrackXi->GetTPCNcls();
      lNegTPCClusters   = nTrackXi->GetTPCNcls();
      lBachTPCClusters  = bachTrackXi->GetTPCNcls();
   
      if(lPosTPCClusters  < 70) {
	continue;
      }
      if(lNegTPCClusters  < 70) {
	continue;
      }
      if(lBachTPCClusters < 70) {
	continue;
      }
      fHistEventV0->Fill(6);


      Int_t lPosTPCCrossedRows=    pTrackXi ->GetTPCNCrossedRows();
      Int_t lNegTPCCrossedRows=    nTrackXi ->GetTPCNCrossedRows();
      Int_t lBachTPCCrossedRows=    bachTrackXi ->GetTPCNCrossedRows();
      if(lPosTPCCrossedRows  < 80) {
	AliWarning("Pb / V0 Pos. track has less than 80 TPC crossed rows ... continue!");
	continue;
      }
      if(lNegTPCCrossedRows  < 80) {
	AliWarning("Pb / V0 Neg. track has less than 80 TPC crossed rows ... continue!");
	continue;
      }
      if(lBachTPCCrossedRows < 80) {
	AliWarning("Pb / Bach.   track has less than 80 TPC crossed rows ... continue!");
	continue;
      }
      fHistEventV0->Fill(7);


      Float_t rationCrnFindpos= (Float_t)lPosTPCCrossedRows/pTrackXi->GetTPCNclsF();
      Float_t rationCrnFindneg= (Float_t)lNegTPCCrossedRows/nTrackXi->GetTPCNclsF();
      Float_t rationCrnFindbach=(Float_t)lBachTPCCrossedRows/bachTrackXi->GetTPCNclsF();
      if(rationCrnFindpos <  0.8) {
	//      continue;                                                                                                                                      
      }
      if(rationCrnFindneg  < 0.8) {
	//      continue;                                                                                                                                      
      }
      if(rationCrnFindbach <  0.8) {
	//        continue;                                                                                                                                   
      }
      fHistEventV0->Fill(8);

      //Tracklength selection                                                                                                                                        
      Float_t lTrackLengthpos = -1;
      Float_t lTrackLengthneg = -1;
      Float_t lTrackLengthbach = -1;

      lTrackLengthpos = GetLengthInActiveZone( pTrackXi, /*1,*/ 2.0, 220.0, fAOD->GetMagneticField())-TMath::Max(lV0RadiusXi-85.,0.);
      lTrackLengthneg = GetLengthInActiveZone( nTrackXi, /*1,*/ 2.0, 220.0, fAOD->GetMagneticField()) -TMath::Max(lV0RadiusXi-85.,0.);
      lTrackLengthbach = GetLengthInActiveZone( bachTrackXi, /*1,*/ 2.0, 220.0, fAOD->GetMagneticField()) -TMath::Max(lXiRadius-85.,0.);

      Float_t lPosTrackNcrOverLength =  pTrackXi->GetTPCClusterInfo(2,1)/lTrackLengthpos;
      Float_t lNegTrackNcrOverLength =  nTrackXi->GetTPCClusterInfo(2,1)/lTrackLengthneg;
      Float_t lBachTrackNcrOverLength = bachTrackXi->GetTPCClusterInfo(2,1)/lTrackLengthbach;

      if (lTrackLengthpos<90 || lTrackLengthneg<90|| lTrackLengthbach<90) continue;
      fHistEventV0->Fill(9);

      if (lPosTrackNcrOverLength< 0.8 || lNegTrackNcrOverLength< 0.8 || lBachTrackNcrOverLength< 0.8) continue;
      fHistEventV0->Fill(10);


      //GetKinkIndex condition                                                                                                                                       
      Bool_t CascVarPosIsKink=kFALSE;
      Bool_t CascVarNegIsKink=kFALSE;
      Bool_t CascVarBachIsKink=kFALSE;
      if( bachTrackXi->GetKinkIndex(0)>0 ) CascVarBachIsKink = kTRUE;
      if( pTrackXi->GetKinkIndex(0)>0 ) CascVarPosIsKink = kTRUE;
      if( nTrackXi->GetKinkIndex(0)>0 ) CascVarNegIsKink = kTRUE;

      if (CascVarPosIsKink || CascVarNegIsKink || CascVarBachIsKink) continue;

      fHistEventV0->Fill(11);

      //------------------------------------------------ 
      // TPC dEdx information   
      //------------------------------------------------ 
                                                                                                            
      lVariableNegNSigmaPion   = fPIDResponse->NumberOfSigmasTPC( nTrackXi, AliPID::kPion   );
      lVariableNegNSigmaProton = fPIDResponse->NumberOfSigmasTPC( nTrackXi, AliPID::kProton );
      lVariablePosNSigmaPion   = fPIDResponse->NumberOfSigmasTPC( pTrackXi, AliPID::kPion );
      lVariablePosNSigmaProton = fPIDResponse->NumberOfSigmasTPC( pTrackXi, AliPID::kProton );
      lVariableBachNSigmaPion  = fPIDResponse->NumberOfSigmasTPC( bachTrackXi, AliPID::kPion );
      lVariableBachNSigmaKaon  = fPIDResponse->NumberOfSigmasTPC( bachTrackXi, AliPID::kKaon );

      if (lChargeXi==1){
	if (lVariablePosNSigmaPion>4) continue;
	if (lVariableNegNSigmaProton>4) continue;
      }
      if (lChargeXi==-1){
	if (lVariablePosNSigmaProton>4) continue;
	if (lVariableNegNSigmaPion>4) continue;
      }
      if (ParticleType==0 && lVariableBachNSigmaPion>4) continue;
      if (ParticleType==1 && lVariableBachNSigmaKaon>4) continue;

      fHistEventV0->Fill(12);

      //out of bunch pileup study from my task                                                                                                                       
      AliPIDResponse::EDetPidStatus statusTOFPos;
      AliPIDResponse::EDetPidStatus statusTOFNeg;
      AliPIDResponse::EDetPidStatus statusTOFBach;

      statusTOFPos = fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF,pTrackXi);
      statusTOFNeg = fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF,nTrackXi);
      statusTOFBach = fPIDResponse->CheckPIDStatus(AliPIDResponse::kTOF,bachTrackXi);

      Bool_t HasPointOnSPDPos=kFALSE;
      Bool_t HasPointOnSPDNeg=kFALSE;
      Bool_t HasPointOnSPDBach=kFALSE;
      if (pTrackXi->HasPointOnITSLayer(0) || pTrackXi->HasPointOnITSLayer(1) ) HasPointOnSPDPos=kTRUE;
      if (nTrackXi->HasPointOnITSLayer(0) || nTrackXi->HasPointOnITSLayer(1) ) HasPointOnSPDNeg=kTRUE;
      if (bachTrackXi->HasPointOnITSLayer(0) || bachTrackXi->HasPointOnITSLayer(1) ) HasPointOnSPDBach=kTRUE;

      if ( !(statusTOFPos ==  AliPIDResponse::kDetPidOk) && !(statusTOFNeg ==  AliPIDResponse::kDetPidOk) && !(statusTOFBach ==  AliPIDResponse::kDetPidOk) && !(HasPointOnSPDPos) && !(HasPointOnSPDNeg) && !(HasPointOnSPDBach)) {
	// just a try 
	//    if ( !(statusTOFPos ==  AliPIDResponse::kDetPidOk) && !(statusTOFNeg ==  AliPIDResponse::kDetPidOk) && !(statusTOFBach ==  AliPIDResponse::kDetPidOk)) {
	continue;
      }

      fHistEventV0->Fill(13);
  
      //eta of the three daughters                                                                                                                               
      Double_t pEta   = pTrackXi->Eta();
      Double_t nEta    = nTrackXi->Eta();
      Double_t bachEta = bachTrackXi->Eta();
      if (TMath::Abs(pEta)>0.8) continue;
      if (TMath::Abs(nEta)>0.8) continue;
      if (TMath::Abs(bachEta)>0.8) continue;
      fHistEventV0->Fill(14);

      //Lambda mass   
      if ( lChargeXi < 0)
	lInvMassLambdaAsCascDghter    = xi->MassLambda();
      else
	lInvMassLambdaAsCascDghter    = xi->MassAntiLambda();
      
      if (TMath::Abs(lInvMassLambdaAsCascDghter - massLambda) > InvMassLambda) continue;
      lInvMassK0sAsCascDghter    = xi->MassK0Short();

      fHistEventV0->Fill(15);
      //All DCA                                                                                                                                                
      lDcaXiDaughters             = xi->DcaXiDaughters();
      lDcaV0DaughtersXi           = xi->DcaV0Daughters();
      lDcaXiToPrimVertex          = xi->DcaXiToPrimVertex(lBestPrimaryVtxPos[0], lBestPrimaryVtxPos[1], lBestPrimaryVtxPos[2]);
      lDcaV0ToPrimVertexXi        = xi->DcaV0ToPrimVertex();
      lDcaBachToPrimVertexXi      = xi->DcaBachToPrimVertex();
      lDcaPosToPrimVertexXi       = xi->DcaPosToPrimVertex();
      lDcaNegToPrimVertexXi       = xi->DcaNegToPrimVertex();

      if (lChargeXi==1){
	if (lDcaPosToPrimVertexXi<DcaMesonToPrimVertex) continue; 
	if (lDcaNegToPrimVertexXi<DcaBaryonToPrimVertex) continue;
      }

      if (lChargeXi==-1){
	if (lDcaPosToPrimVertexXi<DcaBaryonToPrimVertex) continue;
	if (lDcaNegToPrimVertexXi<DcaMesonToPrimVertex) continue; 
      }

      if (lDcaV0ToPrimVertexXi<DcaV0ToPrimVertex) continue;
      if (lDcaV0DaughtersXi>DcaV0Daughters) continue;
      if (lDcaXiDaughters>DcaCascDaughters) continue;
      if (lDcaBachToPrimVertexXi<DcaBachToPrimVertex) continue;
      fHistEventV0->Fill(16);

      if(lXiRadius<XiRadius[ParticleType]) continue;
      if(lV0RadiusXi<V0RadiusXi[ParticleType]) continue;
      //    if(lXiRadius > 34.) continue;
      //    if(lV0RadiusXi > 34.) continue;
      fHistEventV0->Fill(17);

      //cosine of pointing angle                                                                                                                                  
      lXiCosineOfPointingAngle   = xi->CosPointingAngleXi( lBestPrimaryVtxPos[0],lBestPrimaryVtxPos[1], lBestPrimaryVtxPos[2] );
      lV0CosineOfPointingAngleXi = xi->CosPointingAngle(lBestPrimaryVtxPos);
      //Modification: V0 CosPA wrt to Cascade decay vertex                                                                                                       
      lV0CosineOfPointingAngleXiSpecial = xi->CosPointingAngle(xi->GetDecayVertexXi());

      if (lV0CosineOfPointingAngleXiSpecial<V0CosineOfPointingAngleSpecial) continue;
      if (lXiCosineOfPointingAngle<CascCosineOfPointingAngle) continue;
      fHistEventV0->Fill(18);


      //3D Distance travelled by the V0 in the cascade                                                                                                          
      Float_t lV0DistanceTrav =  TMath::Sqrt(  TMath::Power( lPosV0Xi[0]-lPosXi[0] , 2)
					       + TMath::Power( lPosV0Xi[1]-lPosXi[1] , 2)
					       + TMath::Power( lPosV0Xi[2]-lPosXi[2] , 2) );

      //Total V0 momentum                                                                                                                                       
      Float_t lV0TotMomentum = TMath::Sqrt(  TMath::Power( lNMom[0]+lPMom[0] , 2)
					     + TMath::Power( lNMom[1]+lPMom[1] , 2)
					     + TMath::Power( lNMom[2]+lPMom[2] , 2) );

      //V0 transverse momentum                                                                                                                                  
      Float_t lV0Pt = TMath::Sqrt(  TMath::Power( lNMom[0]+lPMom[0] , 2)
				    + TMath::Power( lNMom[1]+lPMom[1] , 2) );

      //Calculate V0 lifetime: mL/p                                                                                                                               
      Double_t lV0Lifetime=-1;
      if( TMath::Abs(lV0TotMomentum)>1e-5 ){
	lV0Lifetime = 1.115683*lV0DistanceTrav / lV0TotMomentum;
      }else{
	lV0Lifetime = -1;
      }

      if (lChargeXi>0)   { if (lV0Lifetime> V0LifetimeXiPlus) continue;}
      else  if (lChargeXi<0)  {  if (lV0Lifetime> V0LifetimeXiMinus) continue;}

      lInvMassXi        = xi->MassXi();
      lInvMassOmega     = xi->MassOmega();

      //Omega(Xi) rejection when Xi(Omega) is analyzed
      if (fV0=="Omega" && TMath::Abs(lInvMassXi - massXi) < CascRejectionWindow[ParticleType]) continue;
      if (fV0=="Xi" && TMath::Abs(lInvMassOmega - massOmega) <  CascRejectionWindow[ParticleType]) continue;
      
      if (  lXiTotMom > 1.e-5) {
	kctauXi=NominalMassCasc[ParticleType]* lXiDecayLength/lXiTotMom;
      }
      else {
	kctauXi=-999.;
      }

      if (kctauXi> NumberCtau*ctauCasc[ParticleType]) continue; 
      fHistEventV0->Fill(19);

      if (ParticleType==0){
	if (lInvMassXi< 1.28 || lInvMassXi> 1.36) continue;
      }
      else if (ParticleType==1){
	if (lInvMassOmega< 1.62 || lInvMassOmega> 1.72) continue;
      }
      fHistEventV0->Fill(20);

      Double_t lInvMassCasc =0;
      Double_t lRapCasc =0;

      if (ParticleType==0){
	lInvMassCasc = lInvMassXi;
	lRapCasc =lRapXi;
      }
      else if (ParticleType==1){
	lInvMassCasc = lInvMassOmega;
	lRapCasc =lRapOmega;
      }

      if (isHybridMCTruth){
	if (labelPos ==  labelPtTMax || labelNeg ==  labelPtTMax || labelBach ==  labelPtTMax) continue;
      }
    
      if(!(lXiTransvMom> fminPtV0 && lXiTransvMom<fmaxPtV0) )continue;
      fHistEventV0->Fill(21);
     
      Bool_t skipV0=kFALSE;
      if (lXiTransvMom>=ptTriggerMassimoDati){
	skipV0=kTRUE;
	NumberSecondParticleNoAssoc++;
	if(fReadMCTruth){
	  if (fMCEvent){
	    if (isCascade)       NumberSecondParticleTrueNoAssoc++;
	  }
	}
      }

      if (!skipV0) KeepV0Global=kTRUE; //the event had at least one Xi with pT < pT trigger
      if (skipV0) SkipV0Global=kTRUE; //the event has at least one Xi with pT > pT trigger
      //some events might verify both cases

      //    if (skipV0)    continue;

      fMassV0->Fill(lInvMassCasc);    

      for (Int_t m =0; m<5;m++){
	if(lPercentiles>=moltep[m] && lPercentiles<moltep[m+1]){
	  fHistMassvsPt_tagli[m]->Fill(lInvMassCasc,lChargeXi*lXiTransvMom);      
	}
      }
    
      fHistMassvsPt_tagli[5]->Fill(lInvMassCasc,lChargeXi*lXiTransvMom);      

      NumberSecondParticle++;
    
      //save second particle information (V0)
      //    cout << "isprimary casc " << isPrimaryCasc << "labelPrimOrSecV0" << labelPrimOrSecV0 << endl; 
      //cout << "save second particle information (V0) "<< endl;
      if((fReadMCTruth && isEfficiency) || (!fReadMCTruth)){
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cLabelMotherBach       = lVariablePDGCode;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cisPrimCasc            = isPrimaryCasc;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cInvMassLambda         = lInvMassLambdaAsCascDghter;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cInvMassXi             = lInvMassXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cInvMassOmega          = lInvMassOmega;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cCosPointingAngleXi    = lXiCosineOfPointingAngle;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cCosPointingAngleV0ToXi= lV0CosineOfPointingAngleXiSpecial;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cDCAXiDaughters        = lDcaXiDaughters;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cRapCasc               = lRapCasc;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cPt                    = lXiTransvMom;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cctau                  = kctauXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cEta                   = lEtaXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cTheta                 = lThetaXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cPhi                   = lPhiXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cCharge                = lChargeXi;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cAssocOrNot            = skipV0;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cIsCommonParton        = IsCommonParton;
	fEvt->fReconstructedSecond[NumberSecondParticle-1].cPdgCommonParton       = PdgCodeCommon;
      
	fHistPtV0->Fill(lXiTransvMom);
      }

    } //end loop for cascades particles as associated
  }
  
  //begin MC truth loop for Casc particles as associated 
  if(fReadMCTruth){
    if (fMCEvent){
      AODMCTrackArray = dynamic_cast<TClonesArray*>(fAOD->FindListObject(AliAODMCParticle::StdBranchName()));
      if (AODMCTrackArray == NULL){
	return;
	Printf("ERROR: stack not available");
      }
      for(Long_t i = 0; i < AODMCTrackArray->GetEntriesFast(); i++) {
	Bool_t skipV0_MC=kFALSE;
	AliAODMCParticle* particleV0 = static_cast<AliAODMCParticle*>(AODMCTrackArray->At(i));
	if (!particleV0) continue;
	//selection on eta applied online to reduce tree size
	if(TMath::Abs(particleV0->Eta())> fEtaV0Assoc)continue;
	if (!(particleV0->IsPhysicalPrimary()))continue; 
	if(!(particleV0->Pt()> fminPtV0 && particleV0->Pt()<fmaxPtV0) )continue;
	if (TMath::Abs(particleV0->GetPdgCode())!=PDGCodeCasc[ParticleType]) continue;

	if (!isHybridMCTruth){
	  if ((particleV0->Pt())>=ptTriggerMassimoMC) {
	    skipV0_MC=kTRUE;
	    NumberSecondParticleMCNoAssoc++;
	  }
	}
	else {
	  if ((particleV0->Pt())>=ptTriggerMassimoDati) {
	    skipV0_MC=kTRUE;
	    NumberSecondParticleMCNoAssoc++;
	  }
	}

	//	if (skipV0_MC)      continue;
	NumberSecondParticleMC++;
	if (!skipV0_MC) KeepV0GlobalMC=kTRUE; //the event had at least one Xi with pT < pT trigger
	if (skipV0_MC) SkipV0GlobalMC=kTRUE; //the event has at least one Xi with pT > pT trigger
	//some events might verify both cases

	if(isEfficiency) continue;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cLabelMotherBach       = particleV0->GetPdgCode();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cisPrimCasc              = 1;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cInvMassLambda         = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cInvMassXi             = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cInvMassOmega          = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cCosPointingAngleXi    = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cCosPointingAngleV0ToXi= 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cDCAXiDaughters        = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cRapCasc               = particleV0->Y();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cPt                    = particleV0->Pt();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cctau                  = 0;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cEta                   = particleV0->Eta();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cTheta                 = particleV0->Theta();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cPhi                   = particleV0->Phi();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cCharge                = particleV0->Charge();
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cAssocOrNot            = skipV0_MC;
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cIsCommonParton        = 0; //to be assigned
	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cPdgCommonParton        = 0; //to be assigned

	//	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cLabelD1	= particleV0->GetDaughterLabel(0);
	//	fEvt->fReconstructedSecond[NumberSecondParticleMC-1].cLabelD2	= particleV0->GetDaughterLabel(1);

      }
    }
  } //end MC truth loop for Casc particles as associated 
  //************************end hV0 section*************************************************

  fHistEventV0->AddBinContent(22, NumberSecondParticle);    
  fHistEventV0->AddBinContent(23, NumberSecondParticleRecoTrue);    
  fHistEventV0->AddBinContent(24, NumberSecondParticleMC);  
  fHistEventV0->AddBinContent(25, NumberSecondParticleNoAssoc);    
  fHistEventV0->AddBinContent(26, NumberSecondParticleTrueNoAssoc);    
  fHistEventV0->AddBinContent(27, NumberSecondParticleMCNoAssoc);      

  fHistMultvsV0All->Fill(NumberSecondParticle,lPercentiles);
  fHistMultvsV0AllTruth->Fill(NumberSecondParticleRecoTrue,lPercentiles);
  fHistMultvsV0MCAll->Fill(NumberSecondParticleMC,lPercentiles);
  fHistMultvsTriggerAll->Fill(NumberFirstParticle, lPercentiles);
  fHistMultvsTriggerMCTruthAll->Fill(NumberFirstParticleMC, lPercentiles);
  fHistSecondParticleAll->Fill(NumberSecondParticle, NumberSecondParticleMC);
  fHistSecondParticleTruthAll->Fill(NumberSecondParticleRecoTrue, NumberSecondParticleMC);
  if(NumberSecondParticleMC==0 && NumberSecondParticle!=0) fHistEventMult->Fill(17); 
  if(NumberSecondParticleMC < NumberSecondParticleRecoTrue) fHistEventMult->Fill(18); 
  fHistTriggerNotLeading->Fill(NumberSecondParticleNoAssoc,lPercentiles, ptTriggerMassimoDati);
  fHistTriggerNotLeadingMC->Fill(NumberSecondParticleMCNoAssoc,lPercentiles, ptTriggerMassimoMC);

  Bool_t DoubleCounted=kFALSE;
  if((fReadMCTruth && isEfficiency) || (!fReadMCTruth)){
    for(Int_t j=0; j < NumberSecondParticle; j++){
      for(Int_t i=0; i< NumberSecondParticle; i++){
	if(i!=j){
	  if(    fEvt->fReconstructedSecond[j].cLabelMotherBach ==     fEvt->fReconstructedSecond[i].cLabelMotherBach) {
	    DoubleCounted=kTRUE;
	  }
	}
      }
    }
  }
  if (DoubleCounted)  fHistEventMult->Fill(19);

  if((fReadMCTruth && isEfficiency) || (!fReadMCTruth)) NumberSecondParticleAll=NumberSecondParticle;
  else if (fReadMCTruth && !isEfficiency) NumberSecondParticleAll=NumberSecondParticleMC;

  if(NumberSecondParticleAll==0){   
    PostData(1, fOutputList);
    PostData(2, fSignalTree );
    PostData(3,fBkgTree);
    PostData(4, fOutputList2);  
    PostData(5, fOutputList3);     
    PostData(6, fOutputList4);
    //    cout << "event has trigger particle but no V0 " << endl;     
    return;
  }

  fHistSecondParticle->Fill(NumberSecondParticle, NumberSecondParticleMC);
  fHistSecondParticleTruth->Fill(NumberSecondParticleRecoTrue, NumberSecondParticleMC);

  fHistEventV0->AddBinContent(28, NumberSecondParticle);    
  fHistEventV0->AddBinContent(29, NumberSecondParticleRecoTrue);    
  fHistEventV0->AddBinContent(30, NumberSecondParticleMC);    

  fHistEventMult->Fill(22);  
  if(!fReadMCTruth || (fReadMCTruth && isEfficiency)){
    fEvt->fNumberCandidateFirst = NumberFirstParticle;
    fEvt->fNumberCandidateSecond = NumberSecondParticle;
  }
  else if (fReadMCTruth && !isEfficiency && !isHybridMCTruth) {
    fEvt->fNumberCandidateFirst = NumberFirstParticleMC;
    fEvt->fNumberCandidateSecond = NumberSecondParticleMC;
  }
  else{
    fEvt->fNumberCandidateFirst = NumberFirstParticle;
    fEvt->fNumberCandidateSecond = NumberSecondParticleMC;
  }

  //Fill histos for selected events (events with which I perform the same-event and the mixed-event angular correlation, that is having NT> 0 and NV0>0
  fHistTriggerwV0->Fill(NumberFirstParticle);
  fHistTriggerwV0MCTruth->Fill(NumberFirstParticleMC);
  fHistMultvsV0->Fill(NumberSecondParticle,lPercentiles);
  fHistMultvsV0Truth->Fill(NumberSecondParticleRecoTrue,lPercentiles);
  fHistMultvsV0MC->Fill(NumberSecondParticleMC,lPercentiles);
  fHist_multiplicity->Fill(lPercentiles);
  fHistZvertex->Fill(lBestPrimaryVtxPos[2]);
  fHistTrack->AddBinContent(16, NumberFirstParticle);
  fHistTrack->AddBinContent(17, NumberFirstParticleMC);
  fHistMultvsTrigger->Fill(NumberFirstParticle, lPercentiles);
  fHistMultvsTriggerMCTruth->Fill(NumberFirstParticleMC, lPercentiles);
  fHistMultiplicityVsVertexZ->Fill(lBestPrimaryVtxPos[2], lPercentiles);
 
  for(Int_t i=0; i< 100; i++){
    if(( lPercentiles <i+1) && (lPercentiles >= i) ) fHistTriggervsMult->AddBinContent(i+1,NumberFirstParticle);  
  }
  for(Int_t i=0; i< 100; i++){
    if(( lPercentiles <i+1) && (lPercentiles >= i) ) fHistTriggervsMultMC->AddBinContent(i+1,NumberFirstParticleMC);  
  }


  for(Int_t l=0; l< NumberFirstParticleAll; l++){
    fHistPt->Fill(fEvt->fReconstructedFirst[l].fPt);
    fHistPtvsMult->Fill(fEvt->fReconstructedFirst[l].fPt, lPercentiles);
    fHist_eta_phi->Fill(fEvt->fReconstructedFirst[l].fPhi, fEvt->fReconstructedFirst[l].fEta);
 
  }

  if (!fReadMCTruth || (fReadMCTruth && isEfficiency) || (isHybridMCTruth)){
    fHistPtMaxvsMult->Fill(ptTriggerMassimoDati, lPercentiles);
    if (KeepV0Global)    fHistPtMaxvsMultKeepV0->Fill(ptTriggerMassimoDati, lPercentiles);
    if (SkipV0Global)    fHistPtMaxvsMultSkipV0->Fill(ptTriggerMassimoDati, lPercentiles);
    fHist_eta_phi_PtMax->Fill(phiTriggerMassimoDati, etaTriggerMassimoDati);
  }
  if (fReadMCTruth && !isEfficiency && !isHybridMCTruth) {
    fHistPtMaxvsMult->Fill(ptTriggerMassimoMC, lPercentiles);
    if (KeepV0GlobalMC)    fHistPtMaxvsMultKeepV0->Fill(ptTriggerMassimoMC, lPercentiles);
    if (SkipV0GlobalMC)    fHistPtMaxvsMultSkipV0->Fill(ptTriggerMassimoMC, lPercentiles);
    fHist_eta_phi_PtMax->Fill(phiTriggerMassimoMC, etaTriggerMassimoMC);
  }

  /* this part is implemented above in a different way 
     if(fFirstpart == fSecondpart){ 
     DoPairshh(lPercentiles, fieldsign);  
     else{
     //Remove candidates that are at the same time a ptc1 and ptc2
     for (int i=0; i < fEvt->fNumberCandidateFirst; i++) {
     for (int j=0; j<fEvt->fNumberCandidateSecond; j++) {
     if (fEvt->fReconstructedFirst[i].index == fEvt->fReconstructedSecond[j].index) {
     //cout<<"the track can be both tracks!"<<endl;
     fEvt->fReconstructedFirst[i].doSkipOver = kTRUE;
     fEvt->fReconstructedSecond[j].doSkipOver = kTRUE;
     }
     }
     }
  */
  
  //--------------------------------------------------------------
  Double_t ptTriggerMassimo=0;
  if (!fReadMCTruth || (fReadMCTruth && isEfficiency) || (fReadMCTruth && isHybridMCTruth) ) ptTriggerMassimo=ptTriggerMassimoDati;
  if (fReadMCTruth && !isEfficiency && !isHybridMCTruth) ptTriggerMassimo=ptTriggerMassimoMC;
  DoPairsh1h2(lPercentiles, fieldsign, lBestPrimaryVtxPos[2], ptTriggerMassimo);  

  PostData(1, fOutputList);     
  PostData(2, fSignalTree);
  PostData(3, fBkgTree);
  PostData(4, fOutputList2);  
  PostData(5, fOutputList3);     
  PostData(6, fOutputList4);     

  fEventColl[zBin][centralityBin]->FifoShift();       
}


//----------------------------------------------------------------------------------------------------

void AliAnalysisTaskCorrelationhCascDATA::DoPairsh1h2 ( const Float_t lPercentiles, int fieldsign, Double_t lBestPrimaryVtxPos, Double_t ptTriggerMassimo)  {

  //-----------
  double DCAxyP1  = -999. ;
  double DCAzP1   = -999. ;  
  double DCAxyP2  = -999. ; 
  double DCAzP2   = -999. ;  

  double  ptP1 = -999.;
  double  ptP2 = -999.;

  bool isP1  = kFALSE;
  bool isaP1 = kFALSE;
  bool isP2  = kFALSE;
  bool isaP2 = kFALSE;

  Int_t  SignP1 = -999;
  Int_t  SignP2 = -999;

  double dphi  = -999.;
  double dphis = -999.;
  double deta  = -999.;
  double dtheta = -999.;
  
  bool isMC1 = kFALSE;
  bool isMC2 = kFALSE;

  bool isMCvector = kFALSE;
  bool sameMother = kFALSE;
  bool sameGrandMother = kFALSE;
  
  Int_t mcMotherLabelP1 = -999;
  Int_t mcMotherLabelP2 = -999;

  Int_t mcGrandMotherLabelP1 = -999;
  Int_t mcGrandMotherLabelP2 = -999;
 
  Int_t mcPDGGrandMother = 0;
  Int_t mcGrandMotherBin = 0;

  int evmultmixed = 0;
  bool multmixedcounted = kFALSE;
 
  double pairKstar   = 0.;
  double pairKstarMC = 0.;
  double pairMass  = 0.;
  double pairMassE = 0.;
  double pairKt    = 0.;
  double pTmax = 0.;

  Int_t lptindex=0;
  /*
    Bool_t  isPDGCodepiKp;
    Bool_t isPDGCodeDesired;
    double ptTriggerSecond=0;
    double ptTriggerThird=0;
    Int_t indexTrigger=-999;



    for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {
    if (fEvt->fReconstructedFirst[i].fPt>ptTriggerSecond && fEvt->fReconstructedFirst[i].fPt!= ptTriggerMassimo ) ptTriggerSecond=fEvt->fReconstructedFirst[i].fPt;
    cout <<"loop on pt " <<  fEvt->fReconstructedFirst[i].fPt<< endl;
    }
    cout << "pt max " << ptTriggerMassimo<< " pt second " <<  ptTriggerSecond<< endl;

    for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {
    if (fEvt->fReconstructedFirst[i].fPt>ptTriggerThird && fEvt->fReconstructedFirst[i].fPt!= ptTriggerMassimo && fEvt->fReconstructedFirst[i].fPt!= ptTriggerSecond ) ptTriggerThird=fEvt->fReconstructedFirst[i].fPt;
    cout <<"loop on pt " <<  fEvt->fReconstructedFirst[i].fPt<< endl;
    }
    cout << "pt max " << ptTriggerMassimo<< " pt second " <<  ptTriggerSecond << " ptriggerthird " << ptTriggerThird<< endl;

    for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {

    isPDGCodepiKp    = (TMath::Abs(fEvt->fReconstructedFirst[i].fPDGcode)==211 || TMath::Abs(fEvt->fReconstructedFirst[i].fPDGcode)==2212 || TMath::Abs(fEvt->fReconstructedFirst[i].fPDGcode)==321);
    isPDGCodeDesired=  isPDGCodepiKp ;

    if (fEvt->fReconstructedFirst[i].fPt == ptTriggerMassimo && isPDGCodeDesired) {
    indexTrigger=i; 
    lptindex=1;
    continue;
    }
    }

    if (indexTrigger!=-999)  cout << fEvt->fReconstructedFirst[indexTrigger].fPt << " pdg " << fEvt->fReconstructedFirst[indexTrigger].fPDGcode<< endl;
    if (indexTrigger==-999){
    for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {
    if (fEvt->fReconstructedFirst[i].fPt == ptTriggerSecond && isPDGCodeDesired) {
    indexTrigger=i;
    lptindex=2;
    continue;
    }
    }
    }
    if (indexTrigger!=-999)  cout <<" second trigger " <<  fEvt->fReconstructedFirst[indexTrigger].fPt << " pdg " << fEvt->fReconstructedFirst[indexTrigger].fPDGcode<< endl;

    if (indexTrigger==-999){
    for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {
    if (fEvt->fReconstructedFirst[i].fPt == ptTriggerThird && isPDGCodeDesired) {
    indexTrigger=i;
    lptindex=3;
    continue;
    }
    }
    }
  */
  for (int i=0; i<fEvt->fNumberCandidateFirst; i++) {
    
    //I select as trigger particle only the highest-pT one
    /*
      if (fReadMCTruth && !isEfficiency && !isHybridMCTruth){
      if (i!= indexTrigger) continue;
      }
      else{
      if ( fEvt->fReconstructedFirst[i].fPt <ptTriggerMassimo ) continue;
      }
    */
    if ( fEvt->fReconstructedFirst[i].fPt <ptTriggerMassimo ) continue;
    //    cout <<" I got " <<  fEvt->fReconstructedFirst[i].fPt << endl;
    for (int eventNumber=0; eventNumber<fnEventsToMix+1; eventNumber++) { 
      //if (!multmixedcounted && eventNumber!=0 && ((fEvt+eventNumber)->fNumberCandidateFirst)!=0.) evmultmixed++; 
      if (!multmixedcounted && eventNumber!=0 && ((fEvt+eventNumber)->fNumberCandidateSecond)!=0.) {
	evmultmixed++; 
      }
      for (int j=0; j<(fEvt+eventNumber)->fNumberCandidateSecond; j++) {
	
	//c 	if ((fEvt+eventNumber)->fReconstructedSecond[j].doSkipOver) continue;
         
	deta   = CalculateDeltaEta(fEvt->fReconstructedFirst[i].fEta, (fEvt+eventNumber)->fReconstructedSecond[j].cEta);
	dtheta = CalculateDeltaTheta(fEvt->fReconstructedFirst[i].fTheta, (fEvt+eventNumber)->fReconstructedSecond[j].cTheta);
	//dphi   = CalculateDeltaPhi(fEvt->fReconstructedFirst[i].fPhi, (fEvt+eventNumber)->fReconstructedSecond[j].cPhi);
	dphi = CalculateDPhiStar(fEvt->fReconstructedFirst[i].fCharge, (fEvt+eventNumber)->fReconstructedSecond[j].cCharge, fieldsign,fEvt->fReconstructedFirst[i].fPt , (fEvt+eventNumber)->fReconstructedSecond[j].cPt, fEvt->fReconstructedFirst[i].fPhi, (fEvt+eventNumber)->fReconstructedSecond[j].cPhi,0); 
	
	if (eventNumber==0) {//Same event pair histogramming

	  fTreeVariablePtTrigger              = fEvt->fReconstructedFirst[i].fPt;		       
	  fTreeVariableChargeTrigger          = fEvt->fReconstructedFirst[i].fCharge;		       
	  fTreeVariableEtaTrigger             = fEvt->fReconstructedFirst[i].fEta; 		       
	  fTreeVariablePhiTrigger	      =	fEvt->fReconstructedFirst[i].fPhi;             
	  fTreeVariableDCAz		      =	fEvt->fReconstructedFirst[i].fDCAz;             
	  fTreeVariableDCAxy		      =	fEvt->fReconstructedFirst[i].fDCAxy;   
	  fTreeVariablePDGCodeTrigger         = fEvt->fReconstructedFirst[i].fPDGcode ;
	  fTreeVariableisPrimaryTrigger       = fEvt->fReconstructedFirst[i].isP ;

	  fTreeVariablePDGCodeAssoc           =    fEvt->fReconstructedSecond[j].cLabelMotherBach      ;
	  fTreeVariableisPrimaryV0	      =    fEvt->fReconstructedSecond[j].cisPrimCasc           ;
	  fTreeVariableInvMassLambda	      =    fEvt->fReconstructedSecond[j].cInvMassLambda        ;	 
	  fTreeVariableInvMassXi	      =    fEvt->fReconstructedSecond[j].cInvMassXi            ;
	  fTreeVariableInvMassOmega           =    fEvt->fReconstructedSecond[j].cInvMassOmega         ;
	  fTreeVariableXiCosineOfPointingAngle=    fEvt->fReconstructedSecond[j].cCosPointingAngleXi   ;
	  fTreeVariableV0CosineOfPointingAngle=    fEvt->fReconstructedSecond[j].cCosPointingAngleV0ToXi;
	  fTreeVariableDcaXiDaughters         =    fEvt->fReconstructedSecond[j].cDCAXiDaughters       ;
	  fTreeVariableRapAssoc 	      =    fEvt->fReconstructedSecond[j].cRapCasc              ;
	  fTreeVariablePtV0     	      =    fEvt->fReconstructedSecond[j].cPt                   ;
	  fTreeVariablectau       	      =    fEvt->fReconstructedSecond[j].cctau                 ; 
	  fTreeVariableEtaV0                  =    fEvt->fReconstructedSecond[j].cEta                  ; 
	  fTreeVariablePhiV0		      =    fEvt->fReconstructedSecond[j].cPhi                  ; 
	  fTreeVariableChargeAssoc 	      =    fEvt->fReconstructedSecond[j].cCharge               ; 
	  fTreeVariableSkipAssoc	      =    fEvt->fReconstructedSecond[j].cAssocOrNot           ; 
	  fTreeVariableIsCommonParton	      =    fEvt->fReconstructedSecond[j].cIsCommonParton       ; 
	  fTreeVariablePdgCommonParton	      =    fEvt->fReconstructedSecond[j].cPdgCommonParton      ; 

	  fTreeVariableDeltaEta	      	      = deta;  
	  fTreeVariableDeltaPhi		      = dphi;
	  fTreeVariableDeltaTheta             = dtheta;
	  fTreeVariableMultiplicity	      = lPercentiles;
	  fTreeVariableZvertex                = lBestPrimaryVtxPos;
	  fTreeVariableTriggerIndex         = lptindex;
	  //	  fTreeVariableisPrimaryTrigger       =  fEvt->fReconstructedFirst[i].isP ; //this should always be zero and doesn't give us the correct info

	  fSignalTree->Fill();  

	}

	else {//Mixed-event pair histogramming
	  fTreeVariablePtTrigger              = fEvt->fReconstructedFirst[i].fPt;		       
	  fTreeVariableChargeTrigger          = fEvt->fReconstructedFirst[i].fCharge;		       
	  fTreeVariableEtaTrigger             = fEvt->fReconstructedFirst[i].fEta; 		       
	  fTreeVariablePhiTrigger	      =	fEvt->fReconstructedFirst[i].fPhi;             
	  fTreeVariableDCAz		      =	fEvt->fReconstructedFirst[i].fDCAz;             
	  fTreeVariableDCAxy		      =	fEvt->fReconstructedFirst[i].fDCAxy;   
	  fTreeVariablePDGCodeTrigger         = fEvt->fReconstructedFirst[i].fPDGcode ;
	  fTreeVariableisPrimaryTrigger       = fEvt->fReconstructedFirst[i].isP ;

	  fTreeVariablePDGCodeAssoc           =    (fEvt+eventNumber)->fReconstructedSecond[j].cLabelMotherBach      ;
	  fTreeVariableisPrimaryV0	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cisPrimCasc           ;
	  fTreeVariableInvMassLambda	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cInvMassLambda        ;	 
	  fTreeVariableInvMassXi	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cInvMassXi            ;
	  fTreeVariableInvMassOmega           =    (fEvt+eventNumber)->fReconstructedSecond[j].cInvMassOmega         ;
	  fTreeVariableXiCosineOfPointingAngle=    (fEvt+eventNumber)->fReconstructedSecond[j].cCosPointingAngleXi   ;
	  fTreeVariableV0CosineOfPointingAngle=    (fEvt+eventNumber)->fReconstructedSecond[j].cCosPointingAngleV0ToXi;
	  fTreeVariableDcaXiDaughters         =    (fEvt+eventNumber)->fReconstructedSecond[j].cDCAXiDaughters       ;
	  fTreeVariableRapAssoc 	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cRapCasc              ;
	  fTreeVariablePtV0     	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cPt                   ;
	  fTreeVariablectau       	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cctau                 ; 
	  fTreeVariableEtaV0                  =    (fEvt+eventNumber)->fReconstructedSecond[j].cEta                  ; 
	  fTreeVariablePhiV0		      =    (fEvt+eventNumber)->fReconstructedSecond[j].cPhi                  ; 
	  fTreeVariableChargeAssoc 	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cCharge               ; 
	  fTreeVariableSkipAssoc 	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cAssocOrNot           ; 
	  fTreeVariableIsCommonParton	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cIsCommonParton       ; 
	  fTreeVariablePdgCommonParton	      =    (fEvt+eventNumber)->fReconstructedSecond[j].cPdgCommonParton      ; 

	  fTreeVariableDeltaEta	       	      =deta;  
	  fTreeVariableDeltaPhi		      =dphi;
	  fTreeVariableDeltaTheta             =dtheta;      
	  fTreeVariableMultiplicity	      = lPercentiles;
	  fTreeVariableZvertex                = lBestPrimaryVtxPos;
	  fTreeVariableTriggerIndex         = lptindex;
	  //	  fTreeVariableisPrimaryTrigger       =  (fEvt+eventNumber)->fReconstructedFirst[i].isP;
       
	  fBkgTree->Fill();  
	  
	} //mixed
	
      } //end second particle loop

    }//end event loop

    if (evmultmixed!=0) multmixedcounted = kTRUE;
    
  } // end first particle loop
  
  if  (multmixedcounted){
    fHistMultiplicityOfMixedEvent->Fill(evmultmixed,lPercentiles); //tells me with how many events the mixed-event was done
    //    cout << "percentiles " << lPercentiles << endl;
  }
  
}

/*
//----------------------------------------------------------------------------------------------
//void AliAnalysisTaskKPFemto::DoPairshh (const Float_t lPercentiles, int fieldsign) {
void AliAnalysisTaskKPFemto::DoPairshh (const Int_t lPercentiles, int fieldsign) {
return;
}
*/


//-----------------------------------------------------------------------------------------------
double AliAnalysisTaskCorrelationhCascDATA::CalculateDPhiStar(Short_t chg1, Short_t chg2, Int_t magSign, Double_t ptv1, Double_t ptv2, Double_t phi1, Double_t phi2,Double_t rad) { //AliFemtoUser/AliFemtoPairCutDetaDphi.h

  const Double_t unit_factor = 0.299792458 / 2.0;
  const Double_t b_field = 0.5006670488586 * magSign;

  Double_t  shift1 = TMath::ASin(unit_factor * chg1 * b_field * rad / ptv1);
  Double_t  shift2 = TMath::ASin(unit_factor * chg2 * b_field * rad / ptv2);

  double dps = (phi1 + shift1) - (phi2 + shift2);
  
  //  dps = TVector2::Phi_mpi_pi(dps); //to be checked

  return dps; //deltaphi;
  
}

//-------------------------------------------------------------------------------------
Double_t AliAnalysisTaskCorrelationhCascDATA::CalculateDeltaEta( Double_t eta1, Double_t eta2 )  {   //AliFemtoUser/AliFemtoPairCutDetaDphi.h
  const double deta = eta2 - eta1;
  return deta;
}
//_______________________________________________________________
Double_t AliAnalysisTaskCorrelationhCascDATA::CalculateDeltaTheta( Double_t theta1, Double_t theta2 )  {  
  const double dtheta = theta2 - theta1;
  return dtheta;
}
//-------------------------------------------------------------------------------------
Double_t AliAnalysisTaskCorrelationhCascDATA::CalculateDeltaPhi( Double_t phi1, Double_t phi2 )  {   //AliFemtoUser/AliFemtoPairCutDetaDphi.h
  const double dphi = phi2 - phi1;
  return dphi;
}

//_____________________________________________________________________________
void AliAnalysisTaskCorrelationhCascDATA::Terminate(Option_t *)
{
  // terminate
  // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
Float_t AliAnalysisTaskCorrelationhCascDATA::GetLengthInActiveZone( AliAODTrack *gt, Float_t deltaY, Float_t deltaZ, Float_t b ){
  // Input parameters:
  //   deltaY - user defined "dead region" in cm
  //   deltaZ - user defined "active region" in cm (250 cm drift lenght - 14 cm L1 delay
  //   b     - magnetic field 
  AliESDtrack esdTrack( gt );
  esdTrack.SetESDEvent((AliESDEvent*) gt->GetEvent() );
  AliExternalTrackParam etp;
  etp.CopyFromVTrack(gt);
  esdTrack.ResetTrackParamIp(&etp);
  return esdTrack.GetLengthInActiveZone(1, deltaY, deltaZ, b);
}
//_____________________________________________________________________________
