#include "PDCDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void PDCDataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibPDCHit = new TArtCalibPDCHit;
  fCalibPDCTrack = new TArtCalibPDCTrack;


  fCalibReady = true;
}
//____________________________________________________________________
void PDCDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  //TClonesArray *pdchit_array = fCalibPDCHit->GetDCHitArray();
  TClonesArray *pdctrack_array = fCalibPDCTrack->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &pdctrack_array);
}
//____________________________________________________________________
void PDCDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_pdc = new TH2D("pdc_idt","PDC ID Traw",816,0.5,816.5,100,0,3000);
  fhidtot_pdc = new TH2D("pdc_idtot","PDC ID TOT raw",816,0.5,816.5,100,0,2000);
  fhxy_pdc = new TH2D("pdc_xy","PDC XY",100,-900,900, 100,-450,450);

  fHistArray.push_back(fhidt_pdc);
  fHistArray.push_back(fhidtot_pdc);
  fHistArray.push_back(fhxy_pdc);

}
//____________________________________________________________________
void PDCDataProcessor::ClearData()
{
  fCalibPDCHit->ClearData();
  fCalibPDCTrack->ClearData();
}
//____________________________________________________________________
void PDCDataProcessor::ReconstructData()
{
  fCalibPDCHit->ReconstructData();
  fCalibPDCTrack->ReconstructData();
}
//____________________________________________________________________
void PDCDataProcessor::FillHistograms()
{  

  // PDC Hit
  TClonesArray *hit_array = fCalibPDCHit->GetDCHitArray();
  int n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    Double_t trail = hit->GetTrailTDC();
    fhidt_pdc->Fill(id,traw);
    fhidtot_pdc->Fill(id,traw-trail);
  }


  // PDC Track
  TClonesArray *PDCTracks = fCalibPDCTrack->GetDCTrackArray();
  TArtDCTrack *track = (TArtDCTrack*)PDCTracks->At(0);
  if (track!=0){
    Double_t x = track->GetPosition(0);
    Double_t y = track->GetPosition(1);
//    Double_t a = TMath::ATan(track->GetAngle(0))*1000.;// mrad
//    Double_t b = TMath::ATan(track->GetAngle(1))*1000.;
    fhxy_pdc->Fill(x,y);
  }

}
//____________________________________________________________________


