#include "/home/s057/exp/exp2505_s057/anaroot/users/hayasaki/sm_online_utils/onlineconverter/OnlineConverter.hh"
void online(){
  //gSystem->Load("sm_online_utils/install/lib/libsmconverter.so");
  gROOT->ProcessLine(".L sm_online_utils/onlineconverter/OnlineMonitor.cc+g");
  OnlineMonitor *mon = new OnlineMonitor("ridf/data1292.ridf");
  mon->Run();
}


