
#include "DmpAlgFdigiTest.h"
#include "DmpRootIOSvc.h"
#include "DmpDataBuffer.h"
#include "DmpParameterBgo.h"
#include "TFile.h"
//-------------------------------------------------------------------
DmpAlgFdigiTest::DmpAlgFdigiTest()
 :DmpVAlg("FdigiTest"),
// fEvtMCPrimaryParticle(0),
 fEvtMCBgo(0),
 fEvtFdigiBgo(0)
{
}

//-------------------------------------------------------------------
DmpAlgFdigiTest::~DmpAlgFdigiTest(){
}

//-------------------------------------------------------------------
bool DmpAlgFdigiTest::Initialize(){
// gRootIOSvc->Set("OutData/FileName","./FdigiTest.root");
  // read input data
//  fEvtMCPrimaryParticle = new DmpEvtMCPrimaryParticle();
//  if(not gDataBuffer->ReadObject("Event/MCTruth/PrimaryParticle",fEvtMCPrimaryParticle)){
//  return false;
//}
  fEvtMCBgo = new DmpEvtMCBgo();
  if(not gDataBuffer->ReadObject("Event/MCTruth/Bgo",fEvtMCBgo)){
    return false;
  }
  fEvtFdigiBgo = new DmpEvtFdigiBgo();
  if(not gDataBuffer->ReadObject("Event/MCTruth/BgoFdigi",fEvtFdigiBgo)){
    return false;
  }
  // create Hist map
  short layerNo = DmpParameterBgo::kPlaneNo*2;
  short barNo = DmpParameterBgo::kBarNo;
  for(short l=0;l<layerNo;++l){

    for(short b=0;b<barNo;++b){
      for(short s=0;s<DmpParameterBgo::kSideNo;++s){
        char name[50];
        short iGpmt = (l*2+s)*22+b ;
          sprintf(name,"BgoPed_L%02d_S%02d_B%02d",l,s,b);
          fFdigiMipsHist.insert(std::make_pair(iGpmt,new TH1F(name,name,5000,0,15000)));
      }
    }
  }
 return true;
}

//-------------------------------------------------------------------
  bool DmpAlgFdigiTest::ProcessThisEvent(){
    short nSignal = fEvtFdigiBgo->fGlobalPmtID.size();

    std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
    std::cout<<nSignal<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
    std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
    
    for(short i=0;i<nSignal;++i){
      fFdigiMipsHist[fEvtFdigiBgo->fGlobalPmtID[i]]->Fill(fEvtFdigiBgo->fADC[i]);
      std::cout<<fEvtFdigiBgo->fADC[i]<<"\t";
    }
    return true;
  }

//-------------------------------------------------------------------
bool DmpAlgFdigiTest::Finalize(){
 // TF1 *gausFit = new TF1("GausFit","gaus",-500,500);
 std::string histFileName = "Fdigi_Hist.root";
TFile *histFile = new TFile(histFileName.c_str(),"RECREATE");
  for(std::map<short,TH1F*>::iterator aHist=fFdigiMipsHist.begin();aHist!=fFdigiMipsHist.end();++aHist){
      aHist->second->Write();
      delete aHist->second;
      }
  histFile->Close();
  return true;
}

