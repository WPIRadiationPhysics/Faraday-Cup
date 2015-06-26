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

  // Get seed num (look into this, gives error)
  //G4cout << G4RunManager::GetRunManager()->GetCurrentEvent()->GetRandomNumberStatus() << G4endl;

  // Get particle charge
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

  // Get particle name and type
  G4String stepParticle = step->GetTrack()->GetDefinition()->GetParticleName();
  G4int stepParticleType = 99;
  if ( stepParticle == "e-" ) { stepParticleType = 0; }
  if ( stepParticle == "proton" ) { stepParticleType = 1; }
  if ( stepParticle == "neutron" ) { stepParticleType = 3; }
  if ( stepParticle == "gamma" ) { stepParticleType = 4; }
  if ( stepParticle == 99 && stepCharge != 0 ) { stepParticleType = 2; }
 
  // Get step position
  G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
  G4double stepX = stepXYZ[0], stepY = stepXYZ[1], stepZ = stepXYZ[2];

  // Get name of volume at step location
  G4String volumeName = step->GetTrack()->GetVolume()->GetName();

  // Get step momenta
  G4ThreeVector stepPxPyPz = step->GetTrack()->GetMomentum();
  G4double stepPx = stepPxPyPz[0], stepPy = stepPxPyPz[1], stepPz = stepPxPyPz[2];

  // Acquire beamCharge and analysis manager
  G4int beamCharge = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Fill histos for nonzero momenta; separated statements to disclude stationary uncharged secondary processes
  if ( stepParticleType != 99 ) { if ( ! ( stepPx == 0 && stepPy == 0 && stepPz == 0 )) { if ( volumeName != "World" ) {

    // Convert to cylindrical polar coordinates
    G4double stepR = pow(pow(stepX,2) + pow(stepY,2), 0.5),
             stepPr = pow(pow(stepPx,2) + pow(stepPy,2), 0.5);

    // if vec{r} dot vec{pr} is less than zero [thus cos(theta_r-theta_pr)<0], then inward-facing r-momentum
    // {cos/sin}(theta_r) = {stepX/stepY}/stepR, {cos/sin}(theta_pr) = {stepPx/stepPy}/stepPr
    // thus, cos(theta_r-theta_pr) = cos(theta_r)*cos(theta_pr) + sin(theta_r)*sin(theta_pr); ignore denominator
    if ( stepX*stepPx + stepY*stepPy < 0 ) { stepPr = -stepPr; }

    // Get Energy deposition and break into vector components
    G4double eDep = step->GetTotalEnergyDeposit();
    G4double eDepR = eDep*stepPr/pow(pow(stepPr,2) + pow(stepPz,2), 0.5),
             eDepZ = eDep*stepPz/pow(pow(stepPr,2) + pow(stepPz,2), 0.5);

    // Fill ntuple row
    analysisManager->FillNtupleIColumn(0, 0, stepParticleType);
    analysisManager->FillNtupleDColumn(0, 1, stepR);
    analysisManager->FillNtupleDColumn(0, 2, stepZ);
    analysisManager->FillNtupleDColumn(0, 3, eDepR/beamCharge);
    analysisManager->FillNtupleDColumn(0, 4, eDepZ/beamCharge);
    analysisManager->AddNtupleRow(0);
  }}}

  // If end of track
  if ( step->GetTrack()->GetTrackStatus() != fAlive ) {

    // Acquire analysis instance
    Analysis* simulationAnalysis = Analysis::GetAnalysis();
    G4double KA_thickness = simulationAnalysis->GetRunKAThickness();

    // Acquire step r-position 
    //G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
    G4double stepR = pow(pow(stepXYZ[0],2) + pow(stepXYZ[1],2), 0.5);
    stepZ = stepXYZ[2];
  
    // Determine film thickness for calculations
    G4double r_Cu = 30, h_Cu = 100, r_KA, h_KA;
  
    // Acquire thickness index (in mm)
    r_KA = r_Cu + KA_thickness/1000;
    h_KA = h_Cu + 2*KA_thickness/1000;
    // All share common center, half in -z hemispace
    G4double half_Cu = h_Cu/2, half_KA = h_KA/2;

    // Track net signal calculation
    //
    // Wait for final state and compare original and
    // final positions of charge q
    G4double netSignal = 0;
  
    // Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    G4ThreeVector stepXYZVertex = step->GetTrack()->GetVertexPosition();
    G4double stepRVertex = pow(pow(stepXYZVertex[0],2) + pow(stepXYZVertex[1],2), 0.5);
    G4double stepZVertex = stepXYZVertex[2];
    
    // Declare vars
    G4double trackDepth = 0, trackDepthVertex = 0,
             stepRCuDepth = 0, stepZCuDepth = 0,
             stepRKADepth = 0, stepZKADepth = 0,
             stepRKADepthVertex = 0, stepZKADepthVertex = 0;
    G4int histoID;

    // ORIGIN
    // particle exits copper, -q_i
    if ( volumeNameVertex == "Cu_cyl" ) { netSignal -= stepCharge; }
  
    // particle exits Kapton, -q_i*[1-max(del_r/(r_KA - r_Cu), del_z/(half_KA - half_Cu))]
    if ( volumeNameVertex == "Kapton_cyl1" ) {
		
      // Radial edge of Kapton
      if ( stepRVertex >= r_Cu ) { stepRKADepthVertex = (stepRVertex - r_Cu)/(r_KA - r_Cu); }
      // Z edges of Kapton (incoming from -z -> 0)
      if ( stepZVertex <= -half_Cu ) { stepZKADepthVertex = (stepZVertex - (-half_Cu))/((-half_KA) - (-half_Cu)); }
      if ( stepZVertex >= half_Cu ) { stepZKADepthVertex = (stepZVertex - half_Cu)/(half_KA - half_Cu); }
      stepRKADepthVertex = stepRVertex/r_KA;

      trackDepthVertex = 1 - ((stepRKADepthVertex>stepZKADepthVertex)?stepRKADepthVertex:stepZKADepthVertex); // concise maximum function
      netSignal -= stepCharge*trackDepthVertex;
    }
    
    // DESTINATION
    // particle enters copper, +q_i
    if ( volumeName == "Cu_cyl" ) {

      stepRCuDepth = stepR/r_Cu;
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
      stepRKADepth = stepR/r_KA;

      trackDepth = 1 - ((stepRKADepth>stepZKADepth)?stepRKADepth:stepZKADepth); // concise maximum function
      netSignal += stepCharge*trackDepth;
    }

    // Append analysis gain
    simulationAnalysis->appendRunGain(netSignal/beamCharge);

    // If charge particle or neutron/gamma track is relevant to analysis
    if ( ( stepCharge != 0 || ( stepParticle == "neutron"  || stepParticle == "gamma" ) ) ) {
    
      // Copper/Kapton 2D Deposition Histogram
             if ( stepParticle == "e-" && volumeName == "Cu_cyl" ) {
        histoID = analysisManager->GetH2Id("eDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth);
      } else if ( stepParticle == "e-" && volumeName == "Kapton_cyl1" ) {
        histoID = analysisManager->GetH2Id("eDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth);
      } else if ( stepParticle == "proton" && volumeName == "Cu_cyl" ) {
        histoID = analysisManager->GetH2Id("pDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth);
      } else if ( stepParticle == "proton" && volumeName == "Kapton_cyl1" ) {
        histoID = analysisManager->GetH2Id("pDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth);
      } else if ( stepParticle == "neutron" && volumeName == "Cu_cyl" ) {
        histoID = analysisManager->GetH2Id("nDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth);
      } else if ( stepParticle == "neutron" && volumeName == "Kapton_cyl1" ) {
        histoID = analysisManager->GetH2Id("nDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth);
      } else if ( stepParticle == "gamma" && volumeName == "Cu_cyl" ) {
        histoID = analysisManager->GetH2Id("gDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth);
      } else if ( stepParticle == "gamma" && volumeName == "Kapton_cyl1" ) {
        histoID = analysisManager->GetH2Id("gDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth);
      } else if ( stepCharge != 0 && ( stepParticle != "e-" && stepParticle != "proton" ) && volumeName == "Cu_cyl" ) {
        histoID = analysisManager->GetH2Id("oDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth);
      } else if ( stepCharge != 0 && ( stepParticle != "e-" && stepParticle != "proton" ) && volumeName == "Kapton_cyl1" ) {
        histoID = analysisManager->GetH2Id("oDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth);
      }
    }
  }
}
