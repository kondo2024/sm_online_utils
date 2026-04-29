#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc < 4){
    std::cout << "Usage: fdc1_converter input.ridf output.root tdcspectrum.root" << std::endl;
    return 1;
  }
  
  TString fname_ridf = argv[1];
  TString fname_out  = argv[2];
  TString fname_tdc  = argv[3];
  
  SAMURAIDataConverter converter;

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_FDC1");

  FDC1DataProcessor fdc1(fname_tdc.Data());
  fdc1.MakeTreeBranches(true);
  fdc1.MakeHistograms(true);
  converter.RegisterDataProcessor(&fdc1);

  converter.Run();

  return 0;
}
