#ifndef __MONITORONLINE__
#define __MONITORONLINE__

#define kArraySize 10

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TTree.h"

#include "TArtPlastic.hh"
#include "TArtEventStore.hh"
#include "SAMURAIDataProcessor.hh"

#include "TMath.h"

#include <vector>
#include <limits>

class OnlineConverter : public TObject {
public:
  OnlineConverter(TString RidfFile="online");
  ~OnlineConverter();

  void Run(ULong64_t);
  void Run() { Run(1e12);} ;
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
  TString fOutFileName;
  Int_t fRunNumber;

  Int_t fnx;
  Int_t fny;
  Int_t fResetCount;
  Int_t fDrawTimeInterval;
  Bool_t fDoesReset;

  ULong64_t fNeve;
  ULong64_t fNeveMax;
  ULong64_t fNeve_monitor;
  Int_t f_iplot;

  const Double_t kNaN = TMath::QuietNaN();

  std::vector<SAMURAIDataProcessor*> fProcessorArray;


  TTree *tree;

  Double_t fTRef;

  Double_t fF3TLRaw;
  Double_t fF3TRRaw;
  Double_t fF3QLRaw;
  Double_t fF3QRRaw;
  Double_t fF3QTCLRawStart;
  Double_t fF3QTCRRawStart;
  Double_t fF3QTCLRawWidth;
  Double_t fF3QTCRRawWidth;
  Int_t fF3NHitL;
  Int_t fF3NHitR;
  Double_t fF3MulHitTDCL[kArraySize];
  Double_t fF3MulHitTDCR[kArraySize];

  Double_t fF5TLRaw;
  Double_t fF5TRRaw;
  Double_t fF5QLRaw;
  Double_t fF5QRRaw;
  Double_t fF5QTCLRawStart;
  Double_t fF5QTCRRawStart;
  Double_t fF5QTCLRawWidth;
  Double_t fF5QTCRRawWidth;
  Int_t fF5NHitL;
  Int_t fF5NHitR;
  Double_t fF5MulHitTDCL[kArraySize];
  Double_t fF5MulHitTDCR[kArraySize];

  Double_t fF7TLRaw;
  Double_t fF7TRRaw;
  Double_t fF7QLRaw;
  Double_t fF7QRRaw;
  Double_t fF7QTCLRawStart;
  Double_t fF7QTCRRawStart;
  Double_t fF7QTCLRawWidth;
  Double_t fF7QTCRRawWidth;
  Int_t fF7NHitL;
  Int_t fF7NHitR;
  Double_t fF7MulHitTDCL[kArraySize];
  Double_t fF7MulHitTDCR[kArraySize];

  Double_t fF13AQLRaw;
  Double_t fF13AQRRaw;
  Double_t fF13BQLRaw;
  Double_t fF13BQRRaw;

  Double_t fF3T_BR;
  Double_t fF3T_SM;
  Double_t fF5DT;
  Double_t fF5Xpla;
  Double_t fF7T_BR;
  Double_t fF7T_SM;
  Double_t fF7Q;
  Double_t fToF37_BR;
  Double_t fToF37_SM;
  Double_t fbeta37;
  Double_t fgamma37;
  Double_t fBetheF;
  Double_t fZ_tmp;
  Double_t fZ;
  Double_t fdelta;
  Double_t fBrhoF5;
  Double_t fAoQ;
  Double_t fdE;

  Double_t fBDC1_X;
  Double_t fBDC1_Y;
  Double_t fBDC1_ThetaX;
  Double_t fBDC1_ThetaY;
  Double_t fBDC2_X;
  Double_t fBDC2_Y;
  Double_t fBDC2_ThetaX;
  Double_t fBDC2_ThetaY;
  Double_t fFDC1_X;
  Double_t fFDC1_Y;
  Double_t fFDC1_ThetaX;
  Double_t fFDC1_ThetaY;

  Double_t fTGT_X;
  Double_t fTGT_Y;
  Double_t fTGT_A;
  Double_t fTGT_B;

  Double_t fF3_X;
  Double_t fF3_Y;
  Double_t fF3_A;
  Double_t fF3_B;
  Double_t fF5_X;
  Double_t fF5_Y;
  Double_t fF5_A;
  Double_t fF5_B;
  Double_t fF7_X;
  Double_t fF7_Y;
  Double_t fF7_A;
  Double_t fF7_B;

  // user histograms
  // TH1* fhcoin;
  TH2* fhpla_pid;
  TH2* fhpla_pid2;
  TH2* fhpla_pid3;
  TH2* fhpla_tof713dt5;
  TH2* fheff_bdc1;
  TH2* fheff_bdc2;
  TH2* fheff_fdc1;
  TH1* fheff;
  TH2D *fhpla_tof313dt5;
  TH2D *fhpla_tof313dt5_cal;
  TH2* hf5dt_f5x;
  TH2* hf5dt2_f5x;
  TH2* hf5dt_f51ax;
  TH2* hf5dt_f51bx;
  TH2* hf5dt_f52ax;
  TH2* hf5dt_f52bx;

  TH2 *hf51bxf5dt;
  TH2 *hf51bxf5dtx;

  TH2 *htof37f5dtx;
  TH2 *htof37f5dtx2;
  TH2 *hzvstof37;
  TH2 *hzvstof372;
  TH2 *hbeta37tof37;
  TH2 *hbeta37tof372;
  TH2 *hBetheFbeta37;
  TH2 *hBetheFbeta372;
  TH2 *hBrhoF5beta37;
  TH2 *hAoQBrhoF5;
  TH2 *hdEBetheF;
  

  TH1 *hf3t_1, *hf3t_2;
  TH1 *hf7t_1, *hf7t_2;
  TH1 *htof37_1, *htof37_2, *htof37_3, *htof37_4;
  TH2 *htof37br_tof37sm;
  TH2 *htof37diff_tof37sm;

  TH1 *hf3lr_1, *hf3rr_1;
  TH1 *hf5lr_1, *hf5rr_1;
  TH1 *hf7lr_1, *hf7rr_1;
  TH1 *hf13alr_1, *hf13arr_1;
  TH1 *hf13blr_1, *hf13brr_1;
  TH1 *hsbvlr_1, *hsbvrr_1;

  TH1 *hf3lr_2, *hf3rr_2;
  TH1 *hf5lr_2, *hf5rr_2;
  TH1 *hf7lr_2, *hf7rr_2;

  TH2 *hf7qrl;
  TH2 *hf7qtd;
  TH2 *hf7qrtd;

  TH2 *hzvsaoq;
  ClassDef(OnlineConverter,1);
};

#endif
