#include "SAMURAIDataConverter.hh"
#include "BDCDataProcessor.hh"
#include "FDC1DataProcessor.hh"
#include "FDC2DataProcessor.hh"
#include "PDCDataProcessor.hh"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

  if (argc<2){
    std::cout<<"usage: dc_converter RunNum"
             <<std::endl;
    return 0;
  }

  Int_t nRun = atoi(argv[1]);

  TString fname_ridf(Form("ridf/sdaq04/data%04d.ridf.gz",nRun));
  TString fname_out(Form("rootfiles/dc/dc%04d.root",nRun));

  SAMURAIDataConverter converter;
  //converter.SetMaxEventNumber(100);// temp for check

  converter.SetRIDFFileName(fname_ridf.Data());
  converter.SetOutputFileName(fname_out.Data());

  converter.SetOutputTreeName("t_DCs");

  const char *TDCdist = "rootfiles/dctdc/run0309_tdcSpectrum.root";

  BDCDataProcessor bdc(TDCdist);
  bdc.MakeTreeBranches(true);
  bdc.MakeHistograms(true);
  converter.RegisterDataProcessor(&bdc);

  FDC1DataProcessor fdc1(TDCdist);
  fdc1.MakeTreeBranches(true);
  fdc1.MakeHistograms(true);
  converter.RegisterDataProcessor(&fdc1);

  FDC2DataProcessor fdc2(TDCdist);
  fdc2.MakeTreeBranches(true);
  fdc2.MakeHistograms(true);
  converter.RegisterDataProcessor(&fdc2);

  PDCDataProcessor pdc;
  pdc.MakeTreeBranches(true);
  pdc.MakeHistograms(true);
  converter.RegisterDataProcessor(&pdc);

  converter.Run();

  return 0;
}
