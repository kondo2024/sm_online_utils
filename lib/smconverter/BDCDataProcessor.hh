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
};

#endif
