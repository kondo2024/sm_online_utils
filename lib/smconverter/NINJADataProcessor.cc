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

  fhidlelu = new TH2D("NINJA_idlelu","NINJA ID LELU",30,0.5,30.5,100,0,80000);
  fhidlerd = new TH2D("NINJA_idlerd","NINJA ID LERD",30,0.5,30.5,100,0,80000);
  fhidtotlu = new TH2D("NINJA_idtotlu","NINJA ID TOTLU",30,0.5,30.5,100,0,100);// up to 10 is temp
  fhidtotrd = new TH2D("NINJA_idtotrd","NINJA ID TOTRD",30,0.5,30.5,100,0,100);
  fhidtime = new TH2D("NINJA_idtime","NINJA ID time average",30,0.5,30.5,100,0,100);
  fhidTOT = new TH2D("NINJA_idTOT","NINJA ID TOT average",30,0.5,30.5,100,0,100);
  fhidxidy = new TH2D("NINJA_idxidy","NINJA IDX IDY",30,0.5,30.5,30,0.5,30.5);

  fHistArray.push_back(fhidlelu);
  fHistArray.push_back(fhidlerd);
  fHistArray.push_back(fhidtotlu);
  fHistArray.push_back(fhidtotrd);
  fHistArray.push_back(fhidtime);
  fHistArray.push_back(fhidTOT);
  fHistArray.push_back(fhidxidy);

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
  int id_x = -9999;
  int id_y = -9999;

  for (int i=0;i<n;++i){
    TArtNINJAPla *NINJA = (TArtNINJAPla*)array->At(i);
    Double_t id = NINJA->fID;
    Double_t LERawLU = NINJA->fT_LERaw[0];
    Double_t LERawRD = NINJA->fT_LERaw[1];
    Double_t TOTLU = NINJA->fTOTCal[0];
    Double_t TOTRD = NINJA->fTOTCal[1];
    Double_t avgtime = (TOTLU + TOTRD)/2.;
    Double_t avgTOT = pow(TOTLU * TOTRD,0.5);


    fhidlelu->Fill(id,LERawLU);
    fhidlerd->Fill(id,LERawRD);
    fhidtotlu->Fill(id,TOTLU);
    fhidtotrd->Fill(id,TOTRD);
    fhidtime->Fill(id,avgtime);
    fhidTOT->Fill(id,avgTOT);

    if (id < 18 ) id_x = id;
    if (id > 17 ) id_y = id;

    if (id_x >-1 && id_y>-1) fhidxidy->Fill(id_x,id_y);
 
 }
}
//____________________________________________________________________
