#ifndef _BDCDATAPROCESSOR_H_
#define _BDCDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibBDC1Track.hh"
#include "TArtCalibBDC2Track.hh"

#include "TH1.h"
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
  }
  ~BDCDataProcessor(){;}

  BDCDataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbfilename1 = "db/SAMURAIBDC1.xml";
    fdbfilename2 = "db/SAMURAIBDC2.xml";
    fTDCDistFileName = inputtdcdistfilename;
  }

  BDCDataProcessor(const char* inputdbfilename1,const char* inputdbfilename2, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbfilename1 = inputdbfilename1;
    fdbfilename2 = inputdbfilename2;
    fTDCDistFileName = inputtdcdistfilename;
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

protected:
  TArtCalibBDC1Hit*   fCalibBDC1Hit;
  TArtCalibBDC2Hit*   fCalibBDC2Hit;
  TArtCalibBDC1Track* fCalibBDC1Track;
  TArtCalibBDC2Track* fCalibBDC2Track;

  const char* fTDCDistFileName;
  const char* fdbfilename1;
  const char* fdbfilename2;
  

  // histograms
  TH1* fhidt_bdc1;
  TH1* fhidt_bdc2;
  TH1* fhxy_bdc1;
  TH1* fhxy_bdc2;

  //Variables
  Double_t fBDC1_X, fBDC1_Y, fBDC1_ThetaX, fBDC1_ThetaY;
  Double_t fBDC2_X, fBDC2_Y, fBDC2_ThetaX, fBDC2_ThetaY;

};

#endif
