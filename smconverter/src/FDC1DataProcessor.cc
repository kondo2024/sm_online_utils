#include "FDC1DataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TFile.h"
#include "TDirectory.h"
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

  fCalibFDC1Track->SetTDCWindow(0,3000);// temp for 2025MS (2025.05.29)
  LoadDCTDCDistribution();

  fCalibReady = true;
}
//____________________________________________________________________
void FDC1DataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *fdc1track_array = fCalibFDC1Track->GetDCTrackArray();
  fTree = tree;
  fTree->Branch("FDC1X",&fFDC1_X);
  fTree->Branch("FDC1Y",&fFDC1_Y);
  fTree->Branch("FDC1A",&fFDC1_A);
  fTree->Branch("FDC1B",&fFDC1_B);
}
//____________________________________________________________________
void FDC1DataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhidt_fdc1 = new TH2D("fdc1_idt","FDC1 ID Traw",448,0.5,448.5,100,0,5000);
  fhxy_fdc1 = new TH2D("fdc1_xy","FDC1 XY",100,-150,150, 100,-150,150);
  fhxa_fdc1 = new TH2D("fdc1_xa","FDC1 XA",100,-150,150, 100,-0.10,0.10);
  fhyb_fdc1 = new TH2D("fdc1_yb","FDC1 YB",100,-150,150, 100,-0.10,0.10);

  fHistArray.push_back(fhidt_fdc1);
  fHistArray.push_back(fhxy_fdc1);
  fHistArray.push_back(fhxa_fdc1);
  fHistArray.push_back(fhyb_fdc1);

}
//____________________________________________________________________
void FDC1DataProcessor::ClearData()
{
  fCalibFDC1Hit->ClearData();
  fCalibFDC1Track->ClearData();
  fFDC1_X=-9999.;
  fFDC1_Y=-9999.;
  fFDC1_A=-9999.;
  fFDC1_B=-9999.;
}
//____________________________________________________________________
void FDC1DataProcessor::ReconstructData()
{
  fCalibFDC1Hit->ReconstructData();

  if (!fDoTracking) return;
  fCalibFDC1Track->ReconstructData();

  // FDC1 Track
  TClonesArray *FDC1Tracks = fCalibFDC1Track->GetDCTrackArray();

  if (FDC1Tracks) {
    Int_t FDC1NumberOfTracks = FDC1Tracks->GetEntries();
    if(FDC1NumberOfTracks > 0) {
      TArtDCTrack *TrackFDC1 = (TArtDCTrack *)FDC1Tracks->At(0);
      fFDC1_X = TrackFDC1->GetPosition(0);
      fFDC1_A = TMath::ATan(TrackFDC1->GetAngle(0));
      fFDC1_Y = TrackFDC1->GetPosition(1);
      fFDC1_B = TMath::ATan(TrackFDC1->GetAngle(1));
    }
  }

  
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

  if (!fDoTracking) return;
  fhxy_fdc1->Fill(fFDC1_X,fFDC1_Y);  
  fhxa_fdc1->Fill(fFDC1_X,fFDC1_A);  
  fhyb_fdc1->Fill(fFDC1_Y,fFDC1_B);  

}
//____________________________________________________________________

void FDC1DataProcessor::LoadDCTDCDistribution() {
  TDirectory *pwd = gDirectory;

  TFile *RootFile = new TFile(fTDCDistFileName,"READ");

  if(RootFile->IsOpen()) {
    gROOT->cd();
    TH1D *Hist1D = NULL;
    Int_t FDC1NumberOfLayers = 14;
   

    for(Int_t i=0; i<FDC1NumberOfLayers; i++) {
      Hist1D = (TH1D*) RootFile->Get(Form("hfdc1tdc%d",i));

      if(Hist1D) {
	fCalibFDC1Track->SetTDCDistribution(Hist1D,i);
	delete Hist1D;
	Hist1D = NULL;
      }
      else
      std::cout << "\e[35m " << "Warning LoadTDCDistribution :: Could not find the following histogram " << Form("hfdc1tdc%d",i) << "\e[0m" << std::endl;
    }
  }else{
    fDoTracking = false;
    std::cout << "\e[35m " << "Skip FDC1 tracking " << "\e[0m" << std::endl;
  }

  pwd->cd();
}

