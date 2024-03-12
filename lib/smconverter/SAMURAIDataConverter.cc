#include "SAMURAIDataConverter.hh"

#include <iostream>
//____________________________________________________________________
SAMURAIDataConverter::SAMURAIDataConverter()
  : fRIDFFileName(""), 
    fOutputFileName("converted.root"),fOutputFile(0), 
    fTreeName("tree"), fTree(0), 
    fDoesFillTree(false)
{
  fEventStore = new TArtEventStore;
}
//____________________________________________________________________
void SAMURAIDataConverter::Run()
{
  bool IsRIDFOK = fEventStore->Open(fRIDFFileName.Data());
  if (!IsRIDFOK){
    std::cout<<"cannot open file:"<<fRIDFFileName.Data()<<std::endl;
    return;
  }

  if (fOutputFileName.Contains(".ridf")){
    std::cout<<"do not use \".ridf\" in the output filename:"<<fOutputFileName.Data()<<std::endl;
    return;
  }

  fOutputFile = new TFile(fOutputFileName.Data(),"recreate");
  if (!fOutputFile->IsOpen()){
    std::cout<<"cannot open file:"<<fOutputFileName.Data()<<std::endl;
    return;
  }

  PrepareTreeBranches();
  PrepareHistograms();
  EventLoop();
  fOutputFile->Write();
  fOutputFile->Close();
}
//____________________________________________________________________
void SAMURAIDataConverter::RegisterDataProcessor(SAMURAIDataProcessor* processor)
{
  fProcessorList.push_back(processor);
  processor->SetTree(fTree);
}
//____________________________________________________________________
void SAMURAIDataConverter::PrepareTreeBranches()
{
  fOutputFile->cd();
  fTree = new TTree(fTreeName.Data(),"");

  int n = fProcessorList.size();

  if (n==0) std::cout<<"SAMURAIDataConverter: no processor is registered"<<std::endl;

  for (int i=0;i<n;i++){
    SAMURAIDataProcessor* p = fProcessorList[i];
    if (p->DoesMakeTreeBranches()) {
      p->PrepareTreeBranches(fTree);
      fDoesFillTree = true;
    }
  }
}
//____________________________________________________________________
void SAMURAIDataConverter::PrepareHistograms()
{
  fOutputFile->cd();

  int n = fProcessorList.size();

  if (n==0) std::cout<<"SAMURAIDataConverter: no processor is registered"<<std::endl;

  for (int i=0;i<n;i++){
    SAMURAIDataProcessor* p = fProcessorList[i];
    if (p->DoesMakeHistograms()) p->PrepareHistograms();
  }
}
//____________________________________________________________________
void SAMURAIDataConverter::EventLoop()
{

  int np = fProcessorList.size();
  ULong64_t neve=0;
  while(fEventStore->GetNextEvent()){
    if (neve%1000==0){
      std::cout<<"\r  Converting ...     "<<neve<<" Events"<<std::flush;
    }

    for (int ip=0;ip<np;ip++)
      fProcessorList[ip]->ClearData();
    for (int ip=0;ip<np;ip++)
      fProcessorList[ip]->ReconstructData();

    if (fDoesFillTree) fTree->Fill();

    for (int ip=0;ip<np;ip++){
      SAMURAIDataProcessor *p = fProcessorList[ip];
      if (p->DoesMakeHistograms()) p->FillHistograms();
    }

    fEventStore->ClearData();
    neve ++;
  }//while(estore->GetNextEvent()){
  std::cout<<"\r  Done               "<<neve<<" Events"<<std::endl;
}
//____________________________________________________________________
//____________________________________________________________________
//____________________________________________________________________
