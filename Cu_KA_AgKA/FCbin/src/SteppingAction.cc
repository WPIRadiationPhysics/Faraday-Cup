#include "Analysis.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"

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
	
  // Acquire run id and thickness index
  G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  G4int KA_i = floor(runID/7); // 7 runs per geometry
	
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
  
  // Determine film thickness for calculations
  G4double r_Cu = 30, h_Cu = 100, r_KA, h_KA;
  G4String data_dir = "data/";
  
  // Acquire thickness index
  G4double Kapton_Thickness[3] = {0.059, 0.100, 0.200};
  r_KA = r_Cu + Kapton_Thickness[KA_i];
  h_KA = h_Cu + 2*Kapton_Thickness[KA_i];
  // All share common center, half in -z hemispace
  G4double half_Cu = h_Cu/2, half_KA = h_KA/2;

  // Track net signal calculation
  //
  // Wait for final state and compare original and
  // final positions of charge q
  G4double netSignal = 0;
  
  // If end of track
  if ( step->GetTrack()->GetTrackStatus() != fAlive ) {
	
	// Acquire Analysis Manager instance
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	
    // Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    G4ThreeVector stepXYZVertex = step->GetTrack()->GetVertexPosition();
    G4double stepRVertex = pow(pow(stepXYZVertex[0],2) + pow(stepXYZVertex[1],2), 0.5);
    G4double stepZVertex = stepXYZVertex[2];
    
    // Signal statistics
    // 0: (World + AgKA) -> KA1
    // 1: (World + AgKA) -> Cu
    // 2: KA1 -> Cu
    // 3: KA1 -> (World + AgKA)
    // 4: Cu -> KA1
    // 5: Cu -> (AgKA + World)
    // 6: KA1 -> KA1
    G4int signalType;
    G4double trackDepth = 0, trackDepthVertex = 0;
    
    // ORIGIN
    // particle exits copper, -q_i
    if ( volumeNameVertex == "Cu_cyl" ) {
	  netSignal -= stepCharge;
	  trackDepthVertex = (stepZVertex + half_Cu)/h_Cu;
	  
	  if ( volumeName != "Cu_cyl" &&  volumeName != "Kapton_cyl1" ) { signalType = 5; }
	  if ( volumeName == "Kapton_cyl1" ) { signalType = 4; }
	}
  
    // particle exits Kapton, -q_i*[1-max(del_r/(r_KA - r_Cu), del_z/(half_KA - half_Cu))]
    if ( volumeNameVertex == "Kapton_cyl1" ) {
		
      G4double percentRVertex = 0, percentZVertex = 0;
      // Radial edge of Kapton
      if ( stepRVertex >= r_Cu ) { percentRVertex = (stepRVertex - r_Cu)/(r_KA - r_Cu); }
      // Z edges of Kapton (incoming from -z -> 0)
      if ( stepZVertex <= -half_Cu ) { percentZVertex = (stepZVertex - (-half_Cu))/((-half_KA) - (-half_Cu)); }
      if ( stepZVertex >= half_Cu ) { percentZVertex = (stepZVertex - half_Cu)/(half_KA - half_Cu); }
      
      trackDepthVertex = 1 - ((percentRVertex>percentZVertex)?percentRVertex:percentZVertex); // concise maximum function
      netSignal -= stepCharge*trackDepthVertex;
      
      if ( volumeName != "Cu_cyl" &&  volumeName != "Kapton_cyl1" ) { signalType = 3; }
    }
    
    // DESTINATION
    // particle enters copper, +q_i
    if ( volumeName == "Cu_cyl" ) {
      netSignal += stepCharge;
      trackDepth = (stepZ + half_Cu)/h_Cu;
      
      if ( volumeNameVertex == "Kapton_cyl1" ) { signalType = 2; }
      if ( volumeNameVertex != "Cu_cyl" &&  volumeNameVertex != "Kapton_cyl1" ) { signalType = 1; }
    }
        
    // particle enters Kapton, +q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
    if ( volumeName == "Kapton_cyl1" ) {
		
      G4double percentR = 0, percentZ = 0;
      // Radial edge of Kapton
      if ( stepR >= r_Cu ) { percentR = (stepR - r_Cu)/(r_KA - r_Cu); }
      // Z edges of Kapton (cylinders are upside-down)
      if ( stepZ <= -half_Cu ) { percentZ = (stepZ - (-half_Cu))/((-half_KA) - (-half_Cu)); }
      if ( stepZ >= half_Cu ) { percentZ = (stepZ - half_Cu)/(half_KA - half_Cu); }
      
      trackDepth = 1 - ((percentR>percentZ)?percentR:percentZ); // concise maximum function
      netSignal += stepCharge*trackDepth;
      
      if ( volumeNameVertex != "Cu_cyl" &&  volumeNameVertex != "Kapton_cyl1" ) { signalType = 0; }
      if ( volumeNameVertex ==  "Kapton_cyl1" ) { signalType = 6; }
    }
    
    if ( netSignal != 0 ) { // Zeros already counted
    
      // Acquire beamCharge and eventId
      G4int beamCharge = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
      G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

      // Fill ntuple row
      analysisManager->FillNtupleIColumn(0, 0, runID);
      analysisManager->FillNtupleIColumn(0, 1, eventID);
      analysisManager->FillNtupleDColumn(0, 2, stepCharge);
      analysisManager->FillNtupleDColumn(0, 3, stepR);
      analysisManager->FillNtupleDColumn(0, 4, stepZ);
      analysisManager->FillNtupleDColumn(0, 5, stepRVertex);
      analysisManager->FillNtupleDColumn(0, 6, stepZVertex);
      analysisManager->FillNtupleDColumn(0, 7, netSignal/beamCharge);
      analysisManager->FillNtupleIColumn(0, 8, signalType);
      analysisManager->FillNtupleDColumn(0, 9, trackDepth);
      analysisManager->FillNtupleDColumn(0, 10, trackDepthVertex);
      analysisManager->AddNtupleRow(0);
    }
  }
}
