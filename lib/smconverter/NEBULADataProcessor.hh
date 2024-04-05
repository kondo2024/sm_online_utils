#ifndef _NEBULADATAPROCESSOR_H_
#define _NEBULADATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibNEBULA.hh"
#include "TArtCalibNEBULAHPC.hh"

#include "TH1.h"
#include "TH2.h"

class NEBULADataProcessor : public SAMURAIDataProcessor
{
public: 

  NEBULADataProcessor()
    : SAMURAIDataProcessor(),
      fIncludeHPC(false)
  {
    fBranchName = "NEBULA";
    fdbFileName = "db/NEBULA.xml";
  }
  ~NEBULADataProcessor(){;}

  NEBULADataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor(),
      fIncludeHPC(false)
  {
    fBranchName = "NEBULA";
    fdbFileName = inputdbfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetHPC(bool tf){fIncludeHPC = tf;}
  void SetdbFileName(const char* setdbfilename){fdbFileName = setdbfilename;}

protected:
  bool fIncludeHPC;

  TArtCalibNEBULA* fCalibNEBULA;
  TArtCalibNEBULAHPC* fCalibNEBULAHPC;

  const char* fdbFileName;

  // histograms
  TH1* fhidtu;
  TH1* fhidtd;
  TH1* fhidqu;
  TH1* fhidqd;
};

#endif
