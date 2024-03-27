#ifndef _PLASTICDATAPROCESSOR_H_
#define _PLASTICDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibPlastic.hh"

#include "TH1.h"
#include "TH2.h"

class PlasticDataProcessor : public SAMURAIDataProcessor
{
public: 

  PlasticDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "Plastic";
    fdbpath = "db/";
  }
  ~PlasticDataProcessor(){;}

  PlasticDataProcessor(const char* inputdbpath)
    : SAMURAIDataProcessor()
  {
    fBranchName = "Plastic";
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
  TArtCalibPlastic* fCalibPlastic;

  const char* fdbpath;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
