/*
 *  $Id: DmpSimBgoSD.h, 2014-05-22 17:04:23 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#ifndef DmpSimBgoSD_H
#define DmpSimBgoSD_H

#include "G4VSensitiveDetector.hh"
#include "DmpEvtFdigiBgo.h"
#include "fstream"
#include "TMath.h"
#include "TRandom.h"
#include "iostream"
class DmpEvtMCBgo;

class DmpSimBgoSD : public G4VSensitiveDetector{
/*
 *  DmpSimBgoSD
 *
 */
public:
  DmpSimBgoSD();
  ~DmpSimBgoSD();
  G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
  void Initialize(G4HCofThisEvent*);
  void EndOfEvent(G4HCofThisEvent*);
  //Get MIPs parameters
  void GetMipPar();
  //Get Attenuation coefficients 
  void GetAttPar();
  //step Energy to ADC
  void Eny2ADC(const short &id, const double &e, const double &x,const double &y);
  //Sampling
  void Sampling();
private:
  DmpEvtMCBgo      *fEvtMCBgo;
  DmpEvtFdigiBgo   *fEvtFdigiBgo;
  double MipPar[616][4];
  double AttPar[308][2];
//  double AttHit[2];//ADC counts
  double TotalE[616];
  TRandom *RanGaus[616];
};

#endif
