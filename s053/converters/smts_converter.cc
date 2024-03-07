#include "SAMURAIDataConverter.hh"
#include "SAMURAITSDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<3){
    std::cout<<"usage: nebula_converter ridffile outputfile"
             <<std::endl;
    return 0;
  }

  TString fname_ridf(argv[1]);
  TString fname_out(argv[2]);

  SAMURAIDataConverter converter;
  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());
  converter.SetOutputTreeName("t_TS");

  SAMURAITSDataProcessor smts;
  smts.MakeTreeBranches(true);

  converter.RegisterDataProcessor(&smts);
  converter.Run();

  return 0;
}
