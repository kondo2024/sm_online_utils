void catana_run(){
  TString fname_sm("root/hd01_smts_run0136.root");
  TString fname_det("ridf/hd01/v1725/run_139/FILTERED/SDataF_run_139.root");
  TString fname_out("temp.root");

  DataMergeFilter filter;
  // set parameters
  filter.SetInputSMTreeName("t_smts");
  filter.SetInputDetTreeName("Data_F");
  filter.SetTS2us_sm(0.01);
  filter.SetTS2us_det(1.e-06);
  filter.SetBranchName_smTS("smts");
  filter.SetBranchName_detTS("Timestamp");
  filter.SetTSMatchWindow(10);// us
  filter.SetNsrch_sm(1000);
  filter.SetNsrch_det(1000);
  //
  bool IsOKFile;
  IsOKFile = filter.OpenInputFiles(fname_sm.Data(), fname_det.Data());
  if (!IsOKFile) return 1;

  IsOKFile = filter.OpenOutputFile(fname_out.Data());
  if (!IsOKFile) return 1;
  
  filter.TSCombinations();
  //filter.CorrectTSPeriod();
  filter.FindTSOffset();
  filter.Filter();
  filter.MakePlots();
}
