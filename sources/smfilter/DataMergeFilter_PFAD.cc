#include "DataMergeFilter_PFAD.hh"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <TString.h>

#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include <TBranch.h>
#include <TPad.h>
#include <TCut.h>
#include <TVector3.h>

#include "PfadPhysics.h"
//_________________________________________________________
void DataMergeFilter_PFAD::Run(const char* fname_sm,
			       const char* fname_pfad,
			       const char* fname_out){

  SetInputSMFileName(fname_sm);
  SetInputDetFileName(fname_pfad);
  bool IsFileOK = SetOutputFileName(fname_out);
  if (! IsFileOK ) return;

  SetInputSMTreeName("t_smts");
  SetInputDetTreeName("PhysicsTree");
  SetTS2us_sm(0.01);
  SetTS2us_det(0.04);
  SetTSMatchWindow(10);// us
  SetBranchName_smTS("smts");
  SetBranchName_detTS("LTS");
  
  FindRunStart();
  FindRunEnd(850000);// temp for 230MeV run07

  TSCombinations("start");
  CorrectTSPeriod();
//  FindTSOffset("start");// included in CorrectTSPeriod()
  Filter();
  MakePlots();
  
}
//_________________________________________________________
void DataMergeFilter_PFAD::Filter(){
  std::cout<<"Filtering PFAD Data ...  "<<std::endl;
  OpenOutputFile();
  OpenInputSMFile();
  OpenInputDetFile();

  if (fMatchWindow_us<0){
    std::cout<<"TS Match Window is set to 1000 us"<<std::endl;
    fMatchWindow_us = 1000;
  }
  TString log(Form("Match Window = +- %f us",fMatchWindow_us));
  AddLog(log.Data());
  
  Long64_t ista_det = fNsta_det - 0.25*fNsrch_det;
  if (ista_det<0) ista_det = 0;
  
  fTree_det->SetBranchStatus("*",1);// activate all the branches
  
  ULong64_t TS_sm=0;
  fTree_sm->SetBranchAddress(fBrName_smTS.Data(),&TS_sm);

  ULong64_t TS_det=0;
  // ***************************************
  nptool::pfad::PfadPhysics *pfadphysics = 0;
  fTree_det->SetBranchAddress("Pfad",&pfadphysics);
  // ...
  // ...
  // ***************************************
  
  fFile_out->cd();
  if (fTreeName_out.Length()==0){
    fTreeName_out = fTreeName_det;
    fTreeName_out.Append("_flt");
  }
  fTree_out = new TTree(fTreeName_out.Data(),"Filtered tree");
  Bool_t IsTSMatched=false;
  Double_t dTS_min=0;
  fTree_out->Branch("IsTSMatched",&IsTSMatched);
  fTree_out->Branch("dTS",&dTS_min);
  //fTree_out->Branch(fBrName_detTS.Data(),&TS_det);
  // ***************************************
  fTree_out->Branch("pfad","nptool::pfad::PfadPhysics",&pfadphysics);
  // ...
  // ***************************************

  Long64_t N_sm = fTree_sm->GetEntries();

  Long64_t i_sm=0;
  Long64_t i_det=ista_det;
  Long64_t i_det_match=ista_det;
  Long64_t N_match=0;

  for (i_sm=0;i_sm<N_sm;++i_sm){
    if (i_sm%1000==0)
      std::cout<<"\r   Event: "<<i_sm<<" / "<<N_sm<<" "
	       <<std::flush;

    fTree_sm->GetEntry(i_sm);

    // Initilalization of tree variables
    IsTSMatched=false;
    TS_det = 0;
    dTS_min=fMatchWindow_us;
    // ***************************************
    pfadphysics->clear();
    // ***************************************

    while( i_det < fNend_det ){
      fTree_det->GetEntry(i_det);

      // ***************************************
      TS_det = pfadphysics->LTS;
      // ***************************************

      if (TS_det==0) {// skip event without TS
	i_det++;
	continue;
      }
      
      Double_t TS_sm_us  = fTS2us_sm  * TS_sm; // us
      Double_t TS_det_us = fTS2us_det * TS_det;
      Double_t dTS = TS_det_us - fTSoff_sta_us - TS_sm_us;

      if (TMath::Abs(dTS)<fMatchWindow_us){

	IsTSMatched=true;
	if (TMath::Abs(dTS) < TMath::Abs(dTS_min)){
	  dTS_min = dTS;
	  i_det_match = i_det;
	}

      }else if (dTS > fMatchWindow_us){
	break;
      }

      i_det++;
    }//while( i_det < fNend_det ){

    if (IsTSMatched){
      fTree_det->GetEntry(i_det_match);
      N_match++;
    }else{// input dummy date
      TS_det = 0;
      dTS_min = 0;
      pfadphysics->clear();
    }
    fTree_out->Fill();

    i_det = i_det_match + 1;
    

  }//for (i_sm=0;i_sm<N_sm;++i_sm){
  std::cout<<"\r   Event: "<<i_sm<<" / "<<N_sm
	   <<std::endl;
  
  log = Form("N_match/N_sm=%lld/%lld (%4.1f%%)",
	     N_match,N_sm,100.*N_match/N_sm);
  AddLog(log.Data());
  std::cout<<log.Data()<<std::endl;

  fTree_out->Write();
  fFile_out->Close();
  fFile_sm->Close();
  fFile_det->Close();
}
