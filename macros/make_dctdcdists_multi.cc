/*
  macro for making TDC distribution of each layer of BDC1,2,FDC1,2
  How to use
  ROOT[] .L sm_online_utils/s057/make_dctdcdists.cc+g
  ROOT[] make_dctdcdists(101) (101 is run number)

  NOTE: check and modify the histogram range
 */


#include "TArtSAMURAIParameters.hh"
#include "TArtEventStore.hh"
#include "TArtCalibBDC1Hit.hh"
#include "TArtCalibBDC2Hit.hh"
#include "TArtCalibFDC1Hit.hh"
#include "TArtCalibFDC2Hit.hh"
#include "TArtDCHit.hh"

#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>

void make_dctdcdists_multi(Int_t nRun=12){

  int neve_max = 10000;
  
  TArtSAMURAIParameters *prm = TArtSAMURAIParameters::Instance();
  prm->LoadParameter("db/SAMURAIBDC1.xml");
  prm->LoadParameter("db/SAMURAIBDC2.xml");
  prm->LoadParameter("db/SAMURAIFDC1.xml");
  prm->LoadParameter("db/SAMURAIFDC2.xml");

  TArtEventStore *estore = new TArtEventStore();
  if (nRun==0) estore->Open(0);
  else         estore->Open(Form("ridf/data%04d.ridf",nRun));

  TArtCalibBDC1Hit *calib_bdc1hit = new TArtCalibBDC1Hit();
  TArtCalibBDC2Hit *calib_bdc2hit = new TArtCalibBDC2Hit();
  TClonesArray* bdc1hits = (TClonesArray*)calib_bdc1hit->GetDCHitArray();
  TClonesArray* bdc2hits = (TClonesArray*)calib_bdc2hit->GetDCHitArray();

  TArtCalibFDC1Hit *calib_fdc1hit = new TArtCalibFDC1Hit();
  TArtCalibFDC2Hit *calib_fdc2hit = new TArtCalibFDC2Hit();
  TClonesArray* fdc1hits = (TClonesArray*)calib_fdc1hit->GetDCHitArray();
  TClonesArray* fdc2hits = (TClonesArray*)calib_fdc2hit->GetDCHitArray();

  TFile *fout = new TFile(Form("db/dc/tdcdists_run%04d.root",nRun),"RECREATE");

  TH1* hbdc1id = new TH1D("hbdc1id","BDC1 ID",128,0.5,128.5);
  hbdc1id->SetMinimum(0);
  TH1* hbdc1idtdc = new TH2D("hbdc1idtdc","BDC1 ID TDC",128,0.5,128.5,100,0,4000);
  
  TH1* hbdc2id = new TH1D("hbdc2id","BDC2 ID",128,0.5,128.5);
  hbdc2id->SetMinimum(0);
  TH1* hbdc2idtdc = new TH2D("hbdc2idtdc","BDC2 ID TDC",128,0.5,128.5,100,0,4000);

  TH1* hfdc1id = new TH1D("hfdc1id","FDC1 ID",448,0.5,448.5);
  hfdc1id->SetMinimum(0);
  TH1* hfdc1idtdc = new TH2D("hfdc1idtdc","FDC1 ID TDC",448,0.5,448.5,100,0,20000);
  
  TH1* hfdc2id = new TH1D("hfdc2id","FDC2 ID",1568,0.5,1568.5);
  hfdc2id->SetMinimum(0);
  TH1* hfdc2idtdc = new TH2D("hfdc2idtdc","FDC2 ID TDC",1568,0.5,1568.5,100,0,20000);


  // BDC1
  TH1* hbdc1tdc[8];
  for (int i=0;i<8;++i){
    hbdc1tdc[i] = new TH1D(Form("hbdc1tdc%d",i),
			   Form("BDC1 TDC layer=%d",i),
			   2000,0,2000);
  }
  // BDC2
  TH1* hbdc2tdc[8];
  for (int i=0;i<8;++i){
    hbdc2tdc[i] = new TH1D(Form("hbdc2tdc%d",i),
			   Form("BDC2 TDC layer=%d",i),
			   2000,0,2000);
  }
		      
  // FDC1
  TH1* hfdc1tdc[14];
  for (int i=0;i<14;++i){
    hfdc1tdc[i] = new TH1D(Form("hfdc1tdc%d",i),
			   Form("FDC1 TDC layer=%d",i),
			   2000,0,2000);
  }
  // FDC2
  TH1* hfdc2tdc[14];
  for (int i=0;i<14;++i){
    hfdc2tdc[i] = new TH1D(Form("hfdc2tdc%d",i),
			   Form("FDC2 TDC layer=%d",i),
			   3000,0,3000);
  }

  int neve = 0;
  while(estore->GetNextEvent() && neve < neve_max){

    if(neve%100==0)
      cout<<"\r event = "<<neve<<" / "<<neve_max<<flush;

    //-----------------------------
    // BDC1,2
    calib_bdc1hit->ClearData();
    calib_bdc1hit->ReconstructData();
    calib_bdc2hit->ClearData();
    calib_bdc2hit->ReconstructData();

    TArtDCHit* hit1[8] = {nullptr};
    
    Int_t nbdc1hit = bdc1hits->GetEntries();
    for(int i=0;i<nbdc1hit;++i){
      TArtDCHit *hit = (TArtDCHit*)bdc1hits->At(i);
      Int_t id = hit->GetID();
      Int_t tdc = hit->GetTDC();
      Int_t layer = hit->GetLayer();
      //hbdc1id->Fill(id);
      //hbdc1idtdc->Fill(id, tdc);
      //hbdc1tdc[layer]->Fill(tdc);

      // store minimum TDC hit
      if (!hit1[layer]){
	hit1[layer] = hit; 
      }else{
	TArtDCHit *hit0 = hit1[layer];
	if (hit->GetTDC() < hit0->GetTDC())
	  hit1[layer] = hit;
      }
    }

    for (int i=0;i<8;++i){
      if (hit1[i]){
	TArtDCHit* hit = hit1[i];
	Int_t id = hit->GetID();
	Int_t tdc = hit->GetTDC();
	hbdc1id->Fill(id);
	hbdc1idtdc->Fill(id,tdc);
	hbdc1tdc[i]->Fill(tdc);	
      }
    }


    TArtDCHit* hit2[8] = {nullptr};
    
    Int_t nbdc2hit = bdc2hits->GetEntries();
    for(int i=0;i<nbdc2hit;++i){
      TArtDCHit *hit = (TArtDCHit*)bdc2hits->At(i);
      Int_t id = hit->GetID();
      Int_t tdc = hit->GetTDC();
      Int_t layer = hit->GetLayer();
      //hbdc2id->Fill(id);
      //hbdc2idtdc->Fill(id, tdc);
      //hbdc2tdc[layer]->Fill(tdc);

      // store minimum TDC hit
      if (!hit2[layer]){
	hit2[layer] = hit; 
      }else{
	TArtDCHit *hit0 = hit2[layer];
	if (hit->GetTDC() < hit0->GetTDC())
	  hit2[layer] = hit;
      }

    }

    for (int i=0;i<8;++i){
      if (hit2[i]){
	TArtDCHit* hit = hit2[i];
	Int_t id = hit->GetID();
	Int_t tdc = hit->GetTDC();
	hbdc2id->Fill(id);
	hbdc2idtdc->Fill(id,tdc);
	hbdc2tdc[i]->Fill(tdc);	
      }
    }

    //-----------------------------
    // FDC1,2
    calib_fdc1hit->ClearData();
    calib_fdc1hit->ReconstructData();
    calib_fdc2hit->ClearData();
    calib_fdc2hit->ReconstructData();

    Int_t nfdc1hit = fdc1hits->GetEntries();
    for(int i=0;i<nfdc1hit;++i){
      TArtDCHit *hit = (TArtDCHit*)fdc1hits->At(i);
      Int_t id = hit->GetID();
      Int_t tdc = hit->GetTDC();
      Int_t layer = hit->GetLayer();
      hfdc1id->Fill(id);
      hfdc1idtdc->Fill(id, hit->GetTDC());
      hfdc1tdc[layer]->Fill(tdc);      


    }

    Int_t nfdc2hit = fdc2hits->GetEntries();
    for(int i=0;i<nfdc2hit;++i){
      TArtDCHit *hit = (TArtDCHit*)fdc2hits->At(i);
      Int_t id = hit->GetID();
      Int_t tdc = hit->GetTDC();
      Int_t layer = hit->GetLayer();
      hfdc2id->Fill(id);
      hfdc2idtdc->Fill(id, hit->GetTDC());
      hfdc2tdc[layer]->Fill(tdc);      
    }

    estore->ClearData();
    neve++;
  }

  cout<<"\r event = "<<neve<<" / "<<neve_max<<flush;
  cout<<endl;
  
  fout->Write();
  fout->Close();
  
  

}
