#ifndef __DATAMERGEFILTER_PFAD__
#define __DATAMERGEFILTER_PFAD__

#include "TROOT.h"
#include "TNamed.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1.h"
#include "DataMergeFilter.hh"

class DataMergeFilter_PFAD : public DataMergeFilter{

public:

  DataMergeFilter_PFAD()
    : DataMergeFilter()
  {;}
  ~DataMergeFilter_PFAD(){;}

  void Run(const char* fname_sm, const char* fname_pfad,
	   const char* fname_out);

  void Filter();        // Fill if TS is within window
  
};

#endif
