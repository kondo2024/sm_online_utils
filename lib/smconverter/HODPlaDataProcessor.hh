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
  }
  ~HODPlaDataProcessor(){;}

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

protected:
  TArtCalibHODPla* fCalibHODPla;

  // histograms
  TH1* fhidtl;
  TH1* fhidtr;
  TH1* fhidql;
  TH1* fhidqr;
};

#endif
