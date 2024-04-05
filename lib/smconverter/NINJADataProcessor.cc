#include "NINJADataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtNINJAPla.hh"
#include "TArtSAMURAIParameters.hh"
//____________________________________________________________________
void NINJADataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibNINJA = new TArtCalibNINJA;
  fCalibReady = true;
}
//____________________________________________________________________
void NINJADataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *array = fCalibNINJA->GetNINJAPlaArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &array);

}
//____________________________________________________________________
void NINJADataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidtl = new TH2D("NINJA_idtotu","NINJA ID TOTU",30,0.5,30.5,100,0,80000);// up to 10 is temp
  fhidtr = new TH2D("NINJA_idtotd","NINJA ID TOTD",30,0.5,30.5,100,0,80000);
  fhidql = new TH2D("NINJA_idqu","NINJA ID QU",30,0.5,30.5,100,0,10000);
  fhidqr = new TH2D("NINJA_idqd","NINJA ID QD",30,0.5,30.5,100,0,10000);

  fHistArray.push_back(fhidtl);
  fHistArray.push_back(fhidtr);
  fHistArray.push_back(fhidql);
  fHistArray.push_back(fhidqr);

  // under construction
}
//____________________________________________________________________
void NINJADataProcessor::ClearData()
{
  fCalibNINJA->ClearData();
}
//____________________________________________________________________
void NINJADataProcessor::ReconstructData()
{
  fCalibNINJA->ReconstructData();
}
//____________________________________________________________________
void NINJADataProcessor::FillHistograms()
{
  ReconstructData();

  TClonesArray *array = fCalibNINJA->GetNINJAPlaArray();
  int n=array->GetEntries();
  for (int i=0;i<n;++i){
    TArtNINJAPla *NINJA = (TArtNINJAPla*)array->At(i);
    Double_t id = NINJA->fID;
    Double_t TOTUCal = NINJA->fTOTCal[0];
    Double_t TOTDCal = NINJA->fTOTCal[1];
    Double_t QUCal = NINJA->fQCal[0];
    Double_t QDCal = NINJA->fQCal[1];

    fhidtl->Fill(id,TOTUCal);
    fhidtr->Fill(id,TOTDCal);
    fhidql->Fill(id,QUCal);
    fhidqr->Fill(id,QDCal);
  }
}
//____________________________________________________________________
