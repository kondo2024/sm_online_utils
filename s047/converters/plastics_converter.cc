#include "SAMURAIDataConverter.hh"
#include "CoinDataProcessor.hh"
#include "PlasticDataProcessor.hh"
#include "HODPlaDataProcessor.hh"
#include "NEBULADataProcessor.hh"
#include "FSDBSDDataProcessor.hh"
#include "NINJADataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<2){
    //std::cout<<"usage: plastics_converter ridffile outputfile"
    std::cout<<"usage: plastics_converter RunNum"
             <<std::endl;
    return 0;
  }

  Int_t nRun = atoi(argv[1]);

  TString fname_ridf(Form("ridf/sdaq04/data%04d.ridf.gz",nRun));
  TString fname_out(Form("rootfiles/plastics/plastics%04d.root",nRun));

  SAMURAIDataConverter converter;
  //converter.SetMaxEventNumber(100);// temp for check

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

  converter.Run();

  return 0;
}
