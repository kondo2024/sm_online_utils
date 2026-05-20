#ifndef _HIMEVETODATAPROCESSOR_H_
#define _HIMEVETODATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibHIMEVETO.hh"

#include "TH1.h"
#include "TH2.h"

class HIMEVETODataProcessor : public SAMURAIDataProcessor
{
public: 

  HIMEVETODataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "HIMEVETO";
    fdbFileName = "db/HIMEVETO.xml";
  }
  ~HIMEVETODataProcessor(){;}

  HIMEVETODataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "HIMEVETO";
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
  bool fVeto_fired;

  TArtCalibHIMEVETO* fCalibHIMEVETO;

  const char* fdbFileName;

  // histograms
  TH1* fhidtl0;
  TH1* fhidtl1;
  TH1* fhidtt0;
  TH1* fhidtt1;
  TH1* fhidtot0;
  TH1* fhidtot1;
};

#endif
