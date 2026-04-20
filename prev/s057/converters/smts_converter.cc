#include "SAMURAIDataConverter.hh"
#include "SAMURAITSDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<2){
    //std::cout<<"usage: nebula_converter ridffile outputfile"
    std::cout<<"usage: smts_converter RunNum"
             <<std::endl;
    return 0;
  }

  Int_t nRun = atoi(argv[1]);

  TString fname_ridf(Form("ridf/sdaq04/data%04d.ridf.gz",nRun));
  TString fname_out(Form("rootfiles/ts/smts%04d.root",nRun));

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
