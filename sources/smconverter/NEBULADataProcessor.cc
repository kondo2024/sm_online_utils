#include "NEBULADataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"

//____________________________________________________________________
void NEBULADataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibNEBULA = new TArtCalibNEBULA;
  fCalibReady = true;
}
//____________________________________________________________________
void NEBULADataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibNEBULA->GetNEBULAPlaArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);
  fTree->Branch("NEBULA.VETO_fired", &fVeto_fired,"VETO_fired/O");

  if (fIncludeHPC){
    fCalibNEBULAHPC = new TArtCalibNEBULAHPC;
    TClonesArray *hpc_array = fCalibNEBULAHPC->GetNEBULAHPCArray();
    fTree->Branch("HPC", &hpc_array);
  }
}
//____________________________________________________________________
void NEBULADataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtu = new TH2D("nebula_idtu","NEBULA ID TuRaw",184,0.5,184.5,100,0,40000);
  fhidtd = new TH2D("nebula_idtd","NEBULA ID TdRaw",184,0.5,184.5,100,0,40000);
  fhidqu = new TH2D("nebula_idqu","NEBULA ID QuRaw",144,0.5,144.5,100,0,4000);
  fhidqd = new TH2D("nebula_idqd","NEBULA ID QdRaw",144,0.5,144.5,100,0,4000);

  fHistArray.push_back(fhidtu);
  fHistArray.push_back(fhidtd);
  fHistArray.push_back(fhidqu);
  fHistArray.push_back(fhidqd);

  // under construction
}
//____________________________________________________________________
void NEBULADataProcessor::ClearData()
{
  fCalibNEBULA->ClearData();
  if (fIncludeHPC){
    fCalibNEBULAHPC->ClearData();
  }  
}
//____________________________________________________________________
void NEBULADataProcessor::ReconstructData()
{
  fCalibNEBULA->ReconstructData();
  if (fIncludeHPC){
    fCalibNEBULAHPC->ReconstructData();
  }  
  fVeto_fired = false ;
  TClonesArray *array = fCalibNEBULA->GetNEBULAPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)array->At(i);
    Double_t id = pla->GetID();
    Double_t turaw = pla->GetTURaw();
    Double_t tdraw = pla->GetTDRaw();
    if(id>140 && turaw>0 && tdraw>0){
      fVeto_fired = true ;
      return ;
    }
  }

}
//____________________________________________________________________
void NEBULADataProcessor::FillHistograms()
{
  TClonesArray *array = fCalibNEBULA->GetNEBULAPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtNEBULAPla *pla = (TArtNEBULAPla*)array->At(i);
    Double_t id = pla->GetID();
    Double_t turaw = pla->GetTURaw();
    Double_t tdraw = pla->GetTDRaw();
    Double_t quraw = pla->GetQURaw();
    Double_t qdraw = pla->GetQDRaw();

    fhidtu->Fill(id,turaw);
    fhidtd->Fill(id,tdraw);
    fhidqu->Fill(id,quraw);
    fhidqd->Fill(id,qdraw);
  }
}
//____________________________________________________________________
