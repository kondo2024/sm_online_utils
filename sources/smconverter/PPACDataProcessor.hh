#ifndef _PPACDATAPROCESSOR_H_
#define _PPACDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibPPAC.hh"
//#include "TArtCalibFocalPlane.hh"

#include "TH1.h"
#include "TH2.h"

class PPACDataProcessor : public SAMURAIDataProcessor
{
public: 

  PPACDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "PPAC";
    fdbFileName = "db/BigRIPSPPAC.xml";
  }
  ~PPACDataProcessor(){;}

  PPACDataProcessor(char* inputdbfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "PPAC";
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
  TArtCalibPPAC* fCalibPPAC;
  // TArtCalibFocalPlane* fCalibFocalPlane;

  char* fdbFileName;

  // histograms
  // TH1* fhidtl;
  // TH1* fhidtr;
  // TH1* fhidql;
  // TH1* fhidqr;
  TH1* hidtx1;
  TH1* hidtx2;
  TH1* hidty1;
  TH1* hidty2;
  TH1* hidta;
  TH1* hidx;
  TH1* hidy;
  // TH1* h2idx;
  // TH1* h2ida;
  // TH1* h2idy;
  // TH1* h2idb;
};

#endif
