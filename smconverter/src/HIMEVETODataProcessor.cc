#include "HIMEVETODataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"

//____________________________________________________________________
void HIMEVETODataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibHIMEVETO = new TArtCalibHIMEVETO;
  fCalibReady = true;
}
//____________________________________________________________________
void HIMEVETODataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibHIMEVETO->GetHIMEVETOPlaArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);
}
//____________________________________________________________________
void HIMEVETODataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtl0 = new TH2D("nebula_idtl0","HIMEVETO ID Tleading0",3,0.5,3.5,100,0,4000);
  fhidtl1 = new TH2D("nebula_idtl1","HIMEVETO ID Tleading1",3,0.5,3.5,100,0,4000);
  fhidtt0 = new TH2D("nebula_idtt0","HIMEVETO ID Ttrailing0",3,0.5,3.5,100,0,4000);
  fhidtt1 = new TH2D("nebula_idtt1","HIMEVETO ID Ttrailing1",3,0.5,3.5,100,0,4000);
  fhidtot0 = new TH2D("nebula_idtot0","HIMEVETO ID ToT0",3,0.5,3.5,100,0,4000);
  fhidtot1 = new TH2D("nebula_idtot1","HIMEVETO ID ToT1",3,0.5,3.5,100,0,4000);

  fHistArray.push_back(fhidtl0);
  fHistArray.push_back(fhidtl1);
  fHistArray.push_back(fhidtt0);
  fHistArray.push_back(fhidtt1);
  fHistArray.push_back(fhidtot0);
  fHistArray.push_back(fhidtot1);
}
//____________________________________________________________________
void HIMEVETODataProcessor::ClearData()
{
  fCalibHIMEVETO->ClearData();
}
//____________________________________________________________________
void HIMEVETODataProcessor::ReconstructData()
{
  fCalibHIMEVETO->ReconstructData();
}
//____________________________________________________________________
void HIMEVETODataProcessor::FillHistograms()
{
  TClonesArray *array = fCalibHIMEVETO->GetHIMEVETOPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtHIMEVETOPla *pla = (TArtHIMEVETOPla*)array->At(i);
    Double_t id = pla->fID;
    Double_t tl0 = pla->fT_LERaw[0];
    Double_t tl1 = pla->fT_LERaw[1];
    Double_t tt0 = pla->fT_TERaw[0];
    Double_t tt1 = pla->fT_TERaw[1];
    Double_t tot0 = pla->fTOTCal[0];
    Double_t tot1 = pla->fTOTCal[1];

    fhidtl0->Fill(id,tl0);
    fhidtl1->Fill(id,tl1);
    fhidtt0->Fill(id,tt0);
    fhidtt1->Fill(id,tt1);
    fhidtot0->Fill(id,tot0);
    fhidtot1->Fill(id,tot1);
  }
}
//____________________________________________________________________
