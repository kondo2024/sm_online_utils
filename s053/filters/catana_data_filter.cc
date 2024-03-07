#include "DataMergeFilter_CATANA.hh"
#include <iostream>

int main(int argc, char **argv){

  if (argc<4){
    std::cout<<"usage: catana_data_filter filename_sm filename_catana filename_output"
	     <<std::endl;
    return 0;
  }

  TString fname_sm(argv[1]);
  TString fname_catana(argv[2]);
  TString fname_out(argv[3]);

  DataMergeFilter_CATANA filter;
  filter.Run(fname_sm.Data(),fname_catana.Data(),
	     fname_out.Data());

  return 0;
}
