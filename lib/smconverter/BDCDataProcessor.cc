#include "BDCDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void BDCDataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbfilename1);
  smprm->LoadParameter(fdbfilename2);

  fCalibBDC1Hit = new TArtCalibBDC1Hit;
  fCalibBDC2Hit = new TArtCalibBDC2Hit;
  fCalibBDC1Track = new TArtCalibBDC1Track;
  fCalibBDC2Track = new TArtCalibBDC2Track;

  LoadDCTDCDistribution();

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
/*
  TClonesArray *track_array = fCalibBDC1Track->GetDCTrackArray();
  n=track_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCTrack *tr = (TArtDCTrack*)track_array->At(i);

//    Double_t x = ;
//    Double_t y = ;
//      fhxy_bdc1->Fill(x,y);
  }
*/
  // BDC2 Track


  // BDC1 Track
  TClonesArray *BDC1Tracks = fCalibBDC1Track->GetDCTrackArray();
  
  if (BDC1Tracks) {
    Int_t BDC1NumberOfTracks = BDC1Tracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;
//    std::cout << BDC1NumberOfTracks << std::endl;
    if(BDC1NumberOfTracks > 0) {
      TArtDCTrack *TrackBDC1;
      
      for(Int_t i = 0; i<BDC1NumberOfTracks; i++) {
        TrackBDC1 = (TArtDCTrack *)BDC1Tracks->At(i);
        
	if(TrackBDC1) {

	  TempXPosition = TrackBDC1->GetPosition(0);
  	  TempYPosition = TrackBDC1->GetPosition(1);
	  TempChi2 = TrackBDC1->GetChi2() / (Double_t)TrackBDC1->GetNDF();
 
	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fBDC1_X = TempXPosition;
  	      fBDC1_ThetaX = TMath::ATan(TrackBDC1->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fBDC1_Y = TempYPosition;
  	      fBDC1_ThetaY = TMath::ATan(TrackBDC1->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

    fhxy_bdc1->Fill(fBDC1_X,fBDC1_Y); 
    }  
  }

  // BDC2 Track
  TClonesArray *BDC2Tracks = fCalibBDC2Track->GetDCTrackArray();

  if (BDC2Tracks) {
    Int_t BDC2NumberOfTracks = BDC2Tracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;

    if(BDC2NumberOfTracks > 0) {
      TArtDCTrack *TrackBDC2;
      
      for(Int_t i = 0; i<BDC2NumberOfTracks; i++) {
        TrackBDC2 = (TArtDCTrack *)BDC2Tracks->At(i);
        
	if(TrackBDC2) {

	  TempXPosition = TrackBDC2->GetPosition(0);
  	  TempYPosition = TrackBDC2->GetPosition(1);
	  TempChi2 = TrackBDC2->GetChi2() / (Double_t)TrackBDC2->GetNDF();

	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fBDC2_X = TempXPosition;
  	      fBDC2_ThetaX = TMath::ATan(TrackBDC2->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fBDC2_Y = TempYPosition;
  	      fBDC2_ThetaY = TMath::ATan(TrackBDC2->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      fhxy_bdc2->Fill(fBDC2_X,fBDC2_Y);  
    }      
  }


}
//____________________________________________________________________

void BDCDataProcessor::LoadDCTDCDistribution() {

  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t BDCNumberOfLayers = 8;

    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hbdc1tdc%d",i));

      if(Hist1D) {
	fCalibBDC1Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hbdc1tdc%d",i) << "\e  [0m" << std::endl;
    }

    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hbdc2tdc%d",i));

      if(Hist1D) {
	fCalibBDC2Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hbdc2tdc%d",i) << "\e  [0m" << std::endl;
    }
  }

}

