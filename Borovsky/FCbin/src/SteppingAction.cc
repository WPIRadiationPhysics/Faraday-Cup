#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Run.hh"
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
void SteppingAction::UserSteppingAction(const G4Step* step) {
  // Get particle charge
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  // Get particle name
  G4String stepParticle = step->GetTrack()->GetDefinition()->GetParticleName();

  // Get step iteration of track number
  G4int stepNum = step->GetTrack()->GetCurrentStepNumber();
  G4int trackNum = step->GetTrack()->GetTrackID();

  // Get name of volume at step location
  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4String volumeName = volume->GetName();

  // Insert particle trajectory into tally file
  G4double netCharge = 0;
  std::ostringstream sstream;

  // particle exits cylinder, -q_i
  if ( volumeName == "Au_cyl" && stepNum == 1 ) { netCharge -= stepCharge; }
  // particle enters cylinder, +q_i
  if ( volumeName == "Au_cyl" && step->GetTrack()->GetTrackStatus() != fAlive ) { netCharge += stepCharge; }

  if ( netCharge != 0 ) { // Zeros already counted
	// Get current run
	G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
	
    // Add to runID's dataset
    G4String data_dir = "data/";
    std::ostringstream rawTallyFileName;
    rawTallyFileName << data_dir << "tallies" << runID << ".txt";
    G4String tallyFileName = rawTallyFileName.str();
    std::ofstream tallyFile;
    tallyFile.open (tallyFileName, std::ios::app);
    tallyFile << netCharge << " " << stepParticle << " " << volumeName << "\n";
    tallyFile.close(); 
  }
}
