#include "BDCDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TDirectory.h"
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
  fTargetTrack = new TArtDCTrack;

  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void BDCDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *bdc1track_array = fCalibBDC1Track->GetDCTrackArray();
  TClonesArray *bdc2track_array = fCalibBDC2Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(Form("%s1",fBranchName.Data()), &bdc1track_array);
  fTree->Branch(Form("%s2",fBranchName.Data()), &bdc2track_array);
  fTree->Branch("Target", &fTargetTrack);
}
//____________________________________________________________________
void BDCDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_bdc1 = new TH2D("bdc1_idtu","BDC1 ID Traw",128,0.5,128.5,100,0,3000);
  fhidt_bdc2 = new TH2D("bdc2_idtu","BDC2 ID Traw",128,0.5,128.5,100,0,3000);
  fhxy_bdc1 = new TH2D("bdc1_xy","BDC1 XY",100,-50,50, 100,-50,50);
  fhxy_bdc2 = new TH2D("bdc2_xy","BDC2 XY",100,-50,50, 100,-50,50);
  fhxy_tgt = new TH2D("tgt_xy","Target XY",100,-50,50, 100,-50,50);
  fhxa_tgt = new TH2D("tgt_xa","Target XA",100,-50,50, 100,-50,50);
  fhyb_tgt = new TH2D("tgt_yb","Target YB",100,-50,50, 100,-50,50);
  fhx_tgt = new TH1D("tgt_x","Target X",100,-50,50);
  fhy_tgt = new TH1D("tgt_y","Target Y",100,-50,50);

  fHistArray.push_back(fhidt_bdc1);
  fHistArray.push_back(fhidt_bdc2);
  fHistArray.push_back(fhxy_bdc1);
  fHistArray.push_back(fhxy_bdc2);
  fHistArray.push_back(fhxy_tgt);
  fHistArray.push_back(fhxa_tgt);
  fHistArray.push_back(fhyb_tgt);
  fHistArray.push_back(fhx_tgt);
  fHistArray.push_back(fhy_tgt);
}
//____________________________________________________________________
void BDCDataProcessor::ClearData()
{
  fCalibBDC1Hit->ClearData();
  fCalibBDC2Hit->ClearData();
  fCalibBDC1Track->ClearData();
  fCalibBDC2Track->ClearData();

  fTargetTrack->SetPosition(0,-9999);
  fTargetTrack->SetPosition(1,-9999);
  fTargetTrack->SetAngle(0,-9999);
  fTargetTrack->SetAngle(1,-9999);

  fBDC1_X = -9999; fBDC1_Y = -9999; fBDC1_ThetaX = -9999; fBDC1_ThetaY = -9999;
  fBDC2_X = -9999; fBDC2_Y = -9999; fBDC2_ThetaX = -9999; fBDC2_ThetaY = -9999;
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

    //std::cout<<fBDC1_X<<" "<<fBDC1_Y<<std::endl;
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

  if(abs(fBDC1_X)<40&&abs(fBDC1_Y)<40&&abs(fBDC2_X)<40&&abs(fBDC2_Y)<40){
    //Target Position
    TVector3 bdc1Position(fBDC1_X,fBDC1_Y,fBDC1_Z);
    TVector3 bdc2Position(fBDC2_X,fBDC2_Y,fBDC2_Z);
    TVector3 beamDirection=bdc2Position-bdc1Position;
    beamDirection=beamDirection.Unit();

    TVector3 targetPosition 
      = bdc1Position +  (fTarget_Z-bdc1Position.Z())/beamDirection.Z()*beamDirection;
	  
    fTargetTrack->SetPosition(targetPosition.X(),0);
    fTargetTrack->SetPosition(targetPosition.Y(),1);
    fTargetTrack->SetAngle(TMath::ATan(beamDirection.X()/beamDirection.Z())*1000.,0);// mrad
    fTargetTrack->SetAngle(TMath::ATan(beamDirection.Y()/beamDirection.Z())*1000.,1);

    fhxy_tgt->Fill(fTargetTrack->GetPosition(0),fTargetTrack->GetPosition(1));
    fhx_tgt->Fill(fTargetTrack->GetPosition(0));
    fhy_tgt->Fill(fTargetTrack->GetPosition(1));

    fhxa_tgt->Fill(fTargetTrack->GetPosition(0),fTargetTrack->GetAngle(0));
    fhyb_tgt->Fill(fTargetTrack->GetPosition(1),fTargetTrack->GetAngle(1));
  }

}
//____________________________________________________________________

void BDCDataProcessor::LoadDCTDCDistribution() {

  TDirectory *pwd = gDirectory;

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
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hbdc1tdc%d",i) << "\e[0m" << std::endl;
    }

    for(Int_t i=0; i<BDCNumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hbdc2tdc%d",i));

      if(Hist1D) {
	fCalibBDC2Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hbdc2tdc%d",i) << "\e[0m" << std::endl;
    }
  }

  pwd->cd();

}

