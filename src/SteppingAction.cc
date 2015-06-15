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
	
  // Acquire run id
  G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();

  // 7 runs per thieck, ditch first 7 (Cu)
  // [Cu runs end up with thickness_KA=0.059mm, but this doesn't matter
  // as without the KA volume it won't undergo calculations anyway]
  G4int KA_i = floor((runID-7)/7);
  if ( KA_i < 0 ) { KA_i = 0; }
	
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
  G4double Kapton_Thickness[6] = {0.059, 0.100, 0.200, 0.059, 0.100, 0.200};
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
	
    // Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    G4ThreeVector stepXYZVertex = step->GetTrack()->GetVertexPosition();
    G4double stepRVertex = pow(pow(stepXYZVertex[0],2) + pow(stepXYZVertex[1],2), 0.5);
    G4double stepZVertex = stepXYZVertex[2];
    
    // Signal statistics
    // Cu
    // 0: e_in
    // 1: e_out
    // 2-3: e_cis
    // 4: p_in
    // 5: p_out
    // 6-7: p_cis
    // 8: o_in
    // 9: o_out
    // 10-11: o_cis
    // KA
    // 12: e_in
    // 13: e_out
    // 14-15: e_cis
    // 16: p_in
    // 17: p_out
    // 18-19: p_cis
    // 20: o_in
    // 21: o_out
    // 22-23: o_cis
    G4int signalType = 99; // 99: null event
    G4double trackDepth = 0, trackDepthVertex = 0,
             stepRCuUndepth = 0, stepRCuUndepthVertex = 0,
             stepZCuDepth = 0, stepZCuDepthVertex = 0,
             stepRKADepth = 0, stepRKAUndepth = 0, stepZKADepth = 0,
             stepRKADepthVertex = 0, stepRKAUndepthVertex = 0, stepZKADepthVertex = 0;

    // ORIGIN
    // particle exits copper, -q_i
    if ( volumeNameVertex == "Cu_cyl" ) {
          
	  stepRCuUndepthVertex = stepRVertex/r_Cu;
	  stepZCuDepthVertex = (stepZVertex + half_Cu)/h_Cu;
	  netSignal -= stepCharge;
    }
  
    // particle exits Kapton, -q_i*[1-max(del_r/(r_KA - r_Cu), del_z/(half_KA - half_Cu))]
    if ( volumeNameVertex == "Kapton_cyl1" ) {
		
      // Radial edge of Kapton
      if ( stepRVertex >= r_Cu ) { stepRKADepthVertex = (stepRVertex - r_Cu)/(r_KA - r_Cu); }
      // Z edges of Kapton (incoming from -z -> 0)
      if ( stepZVertex <= -half_Cu ) { stepZKADepthVertex = (stepZVertex - (-half_Cu))/((-half_KA) - (-half_Cu)); }
      if ( stepZVertex >= half_Cu ) { stepZKADepthVertex = (stepZVertex - half_Cu)/(half_KA - half_Cu); }
      stepRKAUndepthVertex = stepRVertex/r_KA;

      trackDepthVertex = 1 - ((stepRKADepthVertex>stepZKADepthVertex)?stepRKADepthVertex:stepZKADepthVertex); // concise maximum function
      netSignal -= stepCharge*trackDepthVertex;
    }
    
    // DESTINATION
    // particle enters copper, +q_i
    if ( volumeName == "Cu_cyl" ) {

      stepRCuUndepth = stepR/r_Cu;
      stepZCuDepth = (stepZ + half_Cu)/h_Cu;
      netSignal += stepCharge;
    }
        
    // particle enters Kapton, +q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
    if ( volumeName == "Kapton_cyl1" ) {
		
      // Radial edge of Kapton
      if ( stepR >= r_Cu ) { stepRKADepth = (stepR - r_Cu)/(r_KA - r_Cu); }
      // Z edges of Kapton (cylinders are upside-down)
      if ( stepZ <= -half_Cu ) { stepZKADepth = (stepZ - (-half_Cu))/((-half_KA) - (-half_Cu)); }
      if ( stepZ >= half_Cu ) { stepZKADepth = (stepZ - half_Cu)/(half_KA - half_Cu); }
      stepRKAUndepth = stepR/r_KA;
      
      trackDepth = 1 - ((stepRKADepth>stepZKADepth)?stepRKADepth:stepZKADepth); // concise maximum function
      netSignal += stepCharge*trackDepth;
    }

    // If charge particle track is relevant to analysis
    if ( ( stepCharge != 0 ) &&
         ( volumeName == "Cu_cyl" || volumeNameVertex == "Cu_cyl" ||
           volumeName == "KA_cyl1" || volumeNameVertex == "KA_cyl1" ) ) {
    
      // Acquire signal type from particle track for histos
      if ( stepParticle == "e-" ) {

        // Copper
        if ( volumeNameVertex != "Cu_cyl" && volumeName == "Cu_cyl" ) { signalType = 0; }
        else if ( volumeNameVertex == "Cu_cyl" && volumeName != "Cu_cyl" ) { signalType = 1; }
        else if ( volumeNameVertex == "Cu_cyl"  && volumeName == "Cu_cyl" ) { signalType = 2; }
        // Kapton
        else if ( volumeNameVertex != "Kapton_cyl1" && volumeName == "Kapton_cyl1" ) { signalType = 12; }
        else if ( volumeNameVertex == "Kapton_cyl1" && volumeName != "Kapton_cyl1" ) { signalType = 13; }
        else if ( volumeNameVertex == "Kapton_cyl1"  && volumeName == "Kapton_cyl1" ) { signalType = 14; }
      }
      else if ( stepParticle == "proton" ) {

        // Copper
        if ( volumeNameVertex != "Cu_cyl" && volumeName == "Cu_cyl" ) { signalType = 4; }
        else if ( volumeNameVertex == "Cu_cyl" && volumeName != "Cu_cyl" ) { signalType = 5; }
        else if ( volumeNameVertex == "Cu_cyl"  && volumeName == "Cu_cyl" ) { signalType = 6; }
        // Kapton
        else if ( volumeNameVertex != "Kapton_cyl1" && volumeName == "Kapton_cyl1" ) { signalType = 16; }
        else if ( volumeNameVertex == "Kapton_cyl1" && volumeName != "Kapton_cyl1" ) { signalType = 17; }
        else if ( volumeNameVertex == "Kapton_cyl1"  && volumeName == "Kapton_cyl1" ) { signalType = 18; }
      }
      else {

        // Copper
        if ( volumeNameVertex != "Cu_cyl" && volumeName == "Cu_cyl" ) { signalType = 8; }
        else if ( volumeNameVertex == "Cu_cyl" && volumeName != "Cu_cyl" ) { signalType = 9; }
        else if ( volumeNameVertex == "Cu_cyl"  && volumeName == "Cu_cyl" ) { signalType = 10; }
        // Kapton
        else if ( volumeNameVertex != "Kapton_cyl1" && volumeName == "Kapton_cyl1" ) { signalType = 20; }
        else if ( volumeNameVertex == "Kapton_cyl1" && volumeName != "Kapton_cyl1" ) { signalType = 21; }
        else if ( volumeNameVertex == "Kapton_cyl1"  && volumeName == "Kapton_cyl1" ) { signalType = 22; }
      }

      // Acquire beamCharge, eventId and analysis manager
      G4int beamCharge = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
      G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
      G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

      // Fill ntuple row
      analysisManager->FillNtupleIColumn(0, 0, runID);
      analysisManager->FillNtupleIColumn(0, 1, eventID);
      analysisManager->FillNtupleDColumn(0, 2, stepCharge);
      analysisManager->FillNtupleDColumn(0, 3, stepR);
      analysisManager->FillNtupleDColumn(0, 4, stepZ);
      analysisManager->FillNtupleDColumn(0, 5, stepRVertex);
      analysisManager->FillNtupleDColumn(0, 6, stepZVertex);
      analysisManager->FillNtupleDColumn(0, 7, stepRCuUndepth);
      analysisManager->FillNtupleDColumn(0, 8, stepZCuDepth);
      analysisManager->FillNtupleDColumn(0, 9, stepRCuUndepthVertex);
      analysisManager->FillNtupleDColumn(0, 10, stepZCuDepthVertex);
      analysisManager->FillNtupleDColumn(0, 11, stepRKAUndepth);
      analysisManager->FillNtupleDColumn(0, 12, stepZKADepth);
      analysisManager->FillNtupleDColumn(0, 13, stepRKAUndepthVertex);
      analysisManager->FillNtupleDColumn(0, 14, stepZKADepthVertex);
      analysisManager->FillNtupleDColumn(0, 15, netSignal/beamCharge);
      analysisManager->FillNtupleIColumn(0, 16, signalType);
      analysisManager->AddNtupleRow(0);
    }
  }
}
