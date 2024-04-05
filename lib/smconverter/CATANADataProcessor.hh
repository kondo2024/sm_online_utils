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
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
