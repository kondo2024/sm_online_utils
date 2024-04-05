#include "CATANADataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtCATANACsI.hh"
#include "TArtCATANAParameters.hh"
//____________________________________________________________________
void CATANADataProcessor::PrepareCalib()
{
  TArtCATANAParameters *caprm = TArtCATANAParameters::Instance();
  caprm->LoadParameter(fdbFileName);

  fCalibCATANA = new TArtCalibCATANA;
  fCalibReady = true;
}
//____________________________________________________________________
void CATANADataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibCATANA->GetCsIArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);

}
//____________________________________________________________________
void CATANADataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtl = new TH2D("catana_idtl","CATANA ID TL",10,0.5,10.5,100,0,80000);// up to 10 is temp
  fhidtr = new TH2D("catana_idtr","CATANA ID TR",10,0.5,10.5,100,0,80000);
  fhidql = new TH2D("catana_idql","CATANA ID QL",10,0.5,10.5,100,0,4000);
  fhidqr = new TH2D("catana_idqr","CATANA ID QR",10,0.5,10.5,100,0,4000);

  fHistArray.push_back(fhidtl);
  fHistArray.push_back(fhidtr);
  fHistArray.push_back(fhidql);
  fHistArray.push_back(fhidqr);

  // under construction
}
//____________________________________________________________________
void CATANADataProcessor::ClearData()
{
  fCalibCATANA->ClearData();
}
//____________________________________________________________________
void CATANADataProcessor::ReconstructData()
{
  fCalibCATANA->ReconstructData();
}
//____________________________________________________________________
void CATANADataProcessor::FillHistograms()
{
  TClonesArray *array = fCalibCATANA->GetCsIArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtCalibCATANA *ca = (TArtCalibCATANA*)array->At(i);
/*    Double_t id = ca->GetID();
    Double_t tlraw = ca->GetTLRaw();
    Double_t trraw = ca->GetTRRaw();
    Double_t qlraw = ca->GetQLRaw();
    Double_t qrraw = ca->GetQRRaw();

    fhidtl->Fill(id,tlraw);
    fhidtr->Fill(id,trraw);
    fhidql->Fill(id,qlraw);
    fhidqr->Fill(id,qrraw);
*/  }
}
//____________________________________________________________________
