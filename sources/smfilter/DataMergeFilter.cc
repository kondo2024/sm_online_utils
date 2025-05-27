#include "DataMergeFilter.hh"

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


//_________________________________________________________
DataMergeFilter::DataMergeFilter()
  : fFile_sm(0), fFile_det(0), fFile_out(0),
    fIsFirstOpen_sm(true),fIsFirstOpen_det(true),fIsFirstOpen_out(true),
    fTreeName_out(""),
    fTree_sm(0), fTree_det(0), fTree_out(0),
    fNsrch_sm(2000), fNsrch_det(2000), fNbin(1000),
    fTS2us_sm(-1), fTS2us_det(-1),
    fTSoff_sta_us(0),fTSoff_end_us(0),
    fMatchWindow_us(-1),
    ilog(0)
{
}
//_________________________________________________________
DataMergeFilter::~DataMergeFilter(){

  if (fFile_sm != 0)
    if (fFile_sm->IsOpen())  fFile_sm->Close();
  
  if (fFile_det != 0)
    if (fFile_det->IsOpen()) fFile_det->Close();
  
  if (fFile_out !=0)
    if (fFile_out->IsOpen()) fFile_out->Close();
  
}
//_________________________________________________________
bool DataMergeFilter::OpenInputSMFile(){
  //std::cout<<"Open Input SM Files ...  "<<std::flush;

  fFile_sm = new TFile(fFileName_sm.Data());
  if (!fFile_sm->IsOpen()) {
    std::cout<<"Cannot open file:"<<fFileName_sm.Data()<<std::endl;
    return false;
  }

  fTree_sm = (TTree*)fFile_sm->Get(fTreeName_sm.Data());
  if (fTree_sm==0){
    std::cout<<"Cannot find tree:"<<fTreeName_sm.Data()<<std::endl;
    return false;
  }

  if (fIsFirstOpen_sm){
    AddLog(fFile_sm->GetName());
    std::cout<<fFile_sm->GetName()<<"  Nentries = "<<fTree_sm->GetEntries()<<std::endl;

    fIsFirstOpen_sm = false;
  }
  return fFile_sm->IsOpen();
}
//_________________________________________________________
bool DataMergeFilter::OpenInputDetFile(){
  //std::cout<<"Open Input Det Files ...  "<<std::flush;

  fFile_det = new TFile(fFileName_det.Data());
  if (!fFile_det->IsOpen()) {
    std::cout<<"Cannot open file:"<<fFileName_det.Data()<<std::endl;
    return false;
  }
  fTree_det = (TTree*)fFile_det->Get(fTreeName_det.Data());
  if (fTree_det==0){
    std::cout<<"Cannot find tree:"<<fTreeName_det.Data()<<std::endl;
    return false;
  }

  if (fIsFirstOpen_det){
    AddLog(fFile_det->GetName());
    std::cout<<fFile_det->GetName()<<"  Nentries = "<<fTree_det->GetEntries()<<std::endl;

    fNsta_det = 0;
    fNend_det = fTree_det->GetEntries();

    fIsFirstOpen_det = false;
  }
  return fFile_det->IsOpen();
}
//_________________________________________________________
bool DataMergeFilter::OpenOutputFile(){
  //std::cout<<"Open Output File ...  "<<std::flush;
 
  if (fIsFirstOpen_det){

//    // check if file exists
//    std::ifstream ifs(fFileName_out.Data());
//    if (!ifs.fail()) {
//      std::cout<<"Output file already exitsts"<<std::endl;
//      return false;
//    }

    fFile_out  = new TFile(fFileName_out.Data(),"new");
    // log
    fLogDir = fFile_out->mkdir("logs");
    TDatime dt;
    TString str("created ");
    str += dt.AsSQLString();
    AddLog(str.Data());
    
    fIsFirstOpen_out = false;

  }else{
    fFile_out  = new TFile(fFileName_out.Data(),"update");
    fLogDir = (TDirectory*)fFile_out->Get("logs");
  }

  return fFile_out->IsOpen();
}
//_________________________________________________________
void DataMergeFilter::FindRunStart(){
  /*
    During the PFAD HIMAC exp, LTS is zero if RIBF DAQ stops.
    Using this, find run start in the PFAD root file.
   */
  std::cout<<"Find Run Start ...  "<<std::flush;

  OpenOutputFile();
  OpenInputDetFile();
  fFile_out->cd();
  
  Long64_t Nentries = fTree_det->GetEntries();
  Int_t ihist=0;
  TString hname(Form("hfindrunsta%d",ihist));
  fTree_det->Draw(Form("Entry$>>%s(%d,0,%e)",hname.Data(),fNbin,(Double_t)Nentries),
		  Form("%s==0",fBrName_detTS.Data()) );
  gPad->Update();
    
  TH1* hist = (TH1*)gDirectory->Get(hname.Data());
  hist->Write();
  Double_t ymax = hist->GetMaximum();
  Double_t ymin = hist->GetMinimum();
  Double_t thre = 0.5*(ymin+ymax);

  Bool_t IsExceedThre = false;
  Int_t nx = hist->GetNbinsX();
  Double_t xsta = 0;
  for (Int_t ix=1;ix<nx+1;ix++){
    Double_t cont = hist->GetBinContent(ix);
    if (cont>thre) IsExceedThre = true;
    if (cont<thre && IsExceedThre) {// run start
      xsta = hist->GetBinCenter(ix);
      break;
    }
  }
  
  while( hist->GetXaxis()->GetBinWidth(1) > 0.5*fNsrch_det){
    Double_t x1 = hist->GetXaxis()->GetXmin();
    Double_t x2 = hist->GetXaxis()->GetXmax();
    Double_t xrange = x2 - x1;

    x1 = xsta-0.5*xrange;
    x1 = TMath::Max(x1,0.);
    x2 = xsta+0.5*xrange;
    x2 = TMath::Min(x2,(Double_t)Nentries);
    ihist++;
    hname = Form("hfindrunsta%d",ihist);

    fTree_det->Draw(Form("Entry$>>%s(%d,%e,%e)",hname.Data(),fNbin,x1,x2),
		    Form("%s==0",fBrName_detTS.Data()) );
    gPad->Update();
    
    hist = (TH1*)gDirectory->Get(hname.Data());
    hist->Write();

    ymax = hist->GetMaximum();
    ymin = hist->GetMinimum();
    thre = 0.5*(ymin+ymax);

    IsExceedThre = false;
    nx = hist->GetNbinsX();
    xsta = 0;
    for (Int_t ix=1;ix<nx+1;ix++){
      Double_t cont = hist->GetBinContent(ix);
      if (cont>thre) IsExceedThre = true;
      if (cont<thre && IsExceedThre) {// run start
	xsta = hist->GetBinCenter(ix);
	break;
      }
    }
    gPad->Update();
  }

  fNsta_det = xsta;

  fFile_det->Close();
  fFile_out->Close();
  std::cout<<"Nstart = "<<fNsta_det<<std::endl;
}
//_________________________________________________________
void DataMergeFilter::FindRunEnd(Long64_t nmax){
  std::cout<<"Find Run End ...  "<<std::flush;
  OpenOutputFile();
  OpenInputDetFile();

  if (nmax<0) nmax = fTree_det->GetEntries();

  fFile_out->cd();
  
  Long64_t Nentries = fTree_det->GetEntries();
  Int_t ihist=0;
  TString hname(Form("hfindrunend%d",ihist));
  fTree_det->Draw(Form("Entry$>>%s(%d,0,%e)",hname.Data(),fNbin,(Double_t)nmax),
		  Form("Entry$<%e&&%s==0",(Double_t)nmax,fBrName_detTS.Data()));
  gPad->Update();

  TH1* hist = (TH1*)gDirectory->Get(hname.Data());
  hist->Write();
  Double_t ymax = hist->GetMaximum();
  Double_t ymin = hist->GetMinimum();
  Double_t thre = 0.5*(ymin+ymax);

  Bool_t IsExceedThre = false;
  Int_t nx = hist->GetNbinsX();
  Double_t xsto = Nentries;
  for (Int_t ix=nx;ix>1;ix--){
    Double_t cont = hist->GetBinContent(ix);
    if (cont>thre) IsExceedThre = true;
    if (cont<thre && IsExceedThre) {// run end
      xsto = hist->GetBinCenter(ix);
      break;
    }
  }

  while( hist->GetXaxis()->GetBinWidth(1) > 0.5*fNsrch_det){
    Double_t x1 = hist->GetXaxis()->GetXmin();
    Double_t x2 = hist->GetXaxis()->GetXmax();
    Double_t xrange = x2 - x1;
    
    x1 = xsto-0.2*xrange;
    x1 = TMath::Max(x1,0.);
    x2 = xsto+0.2*xrange;
    x2 = TMath::Min(x2,(Double_t)nmax);
    ihist++;
    hname = Form("hfindrunend%d",ihist);

    fTree_det->Draw(Form("Entry$>>%s(%d,%e,%e)",hname.Data(),fNbin,x1,x2),
		    Form("Entry$<%e&&%s==0",(Double_t)nmax,fBrName_detTS.Data()));
    gPad->Update();

    hist = (TH1*)gDirectory->Get(hname.Data());
    hist->Write();
    ymax = hist->GetMaximum();
    ymin = hist->GetMinimum();
    thre = 0.5*(ymin+ymax);

    IsExceedThre = false;
    nx = hist->GetNbinsX();
    xsto = nmax;
    for (Int_t ix=1;ix<nx+1;ix++){
      Double_t cont = hist->GetBinContent(ix);
      if (cont>thre) IsExceedThre = true;
      if (cont<thre && IsExceedThre) {// run end
	xsto = hist->GetBinCenter(ix);
	break;
      }
    }
    gPad->Update();
  }
  
  fNend_det = 1.1*xsto;// 10% margin
  if ( fNend_det > fTree_det->GetEntries() ) fNend_det  = fTree_det->GetEntries();
  
  fFile_det->Close();
  fFile_out->Close();
  std::cout<<"Nend = "<<fNend_det<<std::endl;
}
//_________________________________________________________
void DataMergeFilter::TSCombinations(TString mode){
  mode.ToLower();
  std::cout<<"Taking TS Combinations from run "<<mode.Data()<<" ...  "<<std::flush;

  OpenOutputFile();
  OpenInputSMFile();
  OpenInputDetFile();
  
  TString log = Form("Nsrch_sm = %d   Nsrch_det = %d",fNsrch_sm,fNsrch_det); 
  AddLog(log.Data());
  std::cout<<log.Data()<<std::endl;
  
  Long64_t ista_sm;
  Long64_t ista_det;
  Int_t    incre;
  TTree*   t_comb=0;
  
  fFile_out->cd();
  if (mode == "start"){
    ista_sm  = 0;
    ista_det = fNsta_det;
    incre = 1;
    fTree_comb0 = new TTree("t_comb0","Tree for combinations of run start");
    t_comb = fTree_comb0;
  }else if (mode == "end"){
    ista_sm  = fTree_sm->GetEntries();
    ista_det = fNend_det-1;
    incre = -1;
    fTree_comb1 = new TTree("t_comb1","Tree for combinations of run end");
    t_comb = fTree_comb1;
  }else{
    std::cout<<"  unknown mode="<<mode.Data()<<std::endl;
    return;
  }

  ULong64_t TS_sm;
  fTree_sm->SetBranchAddress(fBrName_smTS.Data(),&TS_sm);

  ULong64_t TS_det;
  fTree_det->SetBranchStatus("*",0);// de-activate all the branches
  fTree_det->SetBranchStatus(fBrName_detTS.Data(),1);// and acitivate limited ones
  fTree_det->SetBranchAddress(fBrName_detTS.Data(),&TS_det);

  fFile_out->cd();
  Double_t dTS=0;
  Double_t TS_sm_us=0;
  Double_t TS_det_us=0;
  t_comb->Branch("dTS",&dTS);
  t_comb->Branch("TS_sm_us",&TS_sm_us);
  t_comb->Branch("TS_det_us",&TS_det_us);

  Long64_t i_entry_sm = ista_sm;
  
  ULong64_t i_count=0;
  for (Int_t i_sm=0;i_sm<fNsrch_sm;i_sm++){
    fTree_sm->GetEntry(i_entry_sm);

    Long64_t i_det = ista_det;
    Int_t i_det_comb=0;
    while(i_det_comb < fNsrch_det){

      if (i_det>=fNend_det) break;
      
      if (i_count%1000==0)
	std::cout<<"\r   Event: "<<i_count<<" / "<<fNsrch_sm*fNsrch_det<<std::flush;

      fTree_det->GetEntry(i_det);

      i_det = i_det + incre;

      if (TS_det==0) continue;
      
      TS_sm_us  = fTS2us_sm  * TS_sm;// us
      TS_det_us = fTS2us_det * TS_det;
      dTS = TS_det_us - TS_sm_us;

      t_comb->Fill();
      i_det_comb++;
      i_count++;
    }
   
    i_entry_sm = i_entry_sm + incre;
  }//for (Int_t i_smts=0;i_smts<Nsrch_smts;i_smts++){

  std::cout<<"\r   Event: "<<i_count<<" / "<<fNsrch_sm*fNsrch_det<<std::endl;

  t_comb->Write();

  fFile_sm->Close();
  fFile_det->Close();
  fFile_out->Close();
  return;
}
//_________________________________________________________
void DataMergeFilter::CorrectTSPeriod(){
  std::cout<<"Correct TS Period ...  "<<std::endl;

  OpenOutputFile();
  fTree_comb0 = (TTree*)fFile_out->Get("t_comb0");

  Int_t ihist=0;
  TString hname("hdts");
  fTree_comb0->Draw(Form("TS_det_us - TS_sm_us>>%s",hname.Data()));
  TH1* hist = (TH1*)gDirectory->Get(hname.Data());
  TH1* hist_cpy = (TH1*)hist->Clone(Form("%s%d",hist->GetName(),ihist));
  hist_cpy->Write();
  ihist++;
  Double_t xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
  Double_t dx = hist->GetXaxis()->GetBinWidth(1);
  
  hist = ChangePlotRange(fTree_comb0, hname, 1.0, xpeak, dx, true);
  hist->Clone(Form("%s%d",hist->GetName(),ihist))->Write();
  ihist++;
  xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
  dx = hist->GetXaxis()->GetBinWidth(1);

  Double_t TS_det_us0 = xpeak;
  
  Double_t eps = 1.e-14;
  Double_t dig = 1.e-2;
  Double_t corr = 1.;
  Double_t fac = 1.;
  while( dig > eps ){

    hist = ChangePlotRange(fTree_comb0, hname, corr, xpeak, dx, false);
    hist->Clone(Form("%s%d",hist->GetName(),ihist))->Write();
    ihist++;
    Double_t ymax = hist->GetMaximum();
    //std::cout<<ymax<<std::endl;
    
    Double_t peak = xpeak;
    for(Int_t i=-9;i<10;++i){
      fac = corr + dig*i;

      std::cout<<"\r   factor="<<std::fixed<<std::setprecision(14)<<fac<<std::flush;
      Double_t xshift = TS_det_us0*dig*i;
      hist = DrawdTS(fTree_comb0, hname, fac, peak+xshift, dx);
      gPad->Update();
      Double_t y = hist->GetMaximum();

      if (y>ymax){
	corr = fac;
	ymax = y;
	xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
	hist->Clone(Form("%s%d",hist->GetName(),ihist))->Write();
	ihist++;
	dx = hist->GetXaxis()->GetBinWidth(1);
      }
    }//for(Int_t i=-9;i<10;++i){

    dx *= 0.1;
    if (dx<TMath::Max(fTS2us_det,fTS2us_sm)) dx = TMath::Max(fTS2us_det,fTS2us_sm);
    dig *= 0.1;
    
  }

  std::cout<<"\r   factor="<<std::fixed<<std::setprecision(14)<<corr<<std::endl;
  SetTS2us_det(corr*fTS2us_det);
  fTSoff_sta_us = xpeak;

  TString log = Form("TS2us_det = %17.15lf",fTS2us_det);
  AddLog(log.Data());
  std::cout<<log.Data()<<std::flush;

  log = Form("TSoff(start) = %17.3lf us",fTSoff_sta_us);  
  AddLog(log.Data());
  std::cout<<"  "<<log.Data()<<std::endl;

  fFile_out->Close();
}
//_________________________________________________________
void DataMergeFilter::FindTSOffset(TString mode){
  mode.ToLower();
  std::cout<<"Finding Offset at run "<<mode.Data()<<" ..."<<std::flush;

  OpenOutputFile();
  //CloseOpenOutputFile();
  
  TTree *tree = 0;
  if      (mode == "start") tree = (TTree*)fFile_out->Get("t_comb0");
  else if (mode == "end")   tree = (TTree*)fFile_out->Get("t_comb1");
  else{
    std::cout<<"  unknown mode="<<mode.Data()<<std::endl;
    return;
  }

  Double_t dx_min = TMath::Max(fTS2us_det,fTS2us_sm);

  Int_t ihist=0;
  TString hname(Form("%s_dts%d",tree->GetName(),ihist));
  tree->Draw(Form("dTS>>%s",hname.Data()));
  gPad->Update();
  TH1* hist = (TH1*)gDirectory->Get(hname.Data());
  hist->Write();
  ihist++;

  Bool_t IsLast=false;
  while(!IsLast){

    Double_t dx = hist->GetXaxis()->GetBinWidth(1);
    dx *= 0.1;
    if (dx<dx_min){
      dx = dx_min;
      IsLast = true;
    }

    Double_t xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
    Double_t x1 = xpeak-0.5*dx*fNbin;
    Double_t x2 = xpeak+0.5*dx*fNbin;
    
    hname = Form("%s_dts%d",tree->GetName(),ihist);
    TString draw_str = Form("dTS>>%s(%i,%17.3lf,%17.3lf)",hname.Data(),fNbin,x1,x2);
    tree->Draw(draw_str.Data());
    gPad->Update();
    hist = (TH1*)gDirectory->Get(hname.Data());
    hist->Write();
    ihist++;

  }//while(!IsLast){


  TString log;
  if      (mode == "start") {
    fTSoff_sta_us = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
    log = Form("TS offset(start) =%17.3lf us",fTSoff_sta_us);
  } else if (mode == "end") {
    fTSoff_end_us = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
    log = Form("TS offset(end) =%17.3lf us",fTSoff_end_us);
  }
  AddLog(log.Data());
  std::cout<<log.Data()<<std::endl;
  fFile_out->Close();
}
//_________________________________________________________
void DataMergeFilter::Filter(){
  std::cout<<"Filtering ...  "<<std::endl;
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
  fTree_det->SetBranchAddress(fBrName_detTS.Data(),&TS_det);
  // ***************************************
  //Double_t Delta=0;
  // ...
  //fTree_det->SetBranchAddress("Delta",&Delta);
  // ...
  // ***************************************
  
  //CloseOpenOutputFile();
  
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
  fTree_out->Branch(fBrName_detTS.Data(),&TS_det);
  // ***************************************
  //fTree_out->Branch("Delta",&Delta);
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
    //Delta = -9999;

    // ***************************************

    while( i_det < fNend_det ){
      fTree_det->GetEntry(i_det);

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
//_________________________________________________________
void DataMergeFilter::MakePlots(){
  std::cout<<"Make plots for check...  "<<std::flush;

  OpenOutputFile();

  fTree_out = (TTree*)fFile_out->Get(fTreeName_out.Data());
  fTree_out->AddFriend(fTreeName_sm.Data(),fFileName_sm.Data());

  fFile_out->cd();
  fTree_out->Draw(Form("%17.15f*%s:%17.15f*smts>>hchk1",
		       fTS2us_det,fBrName_detTS.Data(),fTS2us_sm),"IsTSMatched","colz");
  fFile_out->Get("hchk1")->Write();

  
  TString str = Form("%17.15f*%s-%17.15f*smts-%17.3lf:Entry$>>hchk2",
		     fTS2us_det,fBrName_detTS.Data(),fTS2us_sm,fTSoff_sta_us);
  fTree_out->Draw(str.Data(),"IsTSMatched","colz");  
  fFile_out->Get("hchk2")->Write();

  fFile_out->Close();
  std::cout<<"Done"<<std::endl;
}
//_________________________________________________________
void DataMergeFilter::AddLog(const char* log){
  if (!fFile_out->IsOpen()) 
    std::cout<<"Open Output file before call AddLog"<<std::endl;

  TDirectory *current_dir = gDirectory;
  ++ilog;
  TNamed *l = new TNamed(Form("log%d",ilog),  log); 
  fLogDir->Add(l);
  fLogDir->cd();
  l->Write();
  current_dir->cd();
}
//_________________________________________________________
bool DataMergeFilter::SetOutputFileName(const char* fname){
  fFileName_out = fname;
  // check if file exists
  std::ifstream ifs(fFileName_out.Data());
  if (!ifs.fail()) {
    std::cout<<"Output file already exitsts"<<std::endl;
    return false;
  }
  return true;
}
//_________________________________________________________
//_________________________________________________________
//_________________________________________________________
// protected functions
//_________________________________________________________
TH1* DataMergeFilter::ChangePlotRange(TTree *t, TString hname, Double_t fac,
				      Double_t &xpeak, Double_t &dx, Bool_t TF){

  //  std::cout<<"Change Plot Range..."<<std::endl;
  TH1* hist = DrawdTS(t, hname, fac, xpeak, dx);
  gPad->Update();
  
  while( IsPeakABin(hist) != TF){
    Bool_t IsMinWidth = ZoomHist(t, hist, fac);
    xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
    dx = hist->GetXaxis()->GetBinWidth(1);
    gPad->Update();
    if (IsMinWidth) break;
  }
  return hist;
}
//_________________________________________________________
Bool_t DataMergeFilter::ZoomHist(TTree* t, TH1 *hist, Double_t fac){

  Bool_t retval = false;
  Double_t min_dx = TMath::Max(fTS2us_det,fTS2us_sm);
  
  Int_t nx = hist->GetNbinsX();
  Double_t xpeak = hist->GetXaxis()->GetBinCenter(hist->GetMaximumBin());
  Double_t x1 = hist->GetXaxis()->GetXmin();
  Double_t x2 = hist->GetXaxis()->GetXmax();
  Double_t dx = (x2-x1)/nx * 0.3;
  if (dx<min_dx) {
    dx = min_dx;
    retval = true;
  }

  x1 = xpeak-0.5*dx*nx;
  x2 = xpeak+0.5*dx*nx;

  hist = DrawdTS(t, hist->GetName(), fac, xpeak, dx);
  return retval;
}
//_________________________________________________________
TH1* DataMergeFilter::DrawdTS(TTree* t, TString hname, Double_t fac, Double_t xpeak, Double_t dx){
  Double_t x1 = xpeak-0.5*dx*fNbin;
  Double_t x2 = xpeak+0.5*dx*fNbin;
  t->Draw(Form("%17.15lf*TS_det_us-TS_sm_us>>%s(%i,%17.3lf,%17.3lf)",
	       fac,hname.Data(),fNbin,x1,x2));
  TH1* h = (TH1*)gDirectory->Get(hname.Data());
  return h;
}
//_________________________________________________________
Bool_t DataMergeFilter::IsPeakABin(TH1 *hist){
  Double_t ymax = hist->GetMaximum();
  Double_t y1 = hist->GetBinContent(1);

  Int_t  i_peak = hist->GetMaximumBin();
  Double_t y_l = hist->GetBinContent(i_peak-1);
  Double_t y_r = hist->GetBinContent(i_peak+1);

  Bool_t TF = ( ymax-y_l > TMath::Abs(y_l-y1) ) && ( ymax-y_r > TMath::Abs(y_r-y1) );
  return TF;
}
//_________________________________________________________
