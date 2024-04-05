#include "PlasticDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtPlastic.hh"
#include "TArtBigRIPSParameters.hh"
//____________________________________________________________________
void PlasticDataProcessor::PrepareCalib()
{
  TArtBigRIPSParameters *brprm = TArtBigRIPSParameters::Instance();
  brprm->LoadParameter(fdbFileName);

  fCalibPlastic = new TArtCalibPlastic;
  fCalibReady = true;
}
//____________________________________________________________________
void PlasticDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibPlastic->GetPlasticArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);

}
//____________________________________________________________________
void PlasticDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtl = new TH2D("pla_idtl","Plastic ID TLRaw",10,0.5,10.5,100,0,80000);// up to 10 is temp
  fhidtr = new TH2D("pla_idtr","Plastic ID TRRaw",10,0.5,10.5,100,0,80000);
  fhidql = new TH2D("pla_idql","Plastic ID QLRaw",10,0.5,10.5,100,0,4000);
  fhidqr = new TH2D("pla_idqr","Plastic ID QRRaw",10,0.5,10.5,100,0,4000);

  fHistArray.push_back(fhidtl);
  fHistArray.push_back(fhidtr);
  fHistArray.push_back(fhidql);
  fHistArray.push_back(fhidqr);

  // under construction
}
//____________________________________________________________________
void PlasticDataProcessor::ClearData()
{
  fCalibPlastic->ClearData();
}
//____________________________________________________________________
void PlasticDataProcessor::ReconstructData()
{
  fCalibPlastic->ReconstructData();
}
//____________________________________________________________________
void PlasticDataProcessor::FillHistograms()
{
  TClonesArray *array = fCalibPlastic->GetPlasticArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtPlastic *pla = (TArtPlastic*)array->At(i);
    Double_t id = pla->GetID();
    Double_t tlraw = pla->GetTLRaw();
    Double_t trraw = pla->GetTRRaw();
    Double_t qlraw = pla->GetQLRaw();
    Double_t qrraw = pla->GetQRRaw();

    fhidtl->Fill(id,tlraw);
    fhidtr->Fill(id,trraw);
    fhidql->Fill(id,qlraw);
    fhidqr->Fill(id,qrraw);
  }
}
//____________________________________________________________________
