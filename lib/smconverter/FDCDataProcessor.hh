#ifndef _FDCDATAPROCESSOR_H_
#define _FDCDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibFDC0Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibFDC0Track.hh"
#include "TArtCalibFDC2Track.hh"

#include "TH1.h"
#include "TH2.h"

class FDCDataProcessor : public SAMURAIDataProcessor
{
public: 

  FDCDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "BDC";
  }
  ~FDCDataProcessor(){;}

  FDCDataProcessor(const char* inputdbpath, const char* inputtdcdistfilename)
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
  TArtCalibFDC0Hit*   fCalibFDC0Hit;
  TArtCalibFDC2Hit*   fCalibFDC2Hit;
  TArtCalibFDC0Track* fCalibFDC0Track;
  TArtCalibFDC2Track* fCalibFDC2Track;

  const char* fTDCDistFileName;
  const char* fdbpath;

  // histograms
  TH1* fhidt_fdc0;
  TH1* fhidt_fdc2;
  TH1* fhxy_fdc0;
  TH1* fhxy_fdc2;

  //Variables
  Double_t fFDC0_X, fFDC0_Y, fFDC0_ThetaX, fFDC0_ThetaY;
  Double_t fFDC2_X, fFDC2_Y, fFDC2_ThetaX, fFDC2_ThetaY;

};

#endif
