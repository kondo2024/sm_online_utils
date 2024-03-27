#include "HODPlaDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtHODPla.hh"
#include "TArtSAMURAIParameters.hh"
//____________________________________________________________________
void HODPlaDataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(Form("%sSAMURAIHOD.xml",fdbpath));

  fCalibHODPla = new TArtCalibHODPla;
  fCalibReady = true;
}
//____________________________________________________________________
void HODPlaDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibHODPla->GetHODPlaArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);

}
//____________________________________________________________________
void HODPlaDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtl = new TH2D("HODPla_idtl","HODPla ID TL",40,0.5,40.5,100,0,80000);// up to 10 is temp
  fhidtr = new TH2D("HODPla_idtr","HODPla ID TR",40,0.5,40.5,100,0,80000);
  fhidql = new TH2D("HODPla_idql","HODPla ID QL",40,0.5,40.5,100,0,10000);
  fhidqr = new TH2D("HODPla_idqr","HODPla ID QR",40,0.5,40.5,100,0,10000);

  fHistArray.push_back(fhidtl);
  fHistArray.push_back(fhidtr);
  fHistArray.push_back(fhidql);
  fHistArray.push_back(fhidqr);

  // under construction
}
//____________________________________________________________________
void HODPlaDataProcessor::ClearData()
{
  fCalibHODPla->ClearData();
}
//____________________________________________________________________
void HODPlaDataProcessor::ReconstructData()
{
  fCalibHODPla->ReconstructData();
}
//____________________________________________________________________
void HODPlaDataProcessor::FillHistograms()
{
  ReconstructData();

  TClonesArray *array = fCalibHODPla->GetHODPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtHODPla *HODPla = (TArtHODPla*)array->At(i);
    Double_t id = HODPla->GetID();
    Double_t TURaw = HODPla->GetTURaw();
    Double_t TDRaw = HODPla->GetTDRaw();
    Double_t QURaw = HODPla->GetQURaw();
    Double_t QDRaw = HODPla->GetQDRaw();

    fhidtl->Fill(id,TURaw);
    fhidtr->Fill(id,TDRaw);
    fhidql->Fill(id,QURaw);
    fhidqr->Fill(id,QDRaw);
  }
}
//____________________________________________________________________
