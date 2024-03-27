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
  }
  ~BDCDataProcessor(){;}

  BDCDataProcessor(const char* inputdbpath, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
    fdbpath = inputdbpath;
    fTDCDistFileName = inputtdcdistfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();


  void Setdbpath(const char* setdbpath){fdbpath = setdbpath;}
  void SetTDCDistFile(const char* setfilename){fTDCDistFileName = setfilename;}

  void LoadDCTDCDistribution();

protected:
  TArtCalibBDC1Hit*   fCalibBDC1Hit;
  TArtCalibBDC2Hit*   fCalibBDC2Hit;
  TArtCalibBDC1Track* fCalibBDC1Track;
  TArtCalibBDC2Track* fCalibBDC2Track;

  const char* fTDCDistFileName;
  const char* fdbpath;

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
