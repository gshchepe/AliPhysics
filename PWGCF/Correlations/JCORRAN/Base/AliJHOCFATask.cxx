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
 **************************************************************************/

#include "AliJHOCFATask.h"
#include "AliAnalysisManager.h"
#include "AliJBaseTrack.h"

/* -------------------------------------------------------------------------- /
/ Methods inherited from AliAnalysisTaskSE.                                   /
/ -------------------------------------------------------------------------- */
AliJHOCFATask::AliJHOCFATask():   
	AliAnalysisTaskSE("JHOCFATask"),
	fJCatalystTask(NULL),
	fJCatalystTaskName("JCatalystTask"),
	fIsMC(kFALSE),
	fHOCFATask(NULL),
	fHOCFADebugLevel(0),
	fHOCFAnCentralityBins(9), fHOCFAMultiplicityMin(10),
	fHOCFAPtMin(0.2), fHOCFAPtMax(5.),
  fHOCFAUseWeightsNUE(kTRUE), fHOCFAUseWeightsNUA(kFALSE),
  fHOCFAGetSC3h(kTRUE),
  fHOCFAGetEtaGap(kFALSE), fHOCFAEtaGap(0.),
  fHOCFANCombi(6), fHOCFAvalues(0), fHOCFAcombi(0)
{
// Dummy constructor of the class.
}

// ------------------------------------------------------------------------- //
AliJHOCFATask::AliJHOCFATask(const char *name):
	AliAnalysisTaskSE(name),
	fJCatalystTask(NULL),
	fJCatalystTaskName("JCatalystTask"),
	fIsMC(kFALSE),
	fHOCFATask(NULL),
	fHOCFADebugLevel(0),
	fHOCFAnCentralityBins(9), fHOCFAMultiplicityMin(10),
	fHOCFAPtMin(0.2), fHOCFAPtMax(5.),
  fHOCFAUseWeightsNUE(kTRUE), fHOCFAUseWeightsNUA(kFALSE),
  fHOCFAGetSC3h(kTRUE),
  fHOCFAGetEtaGap(kFALSE), fHOCFAEtaGap(0.),
  fHOCFANCombi(6), fHOCFAvalues(0), fHOCFAcombi(0)
{
// Constructor of the class.
	AliInfo("AliJHOCFATask Constructor");
	DefineOutput(1, TList::Class());
}

// ------------------------------------------------------------------------- //
AliJHOCFATask::AliJHOCFATask(const AliJHOCFATask& ap):
	AliAnalysisTaskSE(ap.GetName()), 
	fJCatalystTask(ap.fJCatalystTask),
	fJCatalystTaskName(ap.fJCatalystTaskName),
  fIsMC(ap.fIsMC),
	fHOCFATask(ap.fHOCFATask),
	fHOCFADebugLevel(ap.fHOCFADebugLevel),
	fHOCFAnCentralityBins(ap.fHOCFAnCentralityBins),
  fHOCFAMultiplicityMin(ap.fHOCFAMultiplicityMin),
  fHOCFAPtMin(ap.fHOCFAPtMin),
  fHOCFAPtMax(ap.fHOCFAPtMax),
  fHOCFAUseWeightsNUE(ap.fHOCFAUseWeightsNUE),
  fHOCFAUseWeightsNUA(ap.fHOCFAUseWeightsNUA),
  fHOCFAGetSC3h(ap.fHOCFAGetSC3h),
  fHOCFAGetEtaGap(ap.fHOCFAGetEtaGap),
  fHOCFAEtaGap(ap.fHOCFAEtaGap),
  fHOCFANCombi(ap.fHOCFANCombi),
  fHOCFAvalues(ap.fHOCFAvalues),
  fHOCFAcombi(ap.fHOCFAcombi)
{ 
// Copy operator of the class.
	AliInfo("DEBUG AliJHOCFATask COPY");
}

// ------------------------------------------------------------------------- //
AliJHOCFATask& AliJHOCFATask::operator = (const AliJHOCFATask& ap)
{
// Assignment operator of the class.
	AliInfo("DEBUG AliJHOCFATask operator =");
	this->~AliJHOCFATask();	// Destroy any already existing version of the task.
	new(this) AliJHOCFATask(ap);	// Create a fresh version with no conflict.
	return *this;
}

// ------------------------------------------------------------------------- //
AliJHOCFATask::~AliJHOCFATask()
{
// Destructor of the class.
	AliInfo("AliJHOCFATask Destructor");
	delete fHOCFATask;
}

// ------------------------------------------------------------------------- //
void AliJHOCFATask::UserCreateOutputObjects()
{  
	if (fHOCFADebugLevel > 1) {printf("AliJHOCFATask::UserCreateOutPutData().\n");}

// Create the jCorran output objects.
	AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
	cout << "Name of AliJCatalystTask: " << fJCatalystTaskName << endl;
	fJCatalystTask = (AliJCatalystTask*)(man->GetTask(fJCatalystTaskName));

// Create an instance of the analysis task.
	fHOCFATask = new AliAnalysisTaskHOCFA("HOCFA");
	fHOCFATask->SetDebugLevel(fHOCFADebugLevel);
  fHOCFATask->SetCentralityBinning(fHOCFAnCentralityBins);
  fHOCFATask->SetCentralityArray(fHOCFAvalues);
  fHOCFATask->SetMinMultiplicity(fHOCFAMultiplicityMin);
  fHOCFATask->SetPtRange(fHOCFAPtMin, fHOCFAPtMax);
  fHOCFATask->SetParticleWeights(fHOCFAUseWeightsNUE, fHOCFAUseWeightsNUA);
  fHOCFATask->SetObservable(fHOCFAGetSC3h);
  fHOCFATask->SetNumberCombi(fHOCFANCombi);
  fHOCFATask->SetHarmoArray(fHOCFAcombi);
  fHOCFATask->SetEtaGaps(fHOCFAGetEtaGap, fHOCFAEtaGap);
	OpenFile(1);

	fHOCFATask->UserCreateOutputObjects();
	PostData(1, fHOCFATask->GetCFAlist());
}

// ------------------------------------------------------------------------- //
void AliJHOCFATask::Init()
{
// Initialisation of parameters.
	AliInfo("Doing initialization");
}

// ------------------------------------------------------------------------- //
void AliJHOCFATask::UserExec(Option_t* /*option*/) 
{
// Processing of one event.
	if (fHOCFADebugLevel > 5) {printf("AliJHOCFATask::UserExec() \n");}
	if (!((Entry()-1)%100)) {AliInfo(Form("Processing event # %lld",  Entry()));}

	if (fJCatalystTask->GetJCatalystEntry() != fEntry) {return;}
	if (fJCatalystTask->GetCentrality() > 80. || fJCatalystTask->GetCentrality() < 0.) {return;}
	if (fHOCFADebugLevel > 5) {
		cout << Form("Event %d:%d\n",fEntry, fJCatalystTask->GetJCatalystEntry()) << endl;
		cout << Form("%s, Nch = %d, cent = %.0f\n", GetJCatalystTaskName().Data(), fJCatalystTask->GetInputList()->GetEntriesFast(), fJCatalystTask->GetCentrality()) << endl;
	}

	fHOCFATask->SetInputList(fJCatalystTask->GetInputList());
	fHOCFATask->SetEventCentrality(fJCatalystTask->GetCentrality());
	fHOCFATask->UserExec("");
}

// ------------------------------------------------------------------------- //
void AliJHOCFATask::Terminate(Option_t *)
{
// Processing when the event loop is done.
	printf("AliJHOCFATask is done!\n");
}