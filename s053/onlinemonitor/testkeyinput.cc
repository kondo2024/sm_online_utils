#include <iostream>

#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1.h"


int getcommand(){

  std::cout<<"\r puase, input command >"<<std::flush;

  system("stty raw"); 
  char input = getchar(); 
  system("stty cooked"); 
  if(input =='q'){
    return -1;
  } else{
    return 0;
  }

  return 0;
}


void testkeyinput(){

  TCanvas *canvas = new TCanvas();
  TH1* h = gPad->DrawFrame(-10,0,10,100);
  gPad->Update();

  ULong64_t icount=0;
  while(true){
    gSystem->ProcessEvents();

    EEventType event = static_cast<EEventType>(canvas->GetEvent());
    std::cout<<"\r "<<icount<<"   event="<<event<<"                      "<< std::flush;
    if(event == kKeyPress){

      int ret = getcommand();
      if (ret==-1) break;
    }

    h->FillRandom("gaus",1);
    h->Draw();
    gPad->Update();

    icount++;

    gSystem->Sleep(10);
  }
  std::cout<<std::endl;
}
//
//void testkeyinput(){
//
//  ULong64_t icount=0;
//  while(true){
//    std::cout << "\r "<<icount<<" [q]: quit> " << std::flush;
//    system("stty raw"); 
//    char input = getchar(); 
//    system("stty cooked"); 
//    //    std::cout << std::endl;
//
//    if(input =='q'){
//      std::cout << "Break loop" << std::endl;
//      break;
//    }else if(input =='n'){
//      std::cout<<"n pressed"<<std::flush;
//    }else if(input =='p'){
//      std::cout<<"p pressed"<<std::flush;
//    }else{
//      std::cout<<"others"<<std::flush;
//    }
//
//    icount++;
//  }
//
//}
