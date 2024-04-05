#include "FDC2DataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TArtSAMURAIParameters.hh"
#include "TArtDCHit.hh"
#include "TArtDCTrack.hh"

//____________________________________________________________________
void FDC2DataProcessor::PrepareCalib()
{
  TArtSAMURAIParameters *smprm = TArtSAMURAIParameters::Instance();
  smprm->LoadParameter(fdbFileName);

  fCalibFDC2Hit = new TArtCalibFDC2Hit;
  fCalibFDC2Track = new TArtCalibFDC2Track;

  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void FDC2DataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *fdc2hit_array = fCalibFDC2Hit->GetDCHitArray();
  TClonesArray *fdc2track_array = fCalibFDC2Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &fdc2hit_array);// is it necessary?
  fTree->Branch(fBranchName.Data(), &fdc2track_array);
}
//____________________________________________________________________
void FDC2DataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_fdc2 = new TH2D("fdc2_idtu","FDC2 ID Traw",1568,0.5,1568.5,100,0,10000);
  fhxy_fdc2 = new TH2D("fdc2_xy","FDC2 XY",100,-80,80, 100,-80,80);

  fHistArray.push_back(fhidt_fdc2);
  fHistArray.push_back(fhxy_fdc2);

}
//____________________________________________________________________
void FDC2DataProcessor::ClearData()
{
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();
}
//____________________________________________________________________
void FDC2DataProcessor::ReconstructData()
{
  fCalibFDC2Hit->ReconstructData();
  fCalibFDC2Track->ReconstructData();
}
//____________________________________________________________________
void FDC2DataProcessor::FillHistograms()
{  

  // FDC2 Hit
  TClonesArray *hit_array = fCalibFDC2Hit->GetDCHitArray();
  int n=hit_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtDCHit *hit = (TArtDCHit*)hit_array->At(i);
    Double_t id = hit->GetID();
    Double_t traw = hit->GetTDC();
    fhidt_fdc2->Fill(id,traw);
  }


  // FDC2 Track
  TClonesArray *FDC2Tracks = fCalibFDC2Track->GetDCTrackArray();

  if (FDC2Tracks) {
    Int_t FDC2NumberOfTracks = FDC2Tracks->GetEntries();
    Double_t TempXPosition, TempYPosition, TempChi2, MinChi2x =1e6, MinChi2y =1e6;

    if(FDC2NumberOfTracks > 0) {
      TArtDCTrack *TrackFDC2;
      
      for(Int_t i = 0; i<FDC2NumberOfTracks; i++) {
        TrackFDC2 = (TArtDCTrack *)FDC2Tracks->At(i);
        
	if(TrackFDC2) {

	  TempXPosition = TrackFDC2->GetPosition(0);
  	  TempYPosition = TrackFDC2->GetPosition(1);
	  TempChi2 = TrackFDC2->GetChi2() / (Double_t)TrackFDC2->GetNDF();

	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fFDC2_X = TempXPosition;
  	      fFDC2_ThetaX = TMath::ATan(TrackFDC2->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fFDC2_Y = TempYPosition;
  	      fFDC2_ThetaY = TMath::ATan(TrackFDC2->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      fhxy_fdc2->Fill(fFDC2_X,fFDC2_Y);  
    }      
  }


}
//____________________________________________________________________

void FDC2DataProcessor::LoadDCTDCDistribution() {
  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t FDC2NumberOfLayers = 14;
   

    for(Int_t i=0; i<FDC2NumberOfLayers; i++) {
      std::cout << i << std::endl;
      Hist1D = (TH1D*) RootFile->Get(Form("hfdc2tdc%d",i));

      if(Hist1D) {
	fCalibFDC2Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hfdc2tdc%d",i) << "\e  [0m" << std::endl;
    }
  }

}

