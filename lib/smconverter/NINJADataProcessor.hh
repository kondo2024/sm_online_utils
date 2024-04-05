#ifndef _NINJADATAPROCESSOR_H_
#define _NINJADATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibNINJA.hh"

#include "TH1.h"
#include "TH2.h"

class NINJADataProcessor : public SAMURAIDataProcessor
{
public: 

  NINJADataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "NINJA";
    fdbFileName = "db/NINJA.xml";
  }
  ~NINJADataProcessor(){;}

  NINJADataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "NINJA";
    fdbFileName = inputdbfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetdbFileName(const char* setdbfilename){fdbFileName = setdbfilename;}

protected:
  TArtCalibNINJA* fCalibNINJA;

  const char* fdbFileName;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
