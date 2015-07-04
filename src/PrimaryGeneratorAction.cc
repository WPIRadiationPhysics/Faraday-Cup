#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <math.h>

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0) {

  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // Default particle kinematic
  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
  fParticleGun->SetParticleEnergy(50.*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  // This function is called at the begining of event
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Tubs* worldTubs = 0;
  
  if ( worldLV) worldTubs = dynamic_cast< G4Tubs*>(worldLV->GetSolid()); 
  if ( worldTubs ) { worldZHalfLength = worldTubs->GetZHalfLength(); }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  }

  // Acquire analysis object and run FWHM
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  G4double beamFWHM = simulationAnalysis->GetRunBeamFWHM()*mm;

  // Acquire position within a Gaussian profile of given FWHM via radial StDev
  // Azimuthal symmetry assumed
  G4double beamSigmaR = beamFWHM/(2*pow(2*log(2), 0.5));
  G4double beamX = G4RandGauss::shoot(0, beamSigmaR/pow(2, 0.5)),
           beamY = G4RandGauss::shoot(0, beamSigmaR/pow(2, 0.5));

  // Set gun position in Cartesian coordinates
  fParticleGun->SetParticlePosition(G4ThreeVector(beamX, beamY, -2*worldZHalfLength));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
