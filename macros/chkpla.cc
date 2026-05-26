void chkpla(Int_t nRun=1070){

  TArtBigRIPSParameters *prm = TArtBigRIPSParameters::Instance();
  prm->LoadParameter("db/SAMURAIPlastic.xml");

  TArtEventStore *estore = new TArtEventStore();
  if (nRun==0) estore->Open(0);
  else         estore->Open(Form("ridf/run%04d.ridf",nRun));

  TArtCalibPlastic *calib_pla = new TArtCalibPlastic();
  TClonesArray* pla_array = (TClonesArray*)calib_pla->GetPlasticArray();

  TFile *fout = new TFile(Form("macros/chkpla/chkpla%04d.root",nRun),"RECREATE");
  TTree *tree = new TTree("tree","tree");
  tree->Branch("pla",&pla_array);
  
  TH1* hplaidtl = new TH2D("hplaidtl","Pla ID TLRaw" ,10,0.5,10.5, 100,0,60000);
  TH1* hplaidtr = new TH2D("hplaidtr","Pla ID TRRaw" ,10,0.5,10.5, 100,0,60000);
  TH1* hplaidql = new TH2D("hplaidql","Pla ID QLRaw" ,10,0.5,10.5, 100,0,4000);
  TH1* hplaidqr = new TH2D("hplaidqr","Pla ID QRRaw" ,10,0.5,10.5, 100,0,4000);

  TH1* fhsbt1ql = new TH1D("SBT1QL","SBT1 QL;QLraw;Counts",200,0,2000);
  TH1* fhsbt1qr = new TH1D("SBT1QR","SBT1 QR;QRraw;Counts",200,0,2000);
  TH1* fhsbttdiff21 = new TH1D("SBTTaveDiff21","SBT Tave2-Tave1;Tave2-Tave1;Counts",
			     200,-5,5);

  TH1* htof713q13 = new TH2D("htof713q13","T_F13-T_F7 Q13",400,-380,-330,200,0,4000);
  TH1* htof713 = new TH1D("htof713","T_F13-T_F7",400,-380,-330);
  
  int neve = 0;
  while(estore->GetNextEvent() && neve < 10000){

    calib_pla->ClearData();
    calib_pla->ReconstructData();

    TArtPlastic *f3pla = nullptr;
    TArtPlastic *f7pla = nullptr;
    TArtPlastic *f13pla1 = nullptr;
    TArtPlastic *f13pla2 = nullptr;
    
    Int_t npla = pla_array->GetEntries();
    for(int i=0;i<npla;++i){
      TArtPlastic *pla = (TArtPlastic*)pla_array->At(i);
      Int_t id = pla->GetID();

      if      (id==1) f3pla = pla;
      else if (id==3) f7pla = pla;
      else if (id==4) f13pla1 = pla;
      else if (id==5) f13pla2 = pla;
      
      Double_t TLRaw = pla->GetTLRaw();
      Double_t TRRaw = pla->GetTRRaw();
      Double_t QLRaw = pla->GetQLRaw();
      Double_t QRRaw = pla->GetQRRaw();

      hplaidtl->Fill(id,TLRaw);
      hplaidtr->Fill(id,TRRaw);
      hplaidql->Fill(id,QLRaw);
      hplaidqr->Fill(id,QRRaw);
    }

    if (f13pla1){
      fhsbt1ql->Fill(f13pla1->GetQLRaw());
      fhsbt1qr->Fill(f13pla1->GetQRRaw());
      
      if (f13pla2){
	double tave1 = f13pla1->GetTime();
	double tave2 = f13pla2->GetTime();
	fhsbttdiff21->Fill(tave2-tave1);
      }
    }

    if (f7pla && f13pla1 && f13pla2){
      double tof713 = 0.5*(f13pla1->GetTime() + f13pla2->GetTime()) - f7pla->GetTime();
      double q13 = f13pla2->GetQAveRaw();

      htof713q13->Fill(tof713,q13);
      htof713->Fill(tof713);
      
    }
    tree->Fill();

    estore->ClearData();
    neve++;
  }

  fout->Write();
  fout->Close();

  std::cout<<"output file: "<<fout->GetName()<<std::endl;
  
}
