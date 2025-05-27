#ifndef _SAMURAIDATAPROCESSOR_H_
#define _SAMURAIDATAPROCESSOR_H_

#include "TTree.h"
#include "TH1.h"
#include <vector>

class SAMURAIDataProcessor
{
public: 
  SAMURAIDataProcessor()
    : fMakeTreeBranches(false),
      fMakeHistograms(false),
      fTree(0), fBranchName("branch"),
      fCalibReady(false)
  {;}
  ~SAMURAIDataProcessor(){;}

  virtual void PrepareCalib() = 0;
  virtual void PrepareTreeBranches(TTree* tree) = 0;
  virtual void PrepareHistograms() = 0;
  virtual void ReconstructData() = 0;
  virtual void ClearData() = 0;
  virtual void FillHistograms() = 0;

  void MakeTreeBranches(bool tf){fMakeTreeBranches = tf;}
  void MakeHistograms(bool tf){fMakeHistograms = tf;}
  bool DoesMakeTreeBranches(){return fMakeTreeBranches;}
  bool DoesMakeHistograms(){return fMakeHistograms;}

  void SetTree(TTree *tree){fTree = tree;}
  const char* GetBranchName(){return fBranchName.Data();}

  std::vector<TH1*>* GetHistArray(){return &fHistArray;}

protected:
  bool fMakeTreeBranches;
  bool fMakeHistograms;

  TTree* fTree;
  TString fBranchName;

  bool fCalibReady;

  std::vector<TH1*> fHistArray;

};

#endif
