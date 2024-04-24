#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>
#include<unistd.h>

using namespace std;

int main(int argc, char **argv){

  if (argc<2){
    std::cout<<"usage: bdc_converter RunNum"
             <<std::endl;
    return 0;
  }

  Int_t nRun = atoi(argv[1]);

  TString fname_ridf(Form("ridf/sdaq04/data%04d.ridf.gz",nRun));
  if(access(fname_ridf.Data(),F_OK)!=0) fname_ridf.ReplaceAll(".ridf.gz",".ridf");
  TString fname_out(Form("rootfiles/dc/bdc_%04d.root",nRun));

  SAMURAIDataConverter converter;
  //converter.SetMaxEventNumber(100);// temp for check

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_BDC");

  const char *TDCdist = "rootfiles/dctdc/run0628_tdcSpectrum.root";// s047 11Li run

  BDCDataProcessor bdc(TDCdist);
  bdc.MakeTreeBranches(true);
  bdc.MakeHistograms(true);
  converter.RegisterDataProcessor(&bdc);

  converter.Run();

  return 0;
}
