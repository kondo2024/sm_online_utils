#ifndef _CoinDATAPROCESSOR_H_
#define _CoinDATAPROCESSOR_H_

#include "SAMURAIDataProcessor.hh"
#include "TArtCalibCoin.hh"

#include "TH1.h"
#include "TH2.h"

class CoinDataProcessor : public SAMURAIDataProcessor
{
public: 

  CoinDataProcessor()
    : SAMURAIDataProcessor()
  {
    fBranchName = "CoinBit";
  }
  ~CoinDataProcessor(){;}

  virtual void PrepareCalib();
  virtual void PrepareTreeBranches(TTree* tree);
  virtual void PrepareHistograms();
  virtual void ReconstructData();
  virtual void ClearData();
  virtual void FillHistograms();
  virtual bool GetCoinBit(int i);

protected:
  TArtCalibCoin* fCalibCoin;

  // Tree variables
  static const Int_t kNch = 8;// number of channels
  Bool_t fBit[kNch+1];// ch 1-8

  // histograms
  TH1* fhcoin;
};

#endif
