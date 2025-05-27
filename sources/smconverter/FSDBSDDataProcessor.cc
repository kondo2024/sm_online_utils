#include "FSDBSDDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtFSDBSDPla.hh"
#include "TArtSAMURAIParameters.hh"
//____________________________________________________________________
void FSDBSDDataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibFSDBSD = new TArtCalibFSDBSDPla;
  fCalibReady = true;
}
//____________________________________________________________________
void FSDBSDDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibFSDBSD->GetFSDBSDPlaArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);

}
//____________________________________________________________________
void FSDBSDDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt1   = new TH2D("fsdbsd_idt1","FSDBSD ID T1Raw",66,0.5,66.5,100,25000,40000);
  fhidt2   = new TH2D("fsdbsd_idt2","FSDBSD ID T2Raw",66,0.5,66.5,100,25000,40000);
  fhidt3   = new TH2D("fsdbsd_idt3","FSDBSD ID T3Raw",66,0.5,66.5,100,25000,40000);
  fhidtot1 = new TH2D("fsdbsd_idtot1","FSDBSD ID ToT1Raw",66,0.5,66.5,100,0,2000);
  fhidtot2 = new TH2D("fsdbsd_idtot2","FSDBSD ID ToT2Raw",66,0.5,66.5,100,0,2000);
  fhidtot3 = new TH2D("fsdbsd_idtot3","FSDBSD ID ToT3Raw",66,0.5,66.5,100,0,2000);

  fHistArray.push_back(fhidt1);
  fHistArray.push_back(fhidt2);
  fHistArray.push_back(fhidt3);
  fHistArray.push_back(fhidtot1);
  fHistArray.push_back(fhidtot2);
  fHistArray.push_back(fhidtot3);
}
//____________________________________________________________________
void FSDBSDDataProcessor::ClearData()
{
  fCalibFSDBSD->ClearData();
}
//____________________________________________________________________
void FSDBSDDataProcessor::ReconstructData()
{
  fCalibFSDBSD->ReconstructData();
}
//____________________________________________________________________
void FSDBSDDataProcessor::FillHistograms()
{
  TClonesArray *array = fCalibFSDBSD->GetFSDBSDPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtFSDBSDPla *pla = (TArtFSDBSDPla*)array->At(i);
    Double_t id = pla->GetID();
    Double_t T1Raw = pla->GetT1Raw();
    Double_t T2Raw = pla->GetT2Raw();
    Double_t T3Raw = pla->GetT3Raw();
    Double_t TOT1Raw = pla->GetTOT1Raw();
    Double_t TOT2Raw = pla->GetTOT2Raw();
    Double_t TOT3Raw = pla->GetTOT3Raw();

    fhidt1->Fill(id,T1Raw);
    fhidt2->Fill(id,T2Raw);
    fhidt3->Fill(id,T3Raw);
    fhidtot1->Fill(id,TOT1Raw);
    fhidtot2->Fill(id,TOT2Raw);
    fhidtot3->Fill(id,TOT3Raw);
  }
}
//____________________________________________________________________
