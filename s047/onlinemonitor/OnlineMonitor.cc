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
  //const char *TDCdist = "rootfiles/dctdc/run0617_tdcSpectrum.root";
  const char *TDCdist = "rootfiles/dctdc/run0628_tdcSpectrum.root";// 11Li run

  // change for your experiment
  fProcessorArray.push_back(new CoinDataProcessor);
  fProcessorArray.push_back(new PlasticDataProcessor);
  fProcessorArray.push_back(new BDCDataProcessor(TDCdist));
  fProcessorArray.push_back(new FDC1DataProcessor(TDCdist));
  fProcessorArray.push_back(new FDC2DataProcessor(TDCdist));
  fProcessorArray.push_back(new HODPlaDataProcessor);
  fProcessorArray.push_back(new NEBULADataProcessor);
  //fProcessorArray.push_back(new NINJADataProcessor);
  //fProcessorArray.push_back(new FSDBSDDataProcessor);
  fProcessorArray.push_back(new CATANADataProcessor);
  fProcessorArray.push_back(new PDCDataProcessor);
  

  fnx=4;
  fny=4;
  fResetCount=200000;
  fDrawTimeInterval=10;// sec

  festore = new TArtEventStore();
  fNeve = 0;
  fNeve_monitor = 0;

  fInitialize = true;


  LoadBeamInterestedCut();
}
//_________________________________________________________________________________
void OnlineMonitor::BookUserHist()
{
  fRootFile->cd();

  // add user histograms
  fhpla_pid = new TH2D("pla_pid","PLA ToF713-dE13",400,-23000,-19500,200,0,1200);
  fHistArray.push_back(fhpla_pid);
  
   
  fheff_bdc1 = new TH2D("bdc1_eff","BDC1 when BDC2&FDC1 hit",100,-50,50, 100,-50,50);
  fheff_bdc2 = new TH2D("bdc2_eff","BDC2 when BDC1&FDC1 hit",100,-50,50, 100,-50,50);
  fheff_fdc1 = new TH2D("fdc1_eff","FDC1 when BDC1&BDC2 hit",100,-150,150, 100,-150,150);
  fheff      = new TH1D("heff","Efficiency for BDC1 BDC2 FDC1",6,0.5,3.5);

  fheff->SetMinimum(0.);
  fheff->SetMaximum(110.);

  //fHistArray.push_back(fheff_bdc1);
  //fHistArray.push_back(fheff_bdc2);
  //fHistArray.push_back(fheff_fdc1);
  fHistArray.push_back(fheff);


  fIsHistBooked = true;
}
void OnlineMonitor::LoadBeamInterestedCut()
{
   //beamInterestedCut = new TCutG("beam10Be",10);
   //beamInterestedCut->SetVarX("PLA ToF713-dE13");
   //beamInterestedCut->SetVarY("");
   //beamInterestedCut->SetTitle("Graph");
   //beamInterestedCut->SetFillColor(1);
   //beamInterestedCut->SetPoint(0,-22885.2,685.505);
   //beamInterestedCut->SetPoint(1,-22919.4,604.072);
   //beamInterestedCut->SetPoint(2,-22895,472.15);
   //beamInterestedCut->SetPoint(3,-22870.5,411.889);
   //beamInterestedCut->SetPoint(4,-22816.7,426.547);
   //beamInterestedCut->SetPoint(5,-22775.9,560.098);
   //beamInterestedCut->SetPoint(6,-22764.5,665.961);
   //beamInterestedCut->SetPoint(7,-22803.6,711.564);
   //beamInterestedCut->SetPoint(8,-22867.2,698.534);
   //beamInterestedCut->SetPoint(9,-22885.2,685.505);
 
   //beamInterestedCut = new TCutG("beam4He",11);
   //beamInterestedCut->SetPoint(0,-22326.4,406.304);
   //beamInterestedCut->SetPoint(1,-22377.9,367.748);
   //beamInterestedCut->SetPoint(2,-22382.8,325.52);
   //beamInterestedCut->SetPoint(3,-22341.1,299.816);
   //beamInterestedCut->SetPoint(4,-22211.1,290.636);
   //beamInterestedCut->SetPoint(5,-22135.1,320.012);
   //beamInterestedCut->SetPoint(6,-22115.5,386.108);
   //beamInterestedCut->SetPoint(7,-22140,424.663);
   //beamInterestedCut->SetPoint(8,-22257.8,426.499);
   //beamInterestedCut->SetPoint(9,-22299.5,417.319);
   //beamInterestedCut->SetPoint(10,-22326.4,406.304);

   //beamInterestedCut = new TCutG("proton",9);
   //beamInterestedCut->SetPoint(0,-22676.4,283.225);
   //beamInterestedCut->SetPoint(1,-22441.5,307.655);
   //beamInterestedCut->SetPoint(2,-22333.9,309.283);
   //beamInterestedCut->SetPoint(3,-22337.2,258.795);
   //beamInterestedCut->SetPoint(4,-22526.4,247.394);
   //beamInterestedCut->SetPoint(5,-22678,239.251);
   //beamInterestedCut->SetPoint(6,-22671.5,268.567);
   //beamInterestedCut->SetPoint(7,-22671.5,268.567);
   //beamInterestedCut->SetPoint(8,-22676.4,283.225);

  //Last working version
  //beamInterestedCut = new TCutG("beam11Li",5);
  //beamInterestedCut->SetPoint(0,-22237.7,519.381);
  //beamInterestedCut->SetPoint(1,-21984.9,519.381);
  //beamInterestedCut->SetPoint(2,-21934.3,428.176);
  //beamInterestedCut->SetPoint(3,-22014.2,250.651);
  //beamInterestedCut->SetPoint(4,-22218.1,227.85);
  //beamInterestedCut->SetPoint(5,-22254,317.427);
  //beamInterestedCut->SetPoint(6,-22263.8,452.606);
  //beamInterestedCut->SetPoint(7,-22237.7,519.381);

  // beamInterestedCut = new TCutG("beam11Li",11);
  // beamInterestedCut->SetPoint(0,-22220.8,814.286);
  // beamInterestedCut->SetPoint(1,-22234,629.221);
  // beamInterestedCut->SetPoint(2,-22249.8,451.948);
  // beamInterestedCut->SetPoint(3,-22249.8,274.675);
  // beamInterestedCut->SetPoint(4,-22162.6,146.104);
  // beamInterestedCut->SetPoint(5,-22043.5,136.364);
  // beamInterestedCut->SetPoint(6,-21940.4,333.117);
  // beamInterestedCut->SetPoint(7,-21937.7,631.169);
  // beamInterestedCut->SetPoint(8,-21993.3,868.831);
  //beamInterestedCut->SetPoint(9,-22141.4,872.727);
  //beamInterestedCut->SetPoint(10,-22220.8,814.286);

  // // Unitl Run767 
  //  beamInterestedCut = new TCutG("beam9Li",5);
  //  beamInterestedCut->SetPoint(0,-22203,753.497);
  //  beamInterestedCut->SetPoint(1,-22210.5,92.8756);
  //  beamInterestedCut->SetPoint(2,-22007.9,98.7047);
  //  beamInterestedCut->SetPoint(3,-22019.2,757.383);
  //  beamInterestedCut->SetPoint(4,-22203,753.497);

   // From Run 768
   beamInterestedCut = new TCutG("beam6He",5);
   beamInterestedCut->SetPoint(0,-22325.,245.);
   beamInterestedCut->SetPoint(1,-22210.,245.);
   beamInterestedCut->SetPoint(2,-22210.,292.);
   beamInterestedCut->SetPoint(3,-22325.,292.);
   beamInterestedCut->SetPoint(4,-22325.,245.);

   // Run 617
   //beamInterestedCut = new TCutG("beam11Li",5);
   //beamInterestedCut->SetPoint(0,-22316.8,543.133);
   //beamInterestedCut->SetPoint(1,-22306.7,414.378);
   //beamInterestedCut->SetPoint(2,-21856,425.107);
   //beamInterestedCut->SetPoint(3,-21889.6,547.425);
   //beamInterestedCut->SetPoint(4,-22316.8,543.133);

   beamInterestedCut->SetLineColor(2);
   beamInterestedCut->SetLineWidth(2);
   beamInterestedCut->SetLineStyle(2);
}

//______________________________________________________________________________
void OnlineMonitor::FillUserHist()
{
  TArtStoreManager *sman = TArtStoreManager::Instance();
  //TClonesArray *a = sman->FindDataContainer("NEBULAPla");

  bool isDSBOK = false;
  for(int i=0;i<8;i++){
	  CoinDataProcessor *test = (CoinDataProcessor*)fProcessorArray[0];
	  if(test->GetCoinBit(i)&&i==1){isDSBOK = true;}
  }    

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

  if (isDSBOK&&F7Pla !=0 && F13Pla_1 !=0){
    double F7T = 0.5*(F7Pla->GetTLRaw()+F7Pla->GetTRRaw());
    double F13T1 = 0.5*(F13Pla_1->GetTLRaw()+F13Pla_1->GetTRRaw());
    double F7Q = sqrt(F7Pla->GetQLRaw()*F7Pla->GetQRRaw());
    double F13Q1 = sqrt(F13Pla_1->GetQLRaw()*F13Pla_1->GetQRRaw());
    fhpla_pid->Fill(F13T1-F7T,F7Q);
    NumBeam++;
    if(beamInterestedCut->IsInside(F13T1-F7T,F7Q)) NumBeamInterested++;
 

  }
  if(fNeve==1000 || fNeve%5000==0 ){
    cout<<endl;
    cout<<Form("Beam interested = %4.2f percent",(double)NumBeamInterested/(double)NumBeam*100)<<endl;
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


  if(isDSBOK&&isBDC1OK&&isBDC2OK&&abs(fBDC1_X)<20&&abs(fBDC1_Y)<20&&abs(fBDC2_X)<20&&abs(fBDC2_Y)<20) fheff_fdc1->Fill(fFDC1_X,fFDC1_Y);
  if(isDSBOK&&isBDC1OK&&isFDC1OK&&abs(fBDC1_X)<20&&abs(fBDC1_Y)<20&&abs(fFDC1_X)<20&&abs(fFDC1_Y)<20) fheff_bdc2->Fill(fBDC2_X,fBDC2_Y);
  if(isDSBOK&&isBDC2OK&&isFDC1OK&&abs(fBDC2_X)<20&&abs(fBDC2_Y)<20&&abs(fFDC1_X)<20&&abs(fFDC1_Y)<20) fheff_bdc1->Fill(fBDC1_X,fBDC1_Y);

  double eff_bdc1 = 0.;
  double eff_bdc2 = 0.;
  double eff_fdc1 = 0.;

  if (fheff_bdc1->GetEntries()>0) eff_bdc1 = fheff_bdc1->Integral()/fheff_bdc1->GetEntries()*100;
  if (fheff_bdc2->GetEntries()>0) eff_bdc2 = fheff_bdc2->Integral()/fheff_bdc2->GetEntries()*100;
  if (fheff_fdc1->GetEntries()>0) eff_fdc1 = fheff_fdc1->Integral()/fheff_fdc1->GetEntries()*100;

  fheff->SetBinContent(1, eff_bdc1);
  fheff->SetBinContent(3, eff_bdc2);
  fheff->SetBinContent(5, eff_fdc1);

  //if((int)fheff_bdc1->GetEntries()%100==0){
  if(fNeve==1000 || fNeve%5000==0 ){
    cout<<endl;
    cout<<Form("BDC1 Eff = %4.2f percent",eff_bdc1)<<endl;
    cout<<Form("BDC2 Eff = %4.2f percent",eff_bdc2)<<endl;
    cout<<Form("FDC1 Eff = %4.2f percent",eff_fdc1)<<endl;
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
    fResetCount(200000), fDrawTimeInterval(10),
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
    fRootFile->cd();
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
	std::cout<<"\r"<<fNeve<<" events analyzed"<<std::flush;

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

      std::cout<<"\r"<<fNeve<<" events analyzed,"
	       <<fNeve_monitor<<" events for display"<<std::flush;
		 
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

    if(string(hist->GetName())=="pla_pid"){
	    hist->SetTitle(Form("Beam Interested/Total = %4.2f percent",(double)NumBeamInterested/(double)NumBeam*100));
	    //beamInterestedCut->Draw("same");
    }


    if (hist->GetNbinsY()>0) hist->Draw("colz");
    else                     hist->Draw();

    if(string(hist->GetName())=="pla_pid"){
	    beamInterestedCut->Draw("same");
    }




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
