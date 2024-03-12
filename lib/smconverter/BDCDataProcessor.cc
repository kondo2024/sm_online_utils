#include "BDCDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void BDCDataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter("db/SAMURAIBDC1.xml");
  smprm->LoadParameter("db/SAMURAIBDC2.xml");

  fCalibBDC1Hit = new TArtCalibBDC1Hit;
  fCalibBDC2Hit = new TArtCalibBDC2Hit;
  fCalibBDC1Track = new TArtCalibBDC1Track;
  fCalibBDC2Track = new TArtCalibBDC2Track;

  // under construction for loading TDC distributions for 
  // drift time -> drift distance calibration

  //TFile *file = new TFile(fTDCDistFileName.Data());
  // temptemptemp...

  fCalibReady = true;
}
//____________________________________________________________________
void BDCDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *bdc1hit_array = fCalibBDC1Hit->GetDCHitArray();
  TClonesArray *bdc2hit_array = fCalibBDC2Hit->GetDCHitArray();
  TClonesArray *bdc1track_array = fCalibBDC1Track->GetDCTrackArray();
  TClonesArray *bdc2track_array = fCalibBDC2Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &bdc1hit_array);// is it necessary?
  fTree->Branch(fBranchName.Data(), &bdc2hit_array);// is it necessary?
  fTree->Branch(fBranchName.Data(), &bdc1track_array);
  fTree->Branch(fBranchName.Data(), &bdc2track_array);
}
//____________________________________________________________________
void BDCDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_bdc1 = new TH2D("bdc1_idtu","BDC1 ID T",128,0.5,128.5,100,0,10000);
  fhidt_bdc2 = new TH2D("bdc2_idtu","BDC2 ID T",128,0.5,128.5,100,0,10000);
  fhxy_bdc1 = new TH2D("bdc1_xy","BDC1 XY",100,-80,80, 100,-80,80);
  fhxy_bdc2 = new TH2D("bdc2_xy","BDC2 XY",100,-80,80, 100,-80,80);

  fHistArray.push_back(fhidt_bdc1);
  fHistArray.push_back(fhidt_bdc2);
  fHistArray.push_back(fhxy_bdc1);
  fHistArray.push_back(fhxy_bdc2);

}
//____________________________________________________________________
void BDCDataProcessor::ClearData()
{
  fCalibBDC1Hit->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Track->ClearData();
}
//____________________________________________________________________
void BDCDataProcessor::ReconstructData()
{
  fCalibBDC1Hit->ReconstructData();
  fCalibBDC2Hit->ReconstructData();
  fCalibBDC1Track->ReconstructData();
  fCalibBDC2Track->ReconstructData();
}
//____________________________________________________________________
void BDCDataProcessor::FillHistograms()
{
  // BDC1 Hit
  TClonesArray *hit_array = fCalibBDC1Hit->GetDCHitArray();
  int n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    fhidt_bdc1->Fill(id,traw);
  }

  // BDC2 Hit
  hit_array = fCalibBDC2Hit->GetDCHitArray();
  n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    fhidt_bdc2->Fill(id,traw);
  }

  // temp, under construction

  // BDC1 Track
  TClonesArray *track_array = fCalibBDC1Track->GetDCTrackArray();
  n=track_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCTrack *tr = (TArtDCTrack*)track_array->At(i);
//    Double_t x = ;
//    Double_t y = ;
//      fhxy_bdc1->Fill(x,y);
  }

  // BDC2 Track


}
//____________________________________________________________________
