#ifndef __MONITORONLINE__
#define __MONITORONLINE__

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include "TArtEventStore.hh"
#include "SAMURAIDataProcessor.hh"

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
