// usage
// root [0] .L macros/onlinemonitor/OnlineMonitor.cc+g
// root [1] OnlineMonitor* mon = new OnlineMonitor()
// root [2] mon->Run()
//
#include "OnlineMonitor.hh"

#include <iostream>
#include <csignal>

#include "TSystem.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TDatime.h"
#include "TText.h"
#include "TClonesArray.h"

#include "TArtStoreManager.hh"
#include "TArtRawEventObject.hh"
#include "TArtRawSegmentObject.hh"
#include "TArtEventInfo.hh"

#include "CoinDataProcessor.hh"
#include "PlasticDataProcessor.hh"
#include "BDCDataProcessor.hh"
#include "FDC0DataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "NEBULADataProcessor.hh"
#include "HODPlaDataProcessor.hh"
#include "NINJADataProcessor.hh"
#include "CATANADataProcessor.hh"
#include "FSDBSDDataProcessor.hh"
#include "PDCDataProcessor.hh"


//_________________________________________________________________________________
// function to exit loop at keyboard interrupt.
bool stoploop = false;
void stop_interrupt(int) {
  printf("keyboard interrupt\n");
  stoploop = true;
}
//_________________________________________________________________________________
void OnlineMonitor::Run(ULong64_t nmax)// main 
{
  if(nmax>0) fNeveMax = nmax ;
  if (!fInitialize) {
    Init();
  }

  if (!fIsHistBooked) {
    BookHist();
    BookUserHist();
  }

  EventLoop();
}
//_________________________________________________________________________________
void OnlineMonitor::Init()
{
  // Path for Input files
  const char *TDCdist = "rootfiles/dctdc/run0175_tdcSpectrum.root";

  // change for your experiment
  fProcessorArray.push_back(new CoinDataProcessor);
  fProcessorArray.push_back(new PlasticDataProcessor);
  fProcessorArray.push_back(new BDCDataProcessor(TDCdist));
  fProcessorArray.push_back(new FDC1DataProcessor(TDCdist));
  fProcessorArray.push_back(new FDC2DataProcessor(TDCdist));
  fProcessorArray.push_back(new HODPlaDataProcessor);
  fProcessorArray.push_back(new NEBULADataProcessor);
  fProcessorArray.push_back(new NINJADataProcessor);
  fProcessorArray.push_back(new FSDBSDDataProcessor);
  fProcessorArray.push_back(new CATANADataProcessor);
  fProcessorArray.push_back(new PDCDataProcessor);
  

  fnx=4;
  fny=4;
  fResetCount=20000;
  fDrawTimeInterval=10;// sec

  festore = new TArtEventStore();
  fNeve = 0;
  fNeve_monitor = 0;

  fInitialize = true;

}
//_________________________________________________________________________________
void OnlineMonitor::BookUserHist()
{
  fRootFile->cd();

  // add user histograms
  fhpla_pid = new TH2D("pla_pid","PLA ToF713-dE13",500,-23000,-20500,500,400,1200);
  fHistArray.push_back(fhpla_pid);
  
   
  fheff_bdc1 = new TH2D("bdc1_eff","BDC1 when BDC2&FDC1 hit",100,-50,50, 100,-50,50);
  fheff_bdc2 = new TH2D("bdc2_eff","BDC2 when BDC1&FDC1 hit",100,-50,50, 100,-50,50);
  fheff_fdc1 = new TH2D("fdc1_eff","FDC1 when BDC1&BDC2 hit",100,-150,150, 100,-150,150);
  fheff      = new TH1D("heff","Efficiency for BDC1 BDC2 FDC1",6,0.5,3.5);
  //fHistArray.push_back(fheff_bdc1);
  //fHistArray.push_back(fheff_bdc2);
  //fHistArray.push_back(fheff_fdc1);
  fHistArray.push_back(fheff);


  fIsHistBooked = true;
}
//______________________________________________________________________________
void OnlineMonitor::FillUserHist()
{
  TArtStoreManager *sman = TArtStoreManager::Instance();
  //TClonesArray *a = sman->FindDataContainer("NEBULAPla");

  //--------------------------------------
  // Beam PID
  TClonesArray *pla_array = (TClonesArray*)sman->FindDataContainer("BigRIPSPlastic");

  TArtPlastic *F7Pla = 0;
  TArtPlastic *F13Pla_1 = 0;

  int n=pla_array->GetEntries();
  for (Int_t i=0;i<n;++i){
    TArtPlastic* pla = (TArtPlastic*)pla_array->At(i);
    Int_t ID= pla->GetID();
    if (ID==3){// F7 plastic
      F7Pla = pla;
    }else if (ID==4){
      F13Pla_1 = pla;
    }
  }

  if (F7Pla !=0 && F13Pla_1 !=0){
    double F7T = 0.5*(F7Pla->GetTLRaw()+F7Pla->GetTRRaw());
    double F13T1 = 0.5*(F13Pla_1->GetTLRaw()+F13Pla_1->GetTRRaw());
    double F7Q = sqrt(F7Pla->GetQLRaw()*F7Pla->GetQRRaw());
    double F13Q1 = sqrt(F13Pla_1->GetQLRaw()*F13Pla_1->GetQRRaw());
    fhpla_pid->Fill(F13T1-F7T,F7Q);
  }

  //--------------------------------------
  // check efficiency of DCs
  //
  bool isBDC1OK = false;
  bool isBDC2OK = false;
  bool isFDC1OK = false;
  double fBDC1_X = -9999, fBDC1_Y= -9999,fBDC1_ThetaX= -9999,fBDC1_ThetaY= -9999;
  double fBDC2_X = -9999, fBDC2_Y= -9999,fBDC2_ThetaX= -9999,fBDC2_ThetaY= -9999;
  double fFDC1_X = -9999, fFDC1_Y= -9999,fFDC1_ThetaX= -9999,fFDC1_ThetaY= -9999;

  // BDC1 Track
  TClonesArray *BDC1Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC1Track");
  
  if (BDC1Tracks) {
    Int_t BDC1NumberOfTracks = BDC1Tracks->GetEntries();
    Double_t TempXPosition=-9999, TempYPosition=-9999, TempChi2 = 1e6, MinChi2x =1e6, MinChi2y =1e6;
//    std::cout << BDC1NumberOfTracks << std::endl;
    if(BDC1NumberOfTracks > 0) {
      TArtDCTrack *TrackBDC1;
      
      for(Int_t i = 0; i<BDC1NumberOfTracks; i++) {
        TrackBDC1 = (TArtDCTrack *)BDC1Tracks->At(i);
        
	if(TrackBDC1) {

	  TempXPosition = TrackBDC1->GetPosition(0);
  	  TempYPosition = TrackBDC1->GetPosition(1);
	  TempChi2 = TrackBDC1->GetChi2() / (Double_t)TrackBDC1->GetNDF();
 
	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fBDC1_X = TempXPosition;
  	      fBDC1_ThetaX = TMath::ATan(TrackBDC1->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fBDC1_Y = TempYPosition;
  	      fBDC1_ThetaY = TMath::ATan(TrackBDC1->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      isBDC1OK = true;
    }  
  }

  // BDC2 Track
  TClonesArray *BDC2Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIBDC2Track");

  if (BDC2Tracks) {
    Int_t BDC2NumberOfTracks = BDC2Tracks->GetEntries();
    Double_t TempXPosition=-9999, TempYPosition=-9999, TempChi2 = 1e6, MinChi2x =1e6, MinChi2y =1e6;

    if(BDC2NumberOfTracks > 0) {
      TArtDCTrack *TrackBDC2;
      
      for(Int_t i = 0; i<BDC2NumberOfTracks; i++) {
        TrackBDC2 = (TArtDCTrack *)BDC2Tracks->At(i);
        
	if(TrackBDC2) {

	  TempXPosition = TrackBDC2->GetPosition(0);
  	  TempYPosition = TrackBDC2->GetPosition(1);
	  TempChi2 = TrackBDC2->GetChi2() / (Double_t)TrackBDC2->GetNDF();

	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fBDC2_X = TempXPosition;
  	      fBDC2_ThetaX = TMath::ATan(TrackBDC2->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fBDC2_Y = TempYPosition;
  	      fBDC2_ThetaY = TMath::ATan(TrackBDC2->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      isBDC2OK = true;
    }      
  }

  // FDC1 Track
  TClonesArray *FDC1Tracks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");

  if (FDC1Tracks) {
    Int_t FDC1NumberOfTracks = FDC1Tracks->GetEntries();
    Double_t TempXPosition=-9999, TempYPosition=-9999, TempChi2 = 1e6, MinChi2x =1e6, MinChi2y =1e6;

    if(FDC1NumberOfTracks > 0) {
      TArtDCTrack *TrackFDC1;
      
      for(Int_t i = 0; i<FDC1NumberOfTracks; i++) {
        TrackFDC1 = (TArtDCTrack *)FDC1Tracks->At(i);
        
	if(TrackFDC1) {

	  TempXPosition = TrackFDC1->GetPosition(0);
  	  TempYPosition = TrackFDC1->GetPosition(1);
	  TempChi2 = TrackFDC1->GetChi2() / (Double_t)TrackFDC1->GetNDF();

	  if(TempChi2 > 0) {
	  
	    if(TMath::Abs(TempXPosition) < 5000 && TempChi2 < MinChi2x) {
	      fFDC1_X = TempXPosition;
  	      fFDC1_ThetaX = TMath::ATan(TrackFDC1->GetAngle(0));
	      MinChi2x = TempChi2;
	    }	      

	    if(TMath::Abs(TempYPosition) < 5000 && TempChi2 < MinChi2y) {
	      fFDC1_Y = TempYPosition;
  	      fFDC1_ThetaY = TMath::ATan(TrackFDC1->GetAngle(1));
	      MinChi2y = TempChi2;
	    }	      
	  }
	}
      }

      isFDC1OK = true;
      //fheff_fdc1->Fill(fFDC1_X,fFDC1_Y);
    }      
  }

  bool isDSBOK = false;
  for(int i=0;i<8;i++){
	  CoinDataProcessor *test = (CoinDataProcessor*)fProcessorArray[0];
	  if(test->GetCoinBit(i)&&i==1){isDSBOK = true;}
  }    

  if(isDSBOK&&isBDC1OK&&isBDC2OK&&abs(fBDC1_X)<40&&abs(fBDC1_Y)<40&&abs(fBDC2_X)<40&&abs(fBDC2_Y)<40) fheff_fdc1->Fill(fFDC1_X,fFDC1_Y);
  if(isDSBOK&&isBDC1OK&&isFDC1OK&&abs(fBDC1_X)<40&&abs(fBDC1_Y)<40&&abs(fFDC1_X)<40&&abs(fFDC1_Y)<40) fheff_bdc2->Fill(fBDC2_X,fBDC2_Y);
  if(isDSBOK&&isBDC2OK&&isFDC1OK&&abs(fBDC2_X)<40&&abs(fBDC2_Y)<40&&abs(fFDC1_X)<40&&abs(fFDC1_Y)<40) fheff_bdc1->Fill(fBDC1_X,fBDC1_Y);

  fheff->SetBinContent(1, (double)(fheff_bdc1->Integral())/fheff_bdc1->GetEntries()*100);
  fheff->SetBinContent(3, (double)(fheff_bdc2->Integral())/fheff_bdc2->GetEntries()*100);
  fheff->SetBinContent(5, (double)(fheff_fdc1->Integral())/fheff_fdc1->GetEntries()*100);

  if((int)fheff_bdc1->GetEntries()%100==0){
	  cout<<Form("BDC1 Eff = %4.2f percent",(double)(fheff_bdc1->Integral())/(double)(fheff_bdc1->GetEntries())*100)<<endl;
	  cout<<Form("BDC2 Eff = %4.2f percent",(double)(fheff_bdc2->Integral())/(double)(fheff_bdc2->GetEntries())*100)<<endl;
	  cout<<Form("FDC1 Eff = %4.2f percent",(double)(fheff_fdc1->Integral())/(double)(fheff_fdc1->GetEntries())*100)<<endl;
  }



  //--------------------------------------


}
//_________________________________________________________________________________
//_________________________________________________________________________________
//_________________________________________________________________________________
//_________________________________________________________________________________
OnlineMonitor::OnlineMonitor(TString RidfFile)
  : festore(0),
    fInitialize(false), fIsHistBooked(false),
    fRunName(RidfFile),
    fResetCount(20000), fDrawTimeInterval(10),
    fDoesReset(true),
    f_iplot(0)
{
  if (gPad==0) fCanvas = new TCanvas("c1","c1",1200,800);
  else         fCanvas = gPad->GetCanvas();

  std::cout<<"OnlineMonitor::Run()"<<std::endl;
}
//_________________________________________________________________________________
OnlineMonitor::~OnlineMonitor()
{
  fRootFile->Close();
  delete festore;
}
//_________________________________________________________________________________
void OnlineMonitor::BookHist()
{
  fRootFile = new TFile("OnlineMonitor.root","recreate");

  Int_t np = fProcessorArray.size();
  for (Int_t ip=0;ip<np;++ip){
    SAMURAIDataProcessor *p = fProcessorArray[ip];
    p->MakeHistograms(true);
    p->PrepareHistograms();

    vector<TH1*>* arr = p->GetHistArray();
    Int_t nhist = arr->size();
    for (Int_t ihist=0;ihist<nhist;++ihist){
      fHistArray.push_back( (*arr)[ihist] );
    }
  }
  
  fIsHistBooked = true;
}
//_________________________________________________________________________________
void OnlineMonitor::AnalyzeOneEvent()
{
  TArtStoreManager *sman = TArtStoreManager::Instance();
  if(festore->GetNextEvent()){

    TArtRawEventObject *rawevent = festore->GetRawEventObject();
    if (rawevent->GetEventNumber()<0) {
      TArtCore::Error("OnlineMonitor.cc","Invalid event ID=%d",rawevent->GetEventNumber());
    }


    Int_t np = fProcessorArray.size();

    for (int ip=0;ip<np;ip++) fProcessorArray[ip]->ClearData();
    for (int ip=0;ip<np;ip++) fProcessorArray[ip]->ReconstructData();

    for (int ip=0;ip<np;ip++){
      SAMURAIDataProcessor *p = fProcessorArray[ip];
      if (p->DoesMakeHistograms()) p->FillHistograms();
    }

    FillUserHist();

    //------------------------------------------
    festore->ClearData();
  }
}
//______________________________________________________________________________
void OnlineMonitor::EventLoop()
{
  signal(SIGINT, stop_interrupt);
  if (fRunName=="online"){
    festore->Open(0);
  }else if (fRunName=="nebula"){
    festore->Open(1);
  }else {
    festore->Open(fRunName.Data());
  }

  TDatime datime_old;
//  Double_t neve_rate = 0;
  while(true){
    gSystem->ProcessEvents();
    if (fNeve%100==0)
      std::cout<<"\r"<<fNeve_monitor<<" events for display, "
	       <<fNeve<<" events analyzed"<<std::flush;

    if (stoploop || (fNeve>=fNeveMax)) {
      stoploop = false;
      break;
    }

    EEventType event = static_cast<EEventType>(fCanvas->GetEvent());
    if(event == kKeyPress){
      std::cout<<std::endl;
      int ret = GetKeyCommand();
      if (ret==-1) break;
    }

    TDatime datime;

    AnalyzeOneEvent();

    ++fNeve;
    ++fNeve_monitor;

    if (datime.GetTime() - datime_old.GetTime() > fDrawTimeInterval){
      //neve_rate = (Double_t)fNeve/(datime.GetTime() - datime_old.GetTime());

      Draw();
      //std::cout<<"\r    "<<fNeve_monitor<<" evts are accumulated (Ctrl+C for stop) "<<std::flush;
      fRootFile->Write();

      datime_old = datime;

      if (fDoesReset){
	if (fNeve_monitor>fResetCount){
	  ResetHist();
	  fNeve_monitor = 0;
	}
      }
    }//if (datime.GetTime() - datime_old.GetTime() > fDrawTimeInterval){

  }//while(true){

}
//______________________________________________________________________________
bool OnlineMonitor::Draw()
{
  bool retval = false;

  Int_t nxy = fnx*fny;

  Int_t npad=1;
  TCanvas *c1 = gPad->GetCanvas();
  c1->Clear();
  TDatime datime;
  TString title=fRunName + " (";

  title+=datime.AsString();
  title+=")";
  c1->SetTitle(title.Data());
  c1->Divide(fnx,fny);

  Int_t nhist = fHistArray.size();
  for(Int_t ihist=0;ihist<nxy;++ihist){
    c1->cd(npad);

    if ( f_iplot >= nhist ) f_iplot = 0;
    TH1* hist = fHistArray[f_iplot];
    if (hist->GetNbinsY()>0) hist->Draw("colz");
    else                     hist->Draw();

    npad++;
    if (npad>nxy) npad=1;
    f_iplot++;
    if (f_iplot==nhist) {
      //f_iplot=0;
      retval = true;
      break;
    }

  }

  gPad->Update();
  return retval;
}
//_________________________________________________________________________________
void OnlineMonitor::ResetHist()
{
  Int_t nhist = fHistArray.size();
  for(Int_t ihist=0;ihist<nhist;++ihist){
    TH1* hist = fHistArray[ihist];
    hist->Reset("M");
  }
}
//______________________________________________________________________________
void OnlineMonitor::Print(const char* fname)
{
  fCanvas->Print(Form("%s[",fname));
  f_iplot=0;
  bool IsEnd=false;
  while (!IsEnd){
    IsEnd = Draw();
    fCanvas->Print(Form("%s",fname));
  }
  fCanvas->Print(Form("%s]",fname));
}
//______________________________________________________________________________
int OnlineMonitor::GetKeyCommand()
{


  Int_t nhist = fHistArray.size();
  Int_t nxy = fnx*fny;

  while(true){
    gSystem->ProcessEvents();

    std::cout<<"\r [s]:Start, [q]:Quit, [n]:Next, [b]:Back, [r]:Reset, [p]:Print >"
	     <<std::flush;

    system("stty raw"); 
    char input = getchar(); 
    system("stty cooked"); 

    if(input =='q'){
      std::cout<<std::endl;
      return -1;

    } else if (input =='n'){
      Draw();

    } else if (input =='b'){
      Int_t num = gPad->GetNumber();
      f_iplot = nhist - num - nxy;
      if (f_iplot<0){
	Int_t remain = nhist%nxy;
	if (remain==0) f_iplot = nhist - nxy;
	else           f_iplot = nhist - remain;
      }
      Draw();

    } else if (input =='r'){
      ResetHist();

    } else if (input =='p'){
      TString fname;
      std::cout<<std::endl<<"file name >"<<std::flush;
      std::cin>>fname;
      std::cout<<"   making "<<fname.Data()<<" ..."<<std::endl;
      Print(fname.Data());
      std::cout<<" Done"<<std::endl;

    } else if (input =='s'){
      std::cout<<std::endl;
      break;
    }

  }
  return 0;
}
//______________________________________________________________________________
