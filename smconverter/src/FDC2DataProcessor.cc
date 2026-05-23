#include "FDC2DataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TDirectory.h"
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

  fCalibFDC2Track->SetTDCWindow(0,3000);// temp for 2025MS (2025.05.29)
  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void FDC2DataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *fdc2track_array = fCalibFDC2Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch("FDC2X", &fFDC2_X);
  fTree->Branch("FDC2Y", &fFDC2_Y);
  fTree->Branch("FDC2A", &fFDC2_A);
  fTree->Branch("FDC2B", &fFDC2_B);
}
//____________________________________________________________________
void FDC2DataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_fdc2 = new TH2D("fdc2_idt","FDC2 ID Traw",1568,0.5,1568.5,100,0,5000);
  fhxy_fdc2 = new TH2D("fdc2_xy","FDC2 XY",100,-1200,1200, 100,-450,450);
  fhxa_fdc2 = new TH2D("fdc2_xa","FDC2 XA",100,-1200,1200, 100,-0.450,0.450);
  fhyb_fdc2 = new TH2D("fdc2_yb","FDC2 YB",100,-1200,1200, 100,-0.450,0.450);

  fHistArray.push_back(fhidt_fdc2);
  fHistArray.push_back(fhxy_fdc2);
  fHistArray.push_back(fhxa_fdc2);
  fHistArray.push_back(fhyb_fdc2);

}
//____________________________________________________________________
void FDC2DataProcessor::ClearData()
{
  fCalibFDC2Hit->ClearData();
  fCalibFDC2Track->ClearData();
  fFDC2_X = -9999;
  fFDC2_Y = -9999;
  fFDC2_A = -9999;
  fFDC2_B = -9999;
}
//____________________________________________________________________
void FDC2DataProcessor::ReconstructData()
{
  fCalibFDC2Hit->ReconstructData();

  if (!fDoTracking) return;
  fCalibFDC2Track->ReconstructData();

  // FDC2 Track
  TClonesArray *FDC2Tracks = fCalibFDC2Track->GetDCTrackArray();

  if (FDC2Tracks) {
    Int_t FDC2NumberOfTracks = FDC2Tracks->GetEntries();
    if(FDC2NumberOfTracks > 0) {

      TArtDCTrack *TrackFDC2 = (TArtDCTrack *)FDC2Tracks->At(0);
      fFDC2_X = TrackFDC2->GetPosition(0);
      fFDC2_A = TMath::ATan(TrackFDC2->GetAngle(0));
      fFDC2_Y = TrackFDC2->GetPosition(1);
      fFDC2_B = TMath::ATan(TrackFDC2->GetAngle(1));
    }      
  }
  
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

  if (!fDoTracking) return;
  fhxy_fdc2->Fill(fFDC2_X,fFDC2_Y);  
  fhxa_fdc2->Fill(fFDC2_X,fFDC2_A);  
  fhyb_fdc2->Fill(fFDC2_Y,fFDC2_B);  
  
}
//____________________________________________________________________

void FDC2DataProcessor::LoadDCTDCDistribution() {
  TDirectory *pwd = gDirectory;

  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile->IsOpen()) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t FDC2NumberOfLayers = 14;
   

    for(Int_t i=0; i<FDC2NumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hfdc2tdc%d",i));

      if(Hist1D) {
	fCalibFDC2Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hfdc2tdc%d",i) << "\e[0m" << std::endl;
    }
  }else{
    fDoTracking = false;
    std::cout << "\e[35m " << "Skip FDC2 tracking " << "\e[0m" << std::endl;
  }

  pwd->cd();

}

