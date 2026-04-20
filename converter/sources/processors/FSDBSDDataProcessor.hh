#ifndef _FSDBSDDATAPROCESSOR_H_
#define _FSDBSDDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibFSDBSDPla.hh"

#include "TH1.h"
#include "TH2.h"

class FSDBSDDataProcessor : public SAMURAIDataProcessor
{
public: 

  FSDBSDDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "FSDBSD";
    fdbFileName = "db/FSDBSD.xml";
  }
  ~FSDBSDDataProcessor(){;}

  FSDBSDDataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "FSDBSD";
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
  TArtCalibFSDBSDPla* fCalibFSDBSD;

  const char* fdbFileName;

  // histograms
  TH1* fhidt1;
  TH1* fhidt2;
  TH1* fhidt3;
  TH1* fhidtot1;
  TH1* fhidtot2;
  TH1* fhidtot3;
};

#endif
