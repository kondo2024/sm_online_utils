#include "FDC0DataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void FDC0DataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibFDC0Hit = new TArtCalibFDC0Hit;
  fCalibFDC0Track = new TArtCalibFDC0Track;

  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void FDC0DataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *fdc0track_array = fCalibFDC0Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &fdc0track_array);
}
//____________________________________________________________________
void FDC0DataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_fdc0 = new TH2D("fdc0_idtu","FDC0 ID Traw",128,0.5,128.5,100,0,10000);
  fhxy_fdc0 = new TH2D("fdc0_xy","FDC0 XY",100,-80,80, 100,-80,80);

  fHistArray.push_back(fhidt_fdc0);
  fHistArray.push_back(fhxy_fdc0);

}
//____________________________________________________________________
void FDC0DataProcessor::ClearData()
{
  fCalibFDC0Hit->ClearData();
  fCalibFDC0Track->ClearData();
}
//____________________________________________________________________
void FDC0DataProcessor::ReconstructData()
{
  fCalibFDC0Hit->ReconstructData();
  fCalibFDC0Track->ReconstructData();
}
//____________________________________________________________________
void FDC0DataProcessor::FillHistograms()
{  

  // FDC0 Hit
  TClonesArray *hit_array = fCalibFDC0Hit->GetDCHitArray();
  int n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    fhidt_fdc0->Fill(id,traw);
  }


  // FDC0 Track
  TClonesArray *FDC0Tracks = fCalibFDC0Track->GetDCTrackArray();
  
  if (FDC0Tracks) {
    Int_t FDC0NumberOfTracks = FDC0Tracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;
//    std::cout << BDC1NumberOfTracks << std::endl;
    if(FDC0NumberOfTracks > 0) {
      TArtDCTrack *TrackFDC0;
      
      for(Int_t i = 0; i<FDC0NumberOfTracks; i++) {
        TrackFDC0 = (TArtDCTrack *)FDC0Tracks->At(i);
        
	if(TrackFDC0) {

	  TempXPosition = TrackFDC0->GetPosition(0);
  	  TempYPosition = TrackFDC0->GetPosition(1);
	  TempChi2 = TrackFDC0->GetChi2() / (Double_t)TrackFDC0->GetNDF();
 
	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fFDC0_X = TempXPosition;
  	      fFDC0_ThetaX = TMath::ATan(TrackFDC0->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fFDC0_Y = TempYPosition;
  	      fFDC0_ThetaY = TMath::ATan(TrackFDC0->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

    fhxy_fdc0->Fill(fFDC0_X,fFDC0_Y); 
    }  
  }



}
//____________________________________________________________________

void FDC0DataProcessor::LoadDCTDCDistribution() {
  TDirectory *pwd = gDirectory;

  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t FDC0NumberOfLayers = 8;
   
    for(Int_t i=0; i<FDC0NumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hfdc0tdc%d",i));

      if(Hist1D) {
	fCalibFDC0Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hfdc0tdc%d",i) << "\e  [0m" << std::endl;
    }
  }

  pwd->cd();
}

