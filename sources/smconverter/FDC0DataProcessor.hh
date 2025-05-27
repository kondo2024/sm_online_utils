#ifndef _FDC0DATAPROCESSOR_H_
#define _FDC0DATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibFDC0Hit.hh"
#include "TArtCalibFDC0Track.hh"

#include "TH1.h"
#include "TH2.h"

class FDC0DataProcessor : public SAMURAIDataProcessor
{
public: 

  FDC0DataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC0";
    fdbFileName = "db/SAMURAIFDC0.xml";
  }
  ~FDC0DataProcessor(){;}

  FDC0DataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC0";
    fdbFileName = "db/SAMURAIFDC0.xml";
    fTDCDistFileName = inputtdcdistfilename;
  }
 
  FDC0DataProcessor(const char* inputdbfilename, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FDC0";
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
  TArtCalibFDC0Hit*   fCalibFDC0Hit;
  TArtCalibFDC0Track* fCalibFDC0Track;

  const char* fTDCDistFileName;
  const char* fdbFileName;

  // histograms
  TH1* fhidt_fdc0;
  TH1* fhxy_fdc0;

  //Variables
  Double_t fFDC0_X, fFDC0_Y, fFDC0_ThetaX, fFDC0_ThetaY;

};

#endif
