#include "CATANADataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtCATANACsI.hh"
#include "TArtCATANAParameters.hh"
#include "TArtCATANACsIPara.hh"

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

  fhidAraw = new TH2D("catana_idAraw","CATANA ID Araw",10,0.5,10.5,100,0,80000);// up to 10 is temp
  fhidTraw = new TH2D("catana_idTraw","CATANA ID Traw",10,0.5,10.5,100,0,80000);
  fhidAcal = new TH2D("catana_idAcal","CATANA ID Acal",10,0.5,10.5,100,0,4000);
  fhidTcal = new TH2D("catana_idTcal","CATANA ID Tcal",10,0.5,10.5,100,0,4000);
  fhidAdop = new TH2D("catana_idAdop","CATANA Adop QR",10,0.5,10.5,100,0,4000);
  fhidAdopBe = new TH2D("catana_idAdopBe","CATANA ID AdopBe",10,0.5,10.5,100,0,4000);
  fhidAdopLi = new TH2D("catana_idAdopLi","CATANA ID AdopLi",10,0.5,10.5,100,0,4000);
  fhidAp = new TH2D("catana_idAp","CATANA ID Ap",10,0.5,10.5,100,0,4000);

  fHistArray.push_back(fhidAraw);
  fHistArray.push_back(fhidTraw);
  fHistArray.push_back(fhidAcal);
  fHistArray.push_back(fhidTcal);
  fHistArray.push_back(fhidAdop);
  fHistArray.push_back(fhidAdopBe);
  fHistArray.push_back(fhidAdopLi);
  fHistArray.push_back(fhidAp);

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
    TArtCATANACsI *ca = (TArtCATANACsI*)array->At(i);
    Double_t id = ca->GetID();
    Double_t Araw = ca->GetRawADC();
    Double_t Traw = ca->GetRawTDC();
    Double_t Acal = ca->GetEnergy();
    Double_t Tcal = ca->GetTime();
    Double_t Adop = ca->GetDoppCorEnergy();
//    Double_t AdopBe = ca->GetTLRaw();
//    Double_t AdopLi = ca->GetTLRaw();
//    Double_t Ap = ca->GetTLRaw();

    fhidAraw->Fill(id,Araw);
    fhidTraw->Fill(id,Traw);
    fhidAcal->Fill(id,Acal);
    fhidTcal->Fill(id,Tcal);
    fhidAdop->Fill(id,Adop);
//    fhidAdopBe->Fill(id,AdopBe);
//    fhidAdopLi->Fill(id,AdopLi);
//    fhidAp->Fill(id,Ap);
  }
}
