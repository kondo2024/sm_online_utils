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
    fdbFileName = "db/SAMURAIHOD.xml";
  }
  ~HODPlaDataProcessor(){;}

  HODPlaDataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "HODPla";
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
  TArtCalibHODPla* fCalibHODPla;

  const char* fdbFileName;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
