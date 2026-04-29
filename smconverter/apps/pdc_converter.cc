#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc < 3){
    std::cout << "Usage: pdc_converter input.ridf output.root" << std::endl;
    return 1;
  }

  TString fname_ridf = argv[1];
  TString fname_out  = argv[2];

  SAMURAIDataConverter converter;

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_PDC");

  PDCDataProcessor pdc;
  pdc.MakeTreeBranches(true);
  pdc.MakeHistograms(true);
  converter.RegisterDataProcessor(&pdc);

  converter.Run();

  return 0;
}
