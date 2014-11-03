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
  // Get particle charge
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  // Get step iteration
  G4int stepNum = step->GetTrack()->GetCurrentStepNumber();

  // Get name of volume at step location
  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4String volumeName = volume->GetName();

  // Get step position r,z-position
  G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
  G4double stepR = pow(pow(stepXYZ[0],2) + pow(stepXYZ[1],2), 0.5);
  G4double stepZ = stepXYZ[2];

  // Get particle energy
  G4double kinEnergy = step->GetTrack()->GetKineticEnergy();

  // Insert particle trajectory info
  G4String stepInfo = "";
  std::ostringstream sstream;
  // Initial state
  if ( stepNum == 1 ) {
    sstream << stepCharge << " FROM " << volumeName << " " << stepR << " " << stepZ << "\n";
    stepInfo = sstream.str();
  }
  // Final state
  if ( kinEnergy == 0 ) {
    sstream << stepCharge << " TO " << volumeName << " " << stepR << " " << stepZ << "\n";
    stepInfo = sstream.str();
  }

  // Add to dataset csv
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  std::ofstream tallyFile;
  tallyFile.open (tallyFileName, std::ios::app);
  tallyFile << stepInfo;
  tallyFile.close();
}
