/*
 *  $Id: DmpSimBgoSD.cc, 2014-08-20 15:44:18 DAMPE/USTC $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cen) 03/03/2014
*/

#include "TClonesArray.h"

#include "G4Step.hh"
#include "G4TouchableHistory.hh"

#include "DmpSimBgoSD.h"
#include "DmpEvtMCBgo.h"
#include "DmpDataBuffer.h"

//-------------------------------------------------------------------
DmpSimBgoSD::DmpSimBgoSD()
 :G4VSensitiveDetector("BgoSD"),
  fEvtMCBgo(0),
  fEvtFdigiBgo(0)
{
        GetMipPar();
        GetAttPar();
  fEvtMCBgo = new DmpEvtMCBgo();
// *  TODO:  check Register status
  gDataBuffer->RegisterObject("Event/MCTruth/Bgo",fEvtMCBgo,"DmpEvtMCBgo");
  gDataBuffer->RegisterObject("Event/MCTruth/BgoFdigi",fEvtFdigiBgo,"DmpEvtFdigiBgo");
}

//-------------------------------------------------------------------
DmpSimBgoSD::~DmpSimBgoSD(){
  delete fEvtMCBgo;
  delete fEvtFdigiBgo;
}

//-------------------------------------------------------------------
#include <boost/lexical_cast.hpp>
G4bool DmpSimBgoSD::ProcessHits(G4Step *aStep,G4TouchableHistory*){
  G4TouchableHistory *theTouchable = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  std::string barName = theTouchable->GetVolume(1)->GetName();
  barName.assign(barName.end()-4,barName.end());        // get ID
  short barID = boost::lexical_cast<int>(barName);

  G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
  fEvtMCBgo->AddG4Hit(barID,aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm,position.z()/mm);
  Eny2ADC(barID,aStep->GetTotalEnergyDeposit()/MeV,position.x()/mm,position.y()/mm);
  return true;
}

//-------------------------------------------------------------------
void DmpSimBgoSD::Initialize(G4HCofThisEvent*){
	memset(TotalE,0,sizeof(TotalE));
}

//-------------------------------------------------------------------
void DmpSimBgoSD::EndOfEvent(G4HCofThisEvent* HCE){
//sampling & save
Sampling();
}

  //Get Attenuation coefficients 
void DmpSimBgoSD::GetAttPar(){
  ifstream Apar;
  Apar.open("/home/zhzhy/Dampe/Calibration/Attenuation/AttPar");
  if(!Apar.good()){
    std::cout<<"Can not open Att Par file!"<<std::endl;
    exit(1);
  } 
  int nGbar=14*22;
  for(int i=0; i<nGbar;i++){
    for(int j=0 ;j<2;j++){ 
      Apar>>AttPar[i][j]; 
      std::cout<<AttPar[i][j]<<"\t";
    }
    std::cout<<std::endl;
  }
  Apar.close();
}   
  //Get MIPs parameters
void DmpSimBgoSD::GetMipPar(){
  ifstream Mpar;
  Mpar.open("/home/zhzhy/Dampe/Calibration/MIPs/MIPsPar");
  if(!Mpar.good()){
    std::cout<<"Can not open MIPs Par file!"<<std::endl;
    exit(1);
  } 
  int nGpmt=14*2*22;
  for(int i=0;i<nGpmt;i++){
    for(int j=0;j<4;j++){
      Mpar>>MipPar[i][j];  
      std::cout<<MipPar[i][j]<<"\t";
    }
    std::cout<<std::endl;
  }
  Mpar.close();
}   
  //step Energy to ADC
void DmpSimBgoSD::Eny2ADC(const short &id, const double &e, const double &x,const double &y){
  //Get iGpmt, iGbar
  short layer=(short)(id/100);
  short bar=id%100;
  short iGbar=layer*22+bar;
  short iGpmt[2]={layer*2*22+bar,(layer*2+1)*22+bar};

  //Set Att coe, MIPs coversion ratio;
  double Dis[2]; //distance between energy deposition and BGO end.
  if(layer%2==0){
    Dis[1]=x+300;
    Dis[0]=300-x;
  }
  else{
    Dis[1]=y+300;
    Dis[0]=300-y;
  }
  double AttCoe[2];  //AttPar[][0]=2/lambda; lambda :cm Dis mm;
  AttCoe[0]=1/TMath::Exp(AttPar[iGbar][0]*Dis[0]/2/10);
  AttCoe[1]=1/TMath::Exp(AttPar[iGbar][0]*Dis[1]/2/10);
  double MipCov[2];
  MipCov[0]=MipPar[iGpmt[0]][1]*TMath::Exp(AttPar[iGbar][0]*30/2)/22.5;   //non-att normalized ADC counts/MeV;
  MipCov[1]=MipPar[iGpmt[1]][1]*TMath::Exp(AttPar[iGbar][0]*30/2)/22.5;   //non-att normalized ADC counts/MeV;
  double AttHit[2];
  AttHit[0]=e*AttCoe[0]*MipCov[0];
  AttHit[1]=e*AttCoe[1]*MipCov[1];
  TotalE[iGpmt[0]]+=AttHit[0];
  TotalE[iGpmt[1]]+=AttHit[1];
}
  //Sampling with calibrated paramneters
  void DmpSimBgoSD::Sampling(){
    //Get iGpmt, iGbar
    for(int layer=0;layer<14;layer++){
      for(int bar=0;bar<22;bar++){
        short iGbar=layer*22+bar;
        short iGpmt[2]={layer*2*22+bar,(layer*2+1)*22+bar};
        if(TotalE[iGpmt[0]]!=0){
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<iGpmt[0]<<std::endl;
	  std::cout<<TotalE[iGpmt[0]]<<std::endl;
	  std::cout<<iGpmt[1]<<std::endl;
	  std::cout<<TotalE[iGpmt[1]]<<std::endl;
          double Mean[2]={TotalE[iGpmt[0]],TotalE[iGpmt[1]]};
          double Sigma[2]={MipPar[iGpmt[0]][3]*TMath::Sqrt(Mean[0]/MipPar[iGpmt[0]][1]),MipPar[iGpmt[1]][3]*TMath::Sqrt(Mean[1]/MipPar[iGpmt[1]][1])};
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  
          TRandom *s0=new TRandom();
          TRandom *s1=new TRandom();
	  fEvtFdigiBgo->fGlobalPmtID.push_back(iGpmt[0]);
          fEvtFdigiBgo->fADC.push_back((short)(s0->Gaus(Mean[0],Sigma[0])));
	  fEvtFdigiBgo->fGlobalPmtID.push_back(iGpmt[1]);
          fEvtFdigiBgo->fADC.push_back((short)(s1->Gaus(Mean[1],Sigma[1])));
          delete s0;
          delete s1;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
	  std::cout<<"~~~~~~~~~~~~~~"<<std::endl;
        }
      }
    }
  }

