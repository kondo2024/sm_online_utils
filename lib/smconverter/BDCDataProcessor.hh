#ifndef _BDCDATAPROCESSOR_H_
#define _BDCDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"

#include "TH1.h"
#include "TVector3.h"
#include "TH2.h"

class BDCDataProcessor : public SAMURAIDataProcessor
{
public: 

  BDCDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbfilename1 = "db/SAMURAIBDC1.xml";
    fdbfilename2 = "db/SAMURAIBDC2.xml";

    // need to be modified
    fBDC1_Z  = -1987.30; //-1996.44; //s053
    fBDC2_Z  = -987.54; //-996.68;  //s053
    fTarget_Z = 0;
  }
  ~BDCDataProcessor(){;}

  BDCDataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbfilename1 = "db/SAMURAIBDC1.xml";
    fdbfilename2 = "db/SAMURAIBDC2.xml";
    fTDCDistFileName = inputtdcdistfilename;

    // need to be modified
    fBDC1_Z  = -1987.30; //-1996.44; //s053
    fBDC2_Z  = -987.54; //-996.68;  //s053
    fTarget_Z = 0;
  }

  BDCDataProcessor(const char* inputdbfilename1,const char* inputdbfilename2, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbfilename1 = inputdbfilename1;
    fdbfilename2 = inputdbfilename2;
    fTDCDistFileName = inputtdcdistfilename;

    // need to be modified
    fBDC1_Z  = -1987.30; //-1996.44; //s053
    fBDC2_Z  = -987.54; //-996.68;  //s053
    fTarget_Z = 0;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();


  void SetdbfilenameBDC1(const char* setdbfilename1){fdbfilename1 = setdbfilename1;}
  void SetdbfilenameBDC2(const char* setdbfilename2){fdbfilename2 = setdbfilename2;}
  void SetTDCDistFile(const char* setfilename){fTDCDistFileName = setfilename;}

  void LoadDCTDCDistribution();

  void SetBDC1Z(Double_t val){fBDC1_Z = val;}
  void SetBDC2Z(Double_t val){fBDC2_Z = val;}
  void SetTargetZ(Double_t val){fTarget_Z = val;}

  Double_t GetBDC1Z(){return fBDC1_Z;}
  Double_t GetBDC2Z(){return fBDC2_Z;}
  Double_t GetTargetZ(){return fTarget_Z;}

protected:
  TArtCalibBDC1Hit*   fCalibBDC1Hit;
  TArtCalibBDC2Hit*   fCalibBDC2Hit;
  TArtCalibBDC1Track* fCalibBDC1Track;
  TArtCalibBDC2Track* fCalibBDC2Track;
  TArtDCTrack*        fTargetTrack;

  const char* fTDCDistFileName;
  const char* fdbfilename1;
  const char* fdbfilename2;

  // histograms
  TH1* fhidt_bdc1;
  TH1* fhidt_bdc2;
  TH1* fhxy_bdc1;
  TH1* fhxy_bdc2;
  TH1* fhxy_tgt;
  TH1* fhxa_tgt;
  TH1* fhyb_tgt;
  TH1* fhx_tgt;
  TH1* fhy_tgt;  

  //Variables
  Double_t fBDC1_X, fBDC1_Y, fBDC1_ThetaX, fBDC1_ThetaY;
  Double_t fBDC2_X, fBDC2_Y, fBDC2_ThetaX, fBDC2_ThetaY;
  //constant
  Double_t fBDC1_Z;
  Double_t fBDC2_Z;
  Double_t fTarget_Z;

};

#endif
