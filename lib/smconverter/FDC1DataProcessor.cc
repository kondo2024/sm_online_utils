#include "FDC1DataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void FDC1DataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibFDC1Hit = new TArtCalibFDC1Hit;
  fCalibFDC1Track = new TArtCalibFDC1Track;

  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void FDC1DataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *fdc1hit_array = fCalibFDC1Hit->GetDCHitArray();
  TClonesArray *fdc1track_array = fCalibFDC1Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &fdc1hit_array);// is it necessary?
  fTree->Branch(fBranchName.Data(), &fdc1track_array);
}
//____________________________________________________________________
void FDC1DataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_fdc1 = new TH2D("fdc1_idtu","FDC1 ID Traw",448,0.5,448.5,100,0,3000);
  fhxy_fdc1 = new TH2D("fdc1_xy","FDC1 XY",100,-350,350, 100,-200,200);

  fHistArray.push_back(fhidt_fdc1);
  fHistArray.push_back(fhxy_fdc1);

}
//____________________________________________________________________
void FDC1DataProcessor::ClearData()
{
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
}
//____________________________________________________________________
void FDC1DataProcessor::ReconstructData()
{
  fCalibFDC1Hit->ReconstructData();
  fCalibFDC1Track->ReconstructData();
}
//____________________________________________________________________
void FDC1DataProcessor::FillHistograms()
{  

  // FDC1 Hit
  TClonesArray *hit_array = fCalibFDC1Hit->GetDCHitArray();
  int n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    fhidt_fdc1->Fill(id,traw);
  }


  // FDC1 Track
  TClonesArray *FDC1Tracks = fCalibFDC1Track->GetDCTrackArray();

  if (FDC1Tracks) {
    Int_t FDC1NumberOfTracks = FDC1Tracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;

    if(FDC1NumberOfTracks > 0) {
      TArtDCTrack *TrackFDC1;
      
      for(Int_t i = 0; i<FDC1NumberOfTracks; i++) {
        TrackFDC1 = (TArtDCTrack *)FDC1Tracks->At(i);
        
	if(TrackFDC1) {

	  TempXPosition = TrackFDC1->GetPosition(0);
  	  TempYPosition = TrackFDC1->GetPosition(1);
	  TempChi2 = TrackFDC1->GetChi2() / (Double_t)TrackFDC1->GetNDF();

	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fFDC1_X = TempXPosition;
  	      fFDC1_ThetaX = TMath::ATan(TrackFDC1->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fFDC1_Y = TempYPosition;
  	      fFDC1_ThetaY = TMath::ATan(TrackFDC1->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      fhxy_fdc1->Fill(fFDC1_X,fFDC1_Y);  
    }      
  }


}
//____________________________________________________________________

void FDC1DataProcessor::LoadDCTDCDistribution() {
  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t FDC1NumberOfLayers = 14;
   

    for(Int_t i=0; i<FDC1NumberOfLayers; i++) {
      std::cout << i << std::endl;
      Hist1D = (TH1D*) RootFile->Get(Form("hfdc1tdc%d",i));

      if(Hist1D) {
	fCalibFDC1Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hfdc1tdc%d",i) << "\e  [0m" << std::endl;
    }
  }

}

