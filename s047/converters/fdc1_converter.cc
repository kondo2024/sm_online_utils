#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<2){
    std::cout<<"usage: fdc1_converter RunNum"
             <<std::endl;
    return 0;
  }

  Int_t nRun = atoi(argv[1]);

  TString fname_ridf(Form("ridf/sdaq04/data%04d.ridf.gz",nRun));
  TString fname_out(Form("rootfiles/dc/fdc1_%04d.root",nRun));

  SAMURAIDataConverter converter;
  //converter.SetMaxEventNumber(100);// temp for check

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_FDC1");

  const char *TDCdist = "rootfiles/dctdc/run0628_tdcSpectrum.root";// s47 11Li run

  FDC1DataProcessor fdc1(TDCdist);
  fdc1.MakeTreeBranches(true);
  fdc1.MakeHistograms(true);
  converter.RegisterDataProcessor(&fdc1);

  converter.Run();

  return 0;
}
