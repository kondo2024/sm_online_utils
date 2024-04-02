#ifndef _FDC1DATAPROCESSOR_H_
#define _FDC1DATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC1Track.hh"

#include "TH1.h"
#include "TH2.h"

class FDC1DataProcessor : public SAMURAIDataProcessor
{
public: 

  FDC1DataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC1";
    fdbFileName = "db/SAMURAIFDC1.xml";
  }
  ~FDC1DataProcessor(){;}

  FDC1DataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC1";
    fdbFileName = "db/SAMURAIFDC1.xml";
    fTDCDistFileName = inputtdcdistfilename;
  }

  FDC1DataProcessor(const char* inputdbfilename, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC1";
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
  TArtCalibFDC1Hit*   fCalibFDC1Hit;
  TArtCalibFDC1Track* fCalibFDC1Track;

  const char* fTDCDistFileName;
  const char* fdbFileName;

  // histograms
  TH1* fhidt_fdc1;
  TH1* fhxy_fdc1;

  //Variables
  Double_t fFDC1_X, fFDC1_Y, fFDC1_ThetaX, fFDC1_ThetaY;

};

#endif
