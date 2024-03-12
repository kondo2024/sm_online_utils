#include "SAMURAIDataConverter.hh"
#include "CoinDataProcessor.hh"
#include "PlasticDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<3){
    std::cout<<"usage: plastics_converter ridffile outputfile"
             <<std::endl;
    return 0;
  }

  TString fname_ridf(argv[1]);
  TString fname_out(argv[2]);

  SAMURAIDataConverter converter;
  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_Plastic");

  CoinDataProcessor coin;
  coin.MakeTreeBranches(true);
  converter.RegisterDataProcessor(&coin);

  PlasticDataProcessor pla;
  pla.MakeTreeBranches(true);
  pla.MakeHistograms(true);
  converter.RegisterDataProcessor(&pla);

  converter.Run();

  return 0;
}
