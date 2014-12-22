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

  // Get step position r,z-position
  G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
  G4double stepR = pow(pow(stepXYZ[0],2) + pow(stepXYZ[1],2), 0.5);
  G4double stepZ = stepXYZ[2];

  // Beam Stop dimensions (mm)
  // Cu cyl is positioned downward from z=0 plane
  G4double r_Cu = 50, h_Cu = 150;
  G4double r_KA = r_Cu + 5, h_KA = h_Cu + 5; // half under z=0
  G4String data_dir = "data/";

  // Track net signal calculation; wait for final state and compare to track origin
  G4double netSignal = 0;
  if ( step->GetTrack()->GetTrackStatus() != fAlive ) {
	
	// Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    G4ThreeVector stepXYZVertex = step->GetTrack()->GetVertexPosition();
    G4double stepRVertex = pow(pow(stepXYZVertex[0],2) + pow(stepXYZVertex[1],2), 0.5);
    G4double stepZVertex = stepXYZVertex[2];
    
    // ORIGIN
    // particle exits cylinder, -q_i
    if ( volumeNameVertex == "Cu_cyl" ) { netSignal -= stepCharge; }
  
    // particle exits Kapton, -q_i*[1-max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))]
    if ( volumeNameVertex == "Kapton_cyl1" ) {
      G4double percentRVertex = 0, percentZVertex = 0;
      
      // Radial edge of Kapton
      if ( stepRVertex >= r_Cu ) { percentRVertex = (stepRVertex - r_Cu)/(r_KA - r_Cu); }
      
      // Z edges of Kapton (cylinders are upside-down)
      if ( stepZVertex <= -h_Cu ) { percentZVertex = (stepZVertex - (-h_Cu))/((-h_KA) - (-h_Cu)); }
      if ( stepZVertex >= 0 ) { percentZVertex = stepZVertex/(h_KA - h_Cu); }
      
      // concise maximum function
      G4double chargeProp = ((percentRVertex<percentZVertex)?percentRVertex:percentZVertex);
      netSignal -= stepCharge*(1-chargeProp);
    }
    
    // DESTINATION
    // particle enters cylinder, +q_i
    if ( volumeName == "Cu_cyl" ) { netSignal += stepCharge; }
        
    // particle enters Kapton, +q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
    if ( volumeName == "Kapton_cyl1" ) {
      G4double percentR = 0, percentZ = 0;
      
      // Radial edge of Kapton
      if ( stepR >= r_Cu ) { percentR = (stepR - r_Cu)/(r_KA - r_Cu); }
      
      // Z edges of Kapton (cylinders are upside-down)
      if ( stepZ <= -h_Cu ) { percentZ = (stepZ - (-h_Cu))/((-h_KA) - (-h_Cu)); }
      if ( stepZ >= 0 ) { percentZ = stepZ/(h_KA - h_Cu); }
      
      // concise maximum function
      G4double chargeProp = ((percentR<percentZ)?percentR:percentZ);
      netSignal += stepCharge*(1-chargeProp);
    }
  }

  if ( netSignal != 0 ) { // Zeros already counted
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    // Add to eventID's dataset
    std::ostringstream rawEventFileName;
    rawEventFileName << data_dir << "event" << eventID << "signals.txt";
    G4String eventFileName = rawEventFileName.str();
    std::ofstream eventFile;
    eventFile.open (eventFileName, std::ios::app);
    eventFile << netSignal << "\n";
    eventFile.close();
  }
}
