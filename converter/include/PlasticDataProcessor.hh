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
    fdbFileName = "db/SAMURAIPlastic.xml";
  }
  ~PlasticDataProcessor(){;}

  PlasticDataProcessor(char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "Plastic";
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
  TArtCalibPlastic* fCalibPlastic;

  char* fdbFileName;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
