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

  TClonesArray *pdchit_array = fCalibPDCHit->GetDCHitArray();
  TClonesArray *pdctrack_array = fCalibPDCTrack->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &pdctrack_array);
}
//____________________________________________________________________
void PDCDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_pdc = new TH2D("pdc_idtu","PDC ID Traw",816,0.5,816.5,100,0,3000);
  fhxy_pdc = new TH2D("pdc_xy","PDC XY",100,-900,900, 100,-450,450);

  fHistArray.push_back(fhidt_pdc);
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
    fhidt_pdc->Fill(id,traw);
  }


  // PDC Track
  TClonesArray *PDCTracks = fCalibPDCTrack->GetDCTrackArray();
  
  if (PDCTracks) {
    Int_t PDCNumberOfTracks = PDCTracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;
//    std::cout << BDC1NumberOfTracks << std::endl;
    if(PDCNumberOfTracks > 0) {
      TArtDCTrack *TrackPDC;
      
      for(Int_t i = 0; i<PDCNumberOfTracks; i++) {
        TrackPDC = (TArtDCTrack *)PDCTracks->At(i);
        
	if(TrackPDC) {

	  TempXPosition = TrackPDC->GetPosition(0);
  	  TempYPosition = TrackPDC->GetPosition(1);
	  TempChi2 = TrackPDC->GetChi2() / (Double_t)TrackPDC->GetNDF();
 
	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fPDC_X = TempXPosition;
  	      fPDC_ThetaX = TMath::ATan(TrackPDC->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fPDC_Y = TempYPosition;
  	      fPDC_ThetaY = TMath::ATan(TrackPDC->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

    fhxy_pdc->Fill(fPDC_X,fPDC_Y); 
    }  
  }



}
//____________________________________________________________________


