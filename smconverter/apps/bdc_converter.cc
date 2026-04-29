#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc < 4){
    std::cout << "Usage: bdc_converter input.ridf output.root tdcspectrum.root" << std::endl;
    return 1;
  }

  TString fname_ridf = argv[1];
  TString fname_out  = argv[2];
  TString fname_tdc  = argv[3];

  SAMURAIDataConverter converter;
  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_BDC");

  BDCDataProcessor bdc(fname_tdc.Data());
  bdc.MakeTreeBranches(true);
  bdc.MakeHistograms(true);
  converter.RegisterDataProcessor(&bdc);

  converter.Run();

  return 0;
}
