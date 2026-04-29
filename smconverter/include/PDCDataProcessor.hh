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
    : SAMURAIDataProcessor(), fDoTracking(true)
  {
    fBranchName = "PDC";
    fdbFileName = "db/SAMURAIPDC.xml";
  }
  ~PDCDataProcessor(){;}

  PDCDataProcessor(const char* inputdbfilename)
    : SAMURAIDataProcessor(), fDoTracking(true)
  {
    fBranchName = "PDC";
    fdbFileName = inputdbfilename;
  }

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();

  void SetdbFileName(const char* setdbfilename){fdbFileName = setdbfilename;}
  void DoTracking(bool tf=true){fDoTracking = tf;}
  
protected:
  TArtCalibPDCHit*   fCalibPDCHit;
  TArtCalibPDCTrack* fCalibPDCTrack;

  const char* fdbFileName;
  Bool_t fDoTracking;
  
  // histograms
  TH1* fhidt_pdc;
  TH1* fhidtot_pdc;
  TH1* fhxy_pdc;

  //Variables
  Double_t fPDC_X, fPDC_Y, fPDC_ThetaX, fPDC_ThetaY;

};

#endif
