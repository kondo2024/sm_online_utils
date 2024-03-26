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

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  //void SetTDCDistFile(const char* rootfilename){fTDCDistFileName = rootfilename;}
  
  void LoadDCTDCDistribution(char *FilenName, TArtCalibBDC1Track *CalibBDC1Track, TArtCalibBDC2Track *CalibBDC2Track);

protected:
  TArtCalibBDC1Hit*   fCalibBDC1Hit;
  TArtCalibBDC2Hit*   fCalibBDC2Hit;
  TArtCalibBDC1Track* fCalibBDC1Track;
  TArtCalibBDC2Track* fCalibBDC2Track;

  // histograms
  TH1* fhidt_bdc1;
  TH1* fhidt_bdc2;
  TH1* fhxy_bdc1;
  TH1* fhxy_bdc2;

  //Variables
  Double_t BDC1_X, BDC1_Y, BDC1_ThetaX, BDC1_ThetaY;
  Double_t BDC2_X, BDC2_Y, BDC2_ThetaX, BDC2_ThetaY;

};

#endif
