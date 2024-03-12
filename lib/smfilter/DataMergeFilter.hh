#ifndef __DATAMERGEFILTER__
#define __DATAMERGEFILTER__

#include "TROOT.h"
#include "TNamed.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1.h"

class DataMergeFilter {

public:

  DataMergeFilter();
  ~DataMergeFilter();

  //bool OpenInputFiles(const char* fname_sm, const char* fname_det);

  void FindRunStart();                  // Optional: Use these
  void FindRunEnd(Long64_t nmax=-1);    // if run start is much different
  void TSCombinations(TString mode="start");// mode = "start" or "end"

  void CorrectTSPeriod();// Optional

  void FindTSOffset(TString mode="start");// mode = "start" or "end"

  void Filter();        // Fill if TS is within window
  void MakePlots();     // For checking
  // method for adding log
  void AddLog(const char* log);
  

  void SetInputSMFileName(const char* fname){fFileName_sm = fname;}
  void SetInputDetFileName(const char* fname){fFileName_det = fname;}
  bool SetOutputFileName(const char* fname);

  void SetInputSMTreeName(const char* treename){fTreeName_sm = treename;}
  void SetInputDetTreeName(const char* treename){fTreeName_det = treename;}
  void SetOutputTreeName(const char* treename){fTreeName_out = treename;}
  void SetBranchName_smTS(const char* name){fBrName_smTS = name;}
  void SetBranchName_detTS(const char* name){fBrName_detTS = name;}
  
  void SetNsrch_sm(Int_t val){fNsrch_sm = val;}
  void SetNsrch_det(Int_t val){fNsrch_det = val;}

  void SetTS2us_sm(Double_t val){fTS2us_sm = val;}
  void SetTS2us_det(Double_t val){fTS2us_det = val;}
  void SetNbin(Int_t n){fNbin=n;}

  void SetTSMatchWindow(Double_t window){fMatchWindow_us = window;}

  TString GetInputSMTreeName(){return fTreeName_sm;}
  TString GetInputDetTreeName(){return fTreeName_det;}
  TString GetBranchName_smTS(){return fBrName_smTS;}
  TString GetBranchName_detTS(){return fBrName_detTS;}
  
  Int_t GetNsrch_sm(){return fNsrch_sm;}
  Int_t GetNsrch_det(){return fNsrch_det;}

  Double_t GetTS2us_sm(){return fTS2us_sm;}
  Double_t GetTS2us_det(){return fTS2us_det;}
  Double_t GetNbin(){return fNbin;}

  Double_t GetTSMatchWindow(){return fMatchWindow_us;}

protected:

  TH1* ChangePlotRange(TTree* t_comb, TString hname, Double_t fac,
		       Double_t &xpeak, Double_t &dx, Bool_t Is1Bin);
  TH1* DrawdTS(TTree* t_comb, TString hname, Double_t fac, Double_t xpeak, Double_t dx);
  Bool_t ZoomHist(TTree* t, TH1 *hist, Double_t fac);
  Bool_t IsPeakABin(TH1* hist);

  bool OpenInputSMFile();
  bool OpenInputDetFile();
  bool OpenOutputFile();
  
  // members
  TString fFileName_sm;
  TString fFileName_det;
  TString fFileName_out;

  TFile *fFile_sm;
  TFile *fFile_det;
  TFile *fFile_out;

  Bool_t fIsFirstOpen_sm;
  Bool_t fIsFirstOpen_det;
  Bool_t fIsFirstOpen_out;
  
  TString fTreeName_sm;
  TString fTreeName_det;
  TString fTreeName_out;

  TTree *fTree_sm;
  TTree *fTree_det;
  TTree *fTree_out;// filtered

  TTree *fTree_comb0;
  TTree *fTree_comb1;

  TString fBrName_smTS;// branch name
  TString fBrName_detTS;// branch name
  
  Long64_t fNsta_det;
  Long64_t fNend_det;

  Int_t fNsrch_sm; // number of events
  Int_t fNsrch_det;// for taking combinations
  Int_t fNbin;     // Nbin for offset search
  
  Double_t fTS2us_sm;
  Double_t fTS2us_det;

  Double_t fTSoff_sta_us;// us
  Double_t fTSoff_end_us;// us
  Double_t fMatchWindow_us;

  TDirectory *fLogDir;
  Int_t ilog;
  
};

#endif
