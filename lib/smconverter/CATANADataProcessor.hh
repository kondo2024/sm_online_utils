#ifndef _CATANADATAPROCESSOR_H_
#define _CATANADATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibCATANA.hh"

#include "TH1.h"
#include "TH2.h"

class CATANADataProcessor : public SAMURAIDataProcessor
{
public: 

  CATANADataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "CATANA";
    fdbFileName = "db/CATANA.xml";
  }
  ~CATANADataProcessor(){;}

  CATANADataProcessor(char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "CATANA";
    fdbFileName = inputdbfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetdbFileName(char* setdbfilename){fdbFileName = setdbfilename;}

protected:
  TArtCalibCATANA* fCalibCATANA;

  char* fdbFileName;

  // histograms
  TH1* fhidAraw;
  TH1* fhidTraw;
  TH1* fhidAcal;
  TH1* fhidTcal;
  TH1* fhidAdop;
  TH1* fhidAdopBe;
  TH1* fhidAdopLi;
  TH1* fhidAp;
};

#endif
