#ifndef _PDCDATAPROCESSOR_H_
#define _PDCDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibPDCHit.hh"
#include "TArtCalibPDCTrack.hh"

#include "TH1.h"
#include "TH2.h"

class PDCDataProcessor : public SAMURAIDataProcessor
{
public: 

  PDCDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "PDC";
    fdbFileName = "db/SAMURAIPDC.xml";
  }
  ~PDCDataProcessor(){;}

  PDCDataProcessor(const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "PDC";
    fdbFileName = "db/SAMURAIPDC.xml";
    fTDCDistFileName = inputtdcdistfilename;
  }
 
  PDCDataProcessor(const char* inputdbfilename, const char* inputtdcdistfilename)
    : SAMURAIDataProcessor()
  {
    fBranchName = "PDC";
    fdbFileName = inputdbfilename;
    fTDCDistFileName = inputtdcdistfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetdbFileName(const char* setdbfilename){fdbFileName = setdbfilename;}
  void SetTDCDistFile(const char* setfilename){fTDCDistFileName = setfilename;} 

protected:
  TArtCalibPDCHit*   fCalibPDCHit;
  TArtCalibPDCTrack* fCalibPDCTrack;

  const char* fTDCDistFileName;
  const char* fdbFileName;

  // histograms
  TH1* fhidt_pdc;
  TH1* fhxy_pdc;

  //Variables
  Double_t fPDC_X, fPDC_Y, fPDC_ThetaX, fPDC_ThetaY;

};

#endif
