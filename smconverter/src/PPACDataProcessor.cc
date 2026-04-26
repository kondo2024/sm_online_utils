#include "PPACDataProcessor.hh"
#include <iostream>
#include "TClonesArray.h"
#include "TArtPPAC.hh"
//#include "TArtFocalPlane.hh"
#include "TArtBigRIPSParameters.hh"
//____________________________________________________________________
void PPACDataProcessor::PrepareCalib()
{
  TArtBigRIPSParameters *brprm = TArtBigRIPSParameters::Instance();
  brprm->LoadParameter((char*)fdbFileName.Data());

  fCalibPPAC = new TArtCalibPPAC;
  //fCalibFocalPlane = new TArtCalibFocalPlane;
  fCalibReady = true;
}
//____________________________________________________________________
void PPACDataProcessor::PrepareTreeBranches(TTree *tree)
{
  if (!fCalibReady) PrepareCalib();

  TClonesArray *ppac_array = fCalibPPAC->GetPPACArray();
  //TClonesArray *focalplane_array = fCalibFocalPlane->GetFocalPlaneArray();
  fTree = tree;
  //fTree->Branch(fBranchName.Data(), &ppac_array);
  //fTree->Branch(TString("FocalPlane").Data(), &focalplane_array);

}
//____________________________________________________________________
void PPACDataProcessor::PrepareHistograms()
{
  if (!fCalibReady) PrepareCalib();
  hidtx1 = new TH2D("ppac_hidtx1","PPAC ID Tx1",40,0.5,40.5,100,0,80000);
  hidtx2 = new TH2D("ppac_hidtx2","PPAC ID Tx2",40,0.5,40.5,100,0,80000);
  hidty1 = new TH2D("ppac_hidty1","PPAC ID Ty1",40,0.5,40.5,100,0,80000);
  hidty2 = new TH2D("ppac_hidty2","PPAC ID Ty2",40,0.5,40.5,100,0,80000);
  hidta  = new TH2D("ppac_hidta","PPAC ID TA",40,0.5,40.5,100,0,80000);

  hidx = new TH2D("ppac_hidx","PPAC ID X",40,0.5,40.5,100,-200,200);
  hidy = new TH2D("ppac_hidy","PPAC ID Y",40,0.5,40.5,100,-200,200);

  // h2idx = new TH2D("focalplane_hidx","FocalPlane ID X",40,0.5,40.5,100,-200,200);
  // h2ida = new TH2D("focalplane_hida","FocalPlane ID A",40,0.5,40.5,100,-200,200);
  // h2idy = new TH2D("focalplane_hidy","FocalPlane ID Y",40,0.5,40.5,100,-200,200);
  // h2idb = new TH2D("focalplane_hidb","FocalPlane ID B",40,0.5,40.5,100,-200,200);

  fHistArray.push_back(hidtx1);
  fHistArray.push_back(hidtx2);
  fHistArray.push_back(hidty1);
  fHistArray.push_back(hidty2);
  fHistArray.push_back(hidta);
  fHistArray.push_back(hidx);
  fHistArray.push_back(hidy);
  // fHistArray.push_back(h2idx);
  // fHistArray.push_back(h2ida);
  // fHistArray.push_back(h2idy);
  // fHistArray.push_back(h2idb);

  // under construction
}
//____________________________________________________________________
void PPACDataProcessor::ClearData()
{
  fCalibPPAC->ClearData();
  // fCalibFocalPlane->ClearData();
}
//____________________________________________________________________
void PPACDataProcessor::ReconstructData()
{
  fCalibPPAC->ReconstructData();
  // fCalibFocalPlane->ReconstructData();
}
//____________________________________________________________________
void PPACDataProcessor::FillHistograms()
{
  TClonesArray *ppac_array = fCalibPPAC->GetPPACArray();
  int n=ppac_array->GetEntries();
  for (int i=0;i<n;++i){
    TArtPPAC *ppac = (TArtPPAC*)ppac_array->At(i);
    Int_t ID = ppac->GetID();

    hidtx1->Fill(ppac->GetID(),ppac->GetTX1Raw());
    hidtx2->Fill(ppac->GetID(),ppac->GetTX2Raw());
    hidty1->Fill(ppac->GetID(),ppac->GetTY1Raw());
    hidty2->Fill(ppac->GetID(),ppac->GetTY2Raw());
    hidta->Fill(ppac->GetID(),ppac->GetTARaw());

    hidx->Fill(ppac->GetID(),ppac->GetX());
    hidy->Fill(ppac->GetID(),ppac->GetY());

    // if (ID==10){// F5PPAC-1A (ID=10-13 F5PPACs)
    //   hdtplappac->Fill(dt_f5pla,ppac->GetX());
    // }
  }

  // TClonesArray *focalplane_array = fCalibFocalPlane->GetFocalPlaneArray();
  // n=focalplane_array->GetEntries();
  // TVectorD *vec;
  // for (int i=0;i<n;++i){
  //   TArtFocalPlane *focalplane = (TArtFocalPlane*)focalplane_array->At(i);
  //   Int_t ID = focalplane->GetID();
  //   vec=focalplane->GetOptVector();
  //   //F7X = (*vec)(0); F7A = (*vec)(1); F7Y = (*vec)(2); F7B = (*vec)(3);
  //   h2idx->Fill(ID, (*vec)(0));
  //   h2ida->Fill(ID, (*vec)(1));
  //   h2idy->Fill(ID, (*vec)(2));
  //   h2idb->Fill(ID, (*vec)(3));

  //   // if (ID==3) {
  //   //   vec=focalplane->GetOptVector();
  //   //   //F3X = (*vec)(0); F3A = (*vec)(1); F3Y = (*vec)(2); F3B = (*vec)(3);
  //   //   h2idx->Fill(3, (*vec)(0));
  //   //   h2ida->Fill(3, (*vec)(1));
  //   //   h2idy->Fill(3, (*vec)(2));
  //   //   h2idb->Fill(3, (*vec)(3));
  //   // } else if (ID==5) {
  //   //   vec=focalplane->GetOptVector();
  //   //   //F5X = (*vec)(0); F5A = (*vec)(1); F5Y = (*vec)(2); F5B = (*vec)(3);
  //   //   h2idx->Fill(5, (*vec)(0));
  //   //   h2ida->Fill(5, (*vec)(1));
  //   //   h2idy->Fill(5, (*vec)(2));
  //   //   h2idb->Fill(5, (*vec)(3));
  //   // } else if (ID==7) {
  //   //   vec=focalplane->GetOptVector();
  //   //   //F7X = (*vec)(0); F7A = (*vec)(1); F7Y = (*vec)(2); F7B = (*vec)(3);
  //   //   h2idx->Fill(7, (*vec)(0));
  //   //   h2ida->Fill(7, (*vec)(1));
  //   //   h2idy->Fill(7, (*vec)(2));
  //   //   h2idb->Fill(7, (*vec)(3));
  //   // }
  // }
}
//____________________________________________________________________
