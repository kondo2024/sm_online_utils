#include "DataMergeFilter_PFAD.hh"
#include <iostream>
#include "TCanvas.h"

int main(int argc, char **argv){

  if (argc<4){
    std::cout<<"usage: pfad_data_filter filename_sm filename_pfad filename_output"
	     <<std::endl;
    return 0;
  }

  TString fname_sm(argv[1]);
  TString fname_pfad(argv[2]);
  TString fname_out(argv[3]);

  //DataMergeFilter filter;
  DataMergeFilter_PFAD filter;
  filter.Run(fname_sm.Data(),fname_pfad.Data(),
	     fname_out.Data());

  return 0;
}
