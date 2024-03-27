#ifndef _HODPLADATAPROCESSOR_H_
#define _HODPLADATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibHODPla.hh"

#include "TH1.h"
#include "TH2.h"

class HODPlaDataProcessor : public SAMURAIDataProcessor
{
public: 

  HODPlaDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "HODPla";
    fdbpath = "db/";
  }
  ~HODPlaDataProcessor(){;}

  HODPlaDataProcessor(const char* inputdbpath)
    : SAMURAIDataProcessor()
  {
    fBranchName = "HODPla";
    fdbpath = inputdbpath;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void Setdbpath(const char* setdbpath){fdbpath = setdbpath;}

protected:
  TArtCalibHODPla* fCalibHODPla;

  const char* fdbpath;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
