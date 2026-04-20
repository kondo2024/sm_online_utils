#include "SAMURAITSDataProcessor.hh"

#include "TArtRawEventObject.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"

#include <iostream>

//____________________________________________________________________
void SAMURAITSDataProcessor::PrepareTreeBranches(TTree *tree)
{
  fTree = tree;
  fTree->Branch(fBranchName.Data(), &fTS);
}
//____________________________________________________________________
void SAMURAITSDataProcessor::PrepareHistograms()
{
  // under construction
}
//____________________________________________________________________
void SAMURAITSDataProcessor::ClearData()
{
  fTS = 0;
}
//____________________________________________________________________
void SAMURAITSDataProcessor::ReconstructData()
{
  ULong64_t ts0=0;
  ULong64_t ts1=0;

  TArtStoreManager *sman = TArtStoreManager::Instance();
  TArtRawEventObject* rawevent = (TArtRawEventObject*)sman->FindDataContainer("RawEvent");
  for(int i=0;i<rawevent->GetNumSeg();i++){
    TArtRawSegmentObject *seg = rawevent->GetSegment(i);
    //int device = seg->GetDevice();
    int fp = seg->GetFP();
    int detector = seg->GetDetector();
    int module = seg->GetModule();
    if(fp==13 && detector==60 && module==42){// should be checked
      ts0 = seg->GetData(0)->GetVal() ;
      ts1 = seg->GetData(1)->GetVal() ;
      fTS =  (ts0<<32) + ts1 ;
      //        printf("%lld ns \n", timestamp );
      //       if(timestamp>0) printf("\n TS == %lld s\n", (timestamp)/1e8 );
      break;
    }
  }

  if (fTS==0)
    TArtCore::Warning(__FILE__,"no timestamp data");
}
//____________________________________________________________________
void SAMURAITSDataProcessor::FillHistograms()
{
  // under construction
}
//____________________________________________________________________
