/*
 *  $Id: DmpSimPrimaryGeneratorAction.cc, 2014-05-24 12:42:06 DAMPE $
 *  Author(s):
 *    Chi WANG (chiwang@mail.ustc.edu.cn) 03/03/2014
*/

#include "G4GeneralParticleSource.hh"

#include "DmpSimPrimaryGeneratorAction.h"
#include "DmpEvtMCPrimaryParticle.h"
#include "DmpDataBuffer.h"
#include "DmpCore.h"

DmpSimPrimaryGeneratorAction::DmpSimPrimaryGeneratorAction()
 :fGPS(0),
  fPrimaryParticle(0)
{
  fGPS = new G4GeneralParticleSource();
  //fGPS->SetNumberOfParticles(1);
  //fGPS->SetParticleDefinition(particle);
  fPrimaryParticle = new DmpEvtMCPrimaryParticle();
  gDataBuffer->RegisterObject("Event/MCTruth/PrimaryParticle",fPrimaryParticle,"DmpEvtMCPrimaryParticle");
}

//-------------------------------------------------------------------
DmpSimPrimaryGeneratorAction::~DmpSimPrimaryGeneratorAction(){
  delete fGPS;
  delete fPrimaryParticle;
}

//-------------------------------------------------------------------
void DmpSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent){
  fGPS->GeneratePrimaryVertex(anEvent);
  // Set information of current event
  fPrimaryParticle->SetEventID(anEvent->GetEventID());
  fPrimaryParticle->SetTime(fGPS->GetParticleTime());
  fPrimaryParticle->SetPosition(fGPS->GetParticlePosition().x(),fGPS->GetParticlePosition().y(),fGPS->GetParticlePosition().z());
  fPrimaryParticle->SetDirection(fGPS->GetParticleMomentumDirection().x(),fGPS->GetParticleMomentumDirection().y(),fGPS->GetParticleMomentumDirection().z());
  fPrimaryParticle->SetKineticEnergy(fGPS->GetParticleEnergy());
  G4ParticleDefinition *primaryParticle = fGPS->GetParticleDefinition();
  fPrimaryParticle->SetPDGCode(primaryParticle->GetPDGEncoding());
  fPrimaryParticle->SetMass(primaryParticle->GetPDGMass());
  fPrimaryParticle->SetCharge(primaryParticle->GetPDGCharge());
  fPrimaryParticle->SetComponent(primaryParticle->GetLeptonNumber(),primaryParticle->GetBaryonNumber());
  
  //debug
  std::cout << "This is GPS debug message: Particle mass=" << primaryParticle->GetPDGMass() << ", Kinetic=" << fGPS->GetParticleEnergy() << ", Charge=" << primaryParticle->GetPDGCharge() << std::endl;
}

