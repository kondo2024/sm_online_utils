#ifndef _FDC2DATAPROCESSOR_H_
#define _FDC2DATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtCalibFDC2Track.hh"

#include "TH1.h"
#include "TH2.h"

class FDC2DataProcessor : public SAMURAIDataProcessor
{
public: 

  FDC2DataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC2";
    fdbFileName = "db/SAMURAIFDC2.xml";
  }
  ~FDC2DataProcessor(){;}

  FDC2DataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC2";
    fdbFileName = "db/SAMURAIFDC2.xml";
    fTDCDistFileName = inputtdcdistfilename;
  }

  FDC2DataProcessor(const char* inputdbfilename, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC2";
    fdbFileName = inputdbfilename;
    fTDCDistFileName = inputtdcdistfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetdbFileName(const char* setdbfilename){fdbFileName = setdbfilename;}
  void SetTDCDistFile(const char* setfilename){fTDCDistFileName = setfilename;} 
  void LoadDCTDCDistribution();

protected:
  TArtCalibFDC2Hit*   fCalibFDC2Hit;
  TArtCalibFDC2Track* fCalibFDC2Track;

  const char* fTDCDistFileName;
  const char* fdbFileName;

  // histograms
  TH1* fhidt_fdc2;
  TH1* fhxy_fdc2;

  //Variables
  Double_t fFDC2_X, fFDC2_Y, fFDC2_ThetaX, fFDC2_ThetaY;

};

#endif
