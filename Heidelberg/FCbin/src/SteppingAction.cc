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

  // Insert particle trajectory into tally file //
  G4double netCharge = 0;
  std::ostringstream sstream;

  /* Add/Remove charges which transport into/out-of the Cu
     Charges which are transported into/out-of a distance
     (del_r, del_z) into Kapton from cylinder have charge equivalence

        q*[1 - max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))]
  */
  
  G4double r_Cu = 30; G4double h_Cu = 100;
  // S59 Beam stop
  G4double r_KA = 30.059; G4double h_KA = 100.118;
  // S100 Beam stop
  //G4double r_KA = 30.100; G4double h_KA = 100.200;
  // S200 Beam stop
  //G4double r_KA = 30.200; G4double h_KA = 100.400;

  // Initial state
  if ( stepNum == 1 ) {
    // particle exits cylinder, -q_i
    if ( volumeName == "Cu_cyl" ) { netCharge -= stepCharge; }
  
    // particle exits Kapton, -q_i*[1-max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))]
    if ( volumeName == "Kapton_cyl1" ) {
      G4double percentR = 0, percentZ = 0;
      // Radial edge of Kapton
      if ( stepR >= r_Cu )
        percentR = (stepR - r_Cu)/(r_KA - r_Cu);
      // Z edge of Kapton
      else
        percentZ = (stepZ - (-h_Cu))/((-h_KA) - (-h_Cu)); // cylinders are upside-down
      G4double chargeProp = ((percentR<percentZ)?percentR:percentZ); // concise maximum function
      netCharge -= stepCharge*(1-chargeProp);
    }
  }
  // Final state
  if ( kinEnergy == 0 ) {
    // particle enters cylinder, +q_i
    if ( volumeName == "Cu_cyl" ) { netCharge += stepCharge; }
    
    // particle enters Kapton, +q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
    if ( volumeName == "Kapton_cyl1" ) {
      G4double percentR = 0, percentZ = 0;
      // Radial edge of Kapton
      if ( stepR >= r_Cu )
        percentR = (stepR - r_Cu)/(r_KA - r_Cu);
      // Z edge of Kapton
      else
        percentZ = (stepZ - (-h_Cu))/((-h_KA) - (-h_Cu)); // cylinders are upside-down
      G4double chargeProp = ((percentR<percentZ)?percentR:percentZ); // concise maximum function
      netCharge += stepCharge*(1-chargeProp);
    }
  }

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
