void pfad_run(){
  TString fname_sm("root/hd01_smts_run0136.root");
  TString fname_det("root/pfad_230MeV_run07_ana.root");
  TString fname_out("temp.root");

  //DataMergeFilter filter;
  DataMergeFilter_PFAD filter;
  // set parameters
  filter.SetInputSMTreeName("t_smts");
  filter.SetInputDetTreeName("PhysicsTree");
  filter.SetTS2us_sm(0.01);
  filter.SetTS2us_det(0.04);
  filter.SetBranchName_smTS("smts");
  filter.SetBranchName_detTS("LTS");
  filter.SetTSMatchWindow(10);// us
  
  //
  bool IsOKFile;
  IsOKFile = filter.OpenInputFiles(fname_sm.Data(), fname_det.Data());
  if (!IsOKFile) return 1;

  IsOKFile = filter.OpenOutputFile(fname_out.Data());
  if (!IsOKFile) return 1;
  
  filter.FindRunStart();
  filter.FindRunEnd(850000);// temp

  filter.TSCombinations("start");
  filter.CorrectTSPeriod();

//  filter.TSCombinations("end");
//  filter.FindTSOffset("end");

  filter.Filter();
  filter.MakePlots();

}
