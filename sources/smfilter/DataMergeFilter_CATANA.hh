#ifndef __DATAMERGEFILTER_CATANA__
#define __DATAMERGEFILTER_CATANA__

#include "TROOT.h"
#include "TNamed.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1.h"
#include "DataMergeFilter.hh"

class DataMergeFilter_CATANA : public DataMergeFilter{

public:

  DataMergeFilter_CATANA()
    : DataMergeFilter()
  {;}
  ~DataMergeFilter_CATANA(){;}

  void Run(const char* fname_sm, const char* fname_catana,
	   const char* fname_out);

  void Filter();        // Fill if TS is within window
  
};

#endif
