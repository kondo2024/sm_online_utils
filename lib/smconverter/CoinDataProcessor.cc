#include "CoinDataProcessor.hh"
#include <iostream>

#include "TClonesArray.h"

//____________________________________________________________________
void CoinDataProcessor::PrepareCalib()
{
  fCalibCoin = new TArtCalibCoin;
  fCalibCoin->SetFpl(13);// for SAMURAI
  fCalibReady = true;
}
//____________________________________________________________________
void CoinDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  fTree = tree;
  fTree->Branch(fBranchName.Data(), fBit ,Form("%s[%d]/O",fBranchName.Data(),kNch+1));
}
//____________________________________________________________________
void CoinDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();

  fhcoin = new TH1D("hcoin","Coin ID",kNch,0.5,kNch+0.5); // start from ch=1
  fHistArray.push_back(fhcoin);
}
//____________________________________________________________________
void CoinDataProcessor::ClearData()
{
  for (int i=0;i<kNch+1;++i) fBit[i] = false;
  fCalibCoin->ClearData();
}
//____________________________________________________________________
void CoinDataProcessor::ReconstructData()
{
  fCalibCoin->ReconstructData();
  for (int i=1;i<kNch+1;++i){ // start from ch=1
    Bool_t tf = fCalibCoin->IsChTrue(i);
    fBit[i] = tf;
  }
}
//____________________________________________________________________
void CoinDataProcessor::FillHistograms()
{
  for (int i=1;i<kNch+1;++i) // start from ch=1
    if (fBit[i]) fhcoin->Fill(i);
}
//____________________________________________________________________
