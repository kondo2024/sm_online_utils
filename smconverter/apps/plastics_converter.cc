#include "SAMURAIDataConverter.hh"
#include "CoinDataProcessor.hh"
#include "PlasticDataProcessor.hh"
#include "HODPlaDataProcessor.hh"
#include "NEBULADataProcessor.hh"
#include "NINJADataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc < 3){
    std::cout << "Usage: plastics_converter input.ridf output.root" << std::endl;
    return 1;
  }

  TString fname_ridf = argv[1];
  TString fname_out  = argv[2];

  SAMURAIDataConverter converter;

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_Plastics");

  CoinDataProcessor coin;
  coin.MakeTreeBranches(true);
  converter.RegisterDataProcessor(&coin);

  PlasticDataProcessor pla;
  pla.MakeTreeBranches(true);
  pla.MakeHistograms(true);
  converter.RegisterDataProcessor(&pla);

  HODPlaDataProcessor hod;
  hod.MakeTreeBranches(true);
  hod.MakeHistograms(true);
  converter.RegisterDataProcessor(&hod);

  NEBULADataProcessor nebula;
  nebula.MakeTreeBranches(true);
  nebula.MakeHistograms(true);
  converter.RegisterDataProcessor(&nebula);

  NINJADataProcessor ninja;
  ninja.MakeTreeBranches(true);
  ninja.MakeHistograms(true);
  converter.RegisterDataProcessor(&ninja);

  converter.Run();

  return 0;
}

