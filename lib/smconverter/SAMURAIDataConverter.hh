#ifndef _SAMURAIDATACONVERTER_H_
#define _SAMURAIDATACONVERTER_H_

#include "SAMURAIDataProcessor.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TArtEventStore.hh"

#include <vector>

class SAMURAIDataConverter
{
public: 
  SAMURAIDataConverter();
  ~SAMURAIDataConverter(){;}

  void Run();

  void RegisterDataProcessor(SAMURAIDataProcessor *processor);
  
  void SetRIDFFileName(const char* filename){fRIDFFileName = filename;}
  void SetOutputFileName(const char* filename){fOutputFileName = filename;}
  void SetOutputTreeName(const char* treename){fTreeName = treename;}
  void PrepareTreeBranches();
  void PrepareHistograms();
  void EventLoop();

protected:
  TString fRIDFFileName;

  TString fOutputFileName;
  TFile *fOutputFile;
  TString fTreeName;
  TTree *fTree;

  std::vector<SAMURAIDataProcessor*> fProcessorList;

  bool fDoesFillTree;

  TArtEventStore* fEventStore;

};

#endif
