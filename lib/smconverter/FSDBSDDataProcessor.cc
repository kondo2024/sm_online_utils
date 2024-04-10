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

  fhidt1 = new TH2D("fsdbsd_idt1","FSDBSD ID T1Raw",120,0.5,120.5,100,0,8000);
  fhidt2 = new TH2D("fsdbsd_idt2","FSDBSD ID T2Raw",120,0.5,120.5,100,0,8000);
  fhidt3 = new TH2D("fsdbsd_idt3","FSDBSD ID T3Raw",120,0.5,120.5,100,0,8000);
  fhidt1_trl = new TH2D("fsdbsd_idt1_trl","FSDBSD ID T1Raw Trail",120,0.5,120.5,100,0,8000);
  fhidt2_trl = new TH2D("fsdbsd_idt2_trl","FSDBSD ID T2Raw Trail",120,0.5,120.5,100,0,8000);
  fhidt3_trl = new TH2D("fsdbsd_idt3_trl","FSDBSD ID T3Raw Trail",120,0.5,120.5,100,0,8000);

  fHistArray.push_back(fhidt1);
  fHistArray.push_back(fhidt2);
  fHistArray.push_back(fhidt3);
  fHistArray.push_back(fhidt1_trl);
  fHistArray.push_back(fhidt2_trl);
  fHistArray.push_back(fhidt3_trl);
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
  ReconstructData();

  TClonesArray *array = fCalibFSDBSD->GetFSDBSDPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtFSDBSDPla *pla = (TArtFSDBSDPla*)array->At(i);
    Double_t id = pla->GetID();
    Double_t T1Raw = pla->GetT1Raw();
    Double_t T2Raw = pla->GetT2Raw();
    Double_t T3Raw = pla->GetT3Raw();
    Double_t T1Raw_trl = pla->GetT1RawTrail();
    Double_t T2Raw_trl = pla->GetT2RawTrail();
    Double_t T3Raw_trl = pla->GetT3RawTrail();

    fhidt1->Fill(id,T1Raw);
    fhidt2->Fill(id,T2Raw);
    fhidt3->Fill(id,T3Raw);
    fhidt1_trl->Fill(id,T1Raw_trl);
    fhidt2_trl->Fill(id,T2Raw_trl);
    fhidt3_trl->Fill(id,T3Raw_trl);
  }
}
//____________________________________________________________________
