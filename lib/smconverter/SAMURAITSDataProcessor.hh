#ifndef _SAMURAITSDATAPROCESSOR_H_
#define _SAMURAITSDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TROOT.h"

class SAMURAITSDataProcessor : public SAMURAIDataProcessor
{
public: 
  SAMURAITSDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "SMTS";
  }
  ~SAMURAITSDataProcessor(){;}

  virtual void PrepareCalib(){;}
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

protected:

  ULong64_t fTS;

};

#endif
