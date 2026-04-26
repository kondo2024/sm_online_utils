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
  TH1* fhidlelu;
  TH1* fhidlerd;
  TH1* fhidtotlu;
  TH1* fhidtotrd;
  TH1* fhidtime;
  TH1* fhidTOT;
  TH1* fhidxidy;
};

#endif
