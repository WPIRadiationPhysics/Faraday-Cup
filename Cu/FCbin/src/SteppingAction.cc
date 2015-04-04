#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
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
  // Feature: remove step references in var names, and check right away for "last step"
  // Get particle charge
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  // Get particle name
  G4String stepParticle = step->GetTrack()->GetDefinition()->GetParticleName();

  // Get name of volume at step location
  G4String volumeName = step->GetTrack()->GetVolume()->GetName();
  
  // Track net signal calculation
  // Wait for final state and compare original and
  // final positions of charge q
  G4double netSignal = 0;
  if ( step->GetTrack()->GetTrackStatus() != fAlive ) {
	// Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    // ORIGIN / DESTINATION
    if ( volumeNameVertex == "Cu_cyl" ) netSignal -= stepCharge;
    if ( volumeName == "Cu_cyl" ) netSignal += stepCharge;
  }

  if ( netSignal != 0 ) { // Zeros already counted
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    // Add to eventID's dataset
    G4String data_dir = "data/";
    std::ostringstream rawEventFileName;
    rawEventFileName << data_dir << "event" << eventID << "signals.txt";
    G4String eventFileName = rawEventFileName.str();
    std::ofstream eventFile;
    eventFile.open (eventFileName, std::ios::app);
    eventFile << netSignal << "\n";
    eventFile.close();
  }
}
