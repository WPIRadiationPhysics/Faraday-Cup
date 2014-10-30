#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>

// Initialize Step Procedure
SteppingAction::SteppingAction(
                const DetectorConstruction* detectorConstruction,
                EventAction* eventAction)
               : G4UserSteppingAction(),
                 fDetConstruction(detectorConstruction),
                 fEventAction(eventAction) {}
SteppingAction::~SteppingAction() {}

// Step Procedure (for every step...)
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Get track ID #
  G4int trackID = step->GetTrack()->GetTrackID();

  // Get particle name
  G4String stepParticle = step->GetTrack()->GetDefinition()->GetParticleName();

  // Get particle charge
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  // Get step iteration
  G4int stepNum = step->GetTrack()->GetCurrentStepNumber();

  // Get name of volume at step location
  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4String volumeName = volume->GetName();

  // Get step position z-position
  G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
  G4double stepZ = stepXYZ[2];

  // Get particle energy and momentum
  G4ThreeVector momentum = step->GetTrack()->GetMomentum();
  G4double kinEnergy = step->GetTrack()->GetKineticEnergy();

  //// Get energy deposited at step location, convert to order of magnitude (oom)
  //G4double edep = step->GetTotalEnergyDeposit(); //G4int edep_oom;
  //if ( edep != 0 ) { edep_oom = floor(log2(edep)/log2(10)); } else { edep_oom = 0; }

  //// Initial state
  //G4String stepInfo = "";
  //std::ostringstream sstream;
  //if ( stepNum == 1 )
  //  sstream << stepParticle << " " << trackID << " begins in " << volumeName;
  //  stepInfo = sstream.str();
  //if ( kinEnergy == 0 ) {
  //  sstream << stepParticle << " " << trackID << " ends up in " << volumeName;
  //  stepInfo = sstream.str();
  //}

  // Add to dataset csv
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  std::ofstream tallyFile;
  tallyFile.open (tallyFileName, std::ios::app);
  tallyFile << trackID << " " << stepParticle << " " << stepCharge << " " << stepNum << " " << volumeName << " " << stepZ << " " << kinEnergy << "\n";
  tallyFile.close();
}