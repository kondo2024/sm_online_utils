#ifndef __MONITORONLINE__
#define __MONITORONLINE__

#include "TArtEventStore.hh"
#include "SAMURAIDataProcessor.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TVector3.h"

#include <vector>

class OnlineMonitor : public TObject {
public:
  OnlineMonitor(TString RidfFile="online");
  ~OnlineMonitor();

  void Run();
  bool Draw();
  void ResetHist();

  void SetResetCount(Double_t val){fResetCount=val;}
  void SetDrawTimeInterval(Double_t val){fDrawTimeInterval=val;}
  void SetNx(Int_t val){fnx=val;}
  void SetNy(Int_t val){fny=val;}
  void DoesReset(Bool_t tf){fDoesReset = tf;}

  void Print(const char* fname="onlinemonitor.pdf");

protected:
  TClonesArray* fCalibBDC1Hit;
  TClonesArray* fCalibBDC2Hit;
  TClonesArray* fCalibBDC1Track;
  TClonesArray* fCalibBDC2Track;

  const char* fTDCDistFileName;
  const char* fdbfilename1;
  const char* fdbfilename2;
  
  // histograms
  TH1* fhxy_target;
  TH1* fhxthx_target;
  TH1* fhythy_target;
  TH1* fhx_target;  // add by WX
  TH1* fhy_target;  // add by WX


  //Variables
  Double_t fBDC1_X, fBDC1_Y, fBDC1_ThetaX, fBDC1_ThetaY;
  Double_t fBDC2_X, fBDC2_Y, fBDC2_ThetaX, fBDC2_ThetaY;
  //constant
  Double_t BDC1_Z  =-1996.44;
  Double_t BDC2_Z  =-996.68;
  Double_t Target_Z=0;



private:
  void Init();
  void BookHist();
  void BookUserHist();
  void AnalyzeOneEvent();
  void FillUserHist();

  void EventLoop();
  int  GetKeyCommand();

  TCanvas* fCanvas;

  TFile *fRootFile;
  TArtEventStore* festore;

  Bool_t fInitialize;
  Bool_t fIsHistBooked;
  std::vector<TH1*> fHistArray;
  TString fRunName;

  Int_t fnx;
  Int_t fny;
  Int_t fResetCount;
  Int_t fDrawTimeInterval;
  Bool_t fDoesReset;

  ULong64_t fNeve;
  ULong64_t fNeve_monitor;
  Int_t f_iplot;

  std::vector<SAMURAIDataProcessor*> fProcessorArray;

  // user histograms
  // TH1* fhcoin;

  ClassDef(OnlineMonitor,1);
};

#endif
