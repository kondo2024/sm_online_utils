#include "SAMURAIDataConverter.hh"
#include "NINJADataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc < 3){
    std::cout << "Usage: ninja_converter input.ridf output.root" << std::endl;
    return 1;
  }

  TString fname_ridf = argv[1];
  TString fname_out  = argv[2];

  SAMURAIDataConverter converter;

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_NINJA");

  NINJADataProcessor ninja;
  ninja.MakeTreeBranches(true);
  ninja.MakeHistograms(true);
  converter.RegisterDataProcessor(&ninja);

  converter.Run();

  return 0;
}

