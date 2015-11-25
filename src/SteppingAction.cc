#include "Analysis.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "G4SteppingManager.hh"

#include "G4Step.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

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
SteppingAction::SteppingAction( const DetectorConstruction* detectorConstruction,
                               EventAction* eventAction)
                               : G4UserSteppingAction(),
                                 fDetConstruction(detectorConstruction),
                                 fEventAction(eventAction) {}
SteppingAction::~SteppingAction() {}

// Step Procedure (for every step...)
void SteppingAction::UserSteppingAction(const G4Step* step) {

  //// WISHLIST: Separate stepping data-collection processes into
  //// their own function to be called alongside specific analyses
  //// (note: currently recycling some vars between latter functions)

  // Acquire world logical volume dimensions
  G4LogicalVolume* CuLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Cu_cyl");
  G4Tubs* CuTubs = 0;
  CuTubs = dynamic_cast< G4Tubs*>(CuLV->GetSolid()); 
  G4double Cu_cylZHalfLength = CuTubs->GetZHalfLength();

  // Acquire analysis instance
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int histoID;

  // Acquire beam charge
  G4int beamCharge = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  // Get name of volume at step location
  G4String volumeName = step->GetTrack()->GetVolume()->GetName();

  // Acquire Kapton thickness, then in mm
  G4double KA_thickness = simulationAnalysis->GetRunKAThickness();
  G4double t_KA = KA_thickness/1000;

  // Get particle name, position, charge
  G4String stepParticle = step->GetTrack()->GetDefinition()->GetParticleName();
  G4ThreeVector stepXYZ = step->GetPostStepPoint()->GetPosition();
  G4double stepX = stepXYZ[0], stepY = stepXYZ[1], stepZ = stepXYZ[2];
  G4double stepCharge = step->GetTrack()->GetDefinition()->GetPDGCharge();

/* 2015-11-15 Suppress analyses and extra models
  //// Branching ratios analysis data
  // Gottschalk's active sheet range (R = 0.484 g/cm2 = 8.96*t_Cu + 1.42*t_KA) with [t]=cm
  G4double t_Cu = 10*(0.484 - (1.42*t_KA/10))/8.96; // in mm

  // If primary track step within Gottschalk range
  if ( step->GetTrack()->GetParentID() == 0 ) {
  if ( stepZ < (-Cu_cylZHalfLength + t_Cu) ) {

    // Acquire multithreaded worker id
    G4int workerID = G4Threading::G4GetThreadId();

    // Obtain total number of and track pointer to secondary particles
    G4int nSecAtRest = fpSteppingManager->GetfN2ndariesAtRestDoIt();
    G4int nSecAlong  = fpSteppingManager->GetfN2ndariesAlongStepDoIt();
    G4int nSecPost   = fpSteppingManager->GetfN2ndariesPostStepDoIt();
    G4int nSecTotal  = nSecAtRest+nSecAlong+nSecPost;
    G4TrackVector* secVec = fpSteppingManager->GetfSecondary();

    G4int stepNumBranchProtons = 0, stepNumBranchNeutrons = 0;
    G4String branchParticle;

    // If secondaries-producing scatter
    if ( nSecTotal > 0 ) {
      for (size_t lp1=(*secVec).size()-nSecTotal; lp1<(*secVec).size(); lp1++) {

        // Tally protons and neutrons
        branchParticle = (*secVec)[lp1]->GetDefinition()->GetParticleName();
        if ( branchParticle == "proton" ) { stepNumBranchProtons += 1; }
        if ( branchParticle == "neutron" ) { stepNumBranchNeutrons += 1; }
      }

      simulationAnalysis->appendEventBranchingPN(stepNumBranchProtons, stepNumBranchNeutrons, workerID);
    }
  }}
*/

/* 2015-11-15 Suppress analyses and extra models
  //// Energy deposition histo and cascade analysis data

  // Get seed num (look into this, gives error)
  //G4cout << G4RunManager::GetRunManager()->GetCurrentEvent()->GetRandomNumberStatus() << G4endl;

  // Get Energy deposition
  G4double eDep = step->GetTotalEnergyDeposit();

  // Get particle type
  G4int stepParticleType = 99;
  if ( stepParticle == "e-" ) { stepParticleType = 0; }
  if ( stepParticle == "proton" ) { stepParticleType = 1; }
  if ( stepParticle == "neutron" ) { stepParticleType = 3; }
  if ( stepParticle == "gamma" ) { stepParticleType = 4; }
  if ( stepParticle == 99 && stepCharge != 0 ) { stepParticleType = 2; }
 
  // Get step momenta
  G4ThreeVector stepPxPyPz = step->GetTrack()->GetMomentum();
  G4double stepPx = stepPxPyPz[0], stepPy = stepPxPyPz[1], stepPz = stepPxPyPz[2];

  // Acquire beam energy
  G4double runEnergy = simulationAnalysis->GetRunEnergy();

  // Fill histos for nonzero momenta; separated statements to disclude stationary uncharged secondary processes
  if ( stepParticleType != 99 ) { if ( ! ( stepPx == 0 && stepPy == 0 && stepPz == 0 )) { if ( volumeName != "World" ) {

    // Acquire world logical volume dimensions
    G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Tubs* worldTubs = 0;
    worldTubs = dynamic_cast< G4Tubs*>(worldLV->GetSolid()); 
    G4double worldZHalfLength = worldTubs->GetZHalfLength(),
             worldOuterRadius = worldTubs->GetOuterRadius();

    // Convert to cylindrical polar coordinates
    G4double stepR = pow(pow(stepX,2) + pow(stepY,2), 0.5),
             stepPr = pow(pow(stepPx,2) + pow(stepPy,2), 0.5);

    // if vec{r} dot vec{pr} is less than zero [thus cos(theta_r-theta_pr)<0], then inward-facing r-momentum
    // {cos/sin}(theta_r) = {stepX/stepY}/stepR, {cos/sin}(theta_pr) = {stepPx/stepPy}/stepPr
    // thus, cos(theta_r-theta_pr) = cos(theta_r)*cos(theta_pr) + sin(theta_r)*sin(theta_pr); ignore denominator
    if ( stepX*stepPx + stepY*stepPy < 0 ) { stepPr = -stepPr; }

    // Acquire position as bin in 100^2 grid
    G4int rBin = floor((stepR/worldOuterRadius)*100),
          zBin = floor(0.5*(1+(stepZ/worldZHalfLength))*100);
    // Adjust slight functional anomaly
    if ( rBin >= 100 ) { rBin = 99; }
    if ( zBin >= 100 ) { zBin = 99; }

    // Break energy deposition into vector components
    G4double eDepR = eDep*stepPr/pow(pow(stepPr,2) + pow(stepPz,2), 0.5),
             eDepZ = eDep*stepPz/pow(pow(stepPr,2) + pow(stepPz,2), 0.5);

    // Fill eDep analysis structures
    switch ( stepParticleType) {
      case 0: simulationAnalysis->appendECascade(rBin, zBin, eDepR, eDepZ); break;
      case 1: simulationAnalysis->appendPCascade(rBin, zBin, eDepR, eDepZ); break;
      case 2: simulationAnalysis->appendOCascade(rBin, zBin, eDepR, eDepZ); break;
      case 3: simulationAnalysis->appendNCascade(rBin, zBin, eDepR, eDepZ); break;
      case 4: simulationAnalysis->appendGCascade(rBin, zBin, eDepR, eDepZ); break;
    }

    // Fill ntuple row
    //analysisManager->FillNtupleIColumn(0, 0, stepParticleType);
    //analysisManager->FillNtupleDColumn(0, 1, stepR);
    //analysisManager->FillNtupleDColumn(0, 2, stepZ);
    //analysisManager->FillNtupleDColumn(0, 3, eDepR/beamCharge);
    //analysisManager->FillNtupleDColumn(0, 4, eDepZ/beamCharge);
    //analysisManager->AddNtupleRow(0);
  }}}
*/

  //// Particle energy spectra analysis data
  // If initial step of non-primary track
  if ( step->GetTrack()->GetCurrentStepNumber() == 1 && step->GetTrack()->GetParentID() != 0 ) {

  // Obtain and fill respective spectra histogram with kinetic energy
    G4double stepKE = step->GetTrack()->GetKineticEnergy();
    if ( stepKE != 0 ) {
      if ( stepParticle == "e-" )
        { histoID = analysisManager->GetH1Id("eSpectra"); analysisManager->FillH1(histoID, stepKE); }
      else if ( stepParticle == "proton" )
        { histoID = analysisManager->GetH1Id("pSpectra"); analysisManager->FillH1(histoID, stepKE); }
      else if ( stepParticle == "neutron" )
        { histoID = analysisManager->GetH1Id("nSpectra"); analysisManager->FillH1(histoID, stepKE); }
      else if ( stepParticle == "gamma" )
        { histoID = analysisManager->GetH1Id("gSpectra"); analysisManager->FillH1(histoID, stepKE); }
      else if ( stepCharge != 0 && ( stepParticle != "e-" && stepParticle != "proton" ) )
        { histoID = analysisManager->GetH1Id("oSpectra"); analysisManager->FillH1(histoID, stepKE); }
    }
  }

  // Acquire step r-position 
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

  // Declare vars
  G4double trackDepth = 0, trackDepthVertex = 0,
           stepRCuDepth = 0, stepZCuDepth = 0,
           stepRKADepth = 0, stepZKADepth = 0,
           stepRKADepthVertex = 0, stepZKADepthVertex = 0;

  // DESTINATION
  // particle enters copper, +q_i
  if ( volumeName == "Cu_cyl" ) {

    stepRCuDepth = stepR/r_Cu;
    stepZCuDepth = (stepZ + half_Cu)/h_Cu;
    netSignal += stepCharge;

/* 2015-11-15 Suppress analyses and extra models
    // Fill energy gain deposition Histo
    histoID = analysisManager->GetH2Id("energyDepHistoCu");
    analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth, eDep/(beamCharge*runEnergy));
*/
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

/* 2015-11-15 Suppress analyses and extra models
    // Fill energy gain deposition Histo
    histoID = analysisManager->GetH2Id("energyDepHistoKA");
    analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth, eDep/(beamCharge*runEnergy));
*/
  }

  // If end of track
  if ( step->GetTrack()->GetTrackStatus() != fAlive ) {
  
    // Get name of volume at track origin (vertex) w/ position
    G4String volumeNameVertex = step->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
    G4ThreeVector stepXYZVertex = step->GetTrack()->GetVertexPosition();
    G4double stepRVertex = pow(pow(stepXYZVertex[0],2) + pow(stepXYZVertex[1],2), 0.5);
    G4double stepZVertex = stepXYZVertex[2];
    
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
    
    // Append analysis gain
    simulationAnalysis->appendRunGain(netSignal/beamCharge);

    // If charge particle or neutron/gamma track is relevant to analysis
    //if ( ( stepCharge != 0 || ( stepParticle == "neutron"  || stepParticle == "gamma" ) ) ) {
    if ( stepCharge != 0 ) {

      // Azimuthal symmetry bin weighting factor // pi*(r_f^2 - r_i)^2
      G4double W_binCu = M_PI*(pow(floor(stepRCuDepth*100)+1, 2) - pow(floor(stepRCuDepth*100), 2));
      G4double W_binKA = M_PI*(pow(floor(stepRKADepth*100)+1, 2) - pow(floor(stepRKADepth*100), 2));

      // Fill gain profile 2D Histogram and var
      if ( stepCharge != 0 && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("gainDepHistoCu");
          analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth, (stepCharge/beamCharge)/W_binCu);
          simulationAnalysis->appendGainProfile(floor(stepZCuDepth*100), floor(stepRCuDepth*100), (stepCharge/beamCharge)/W_binCu); }
      if ( stepCharge != 0 && volumeNameVertex == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("gainDepHistoCu");
          analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth, -(stepCharge/beamCharge)/W_binCu);
          simulationAnalysis->appendGainProfile(floor(stepZCuDepth*100), floor(stepRCuDepth*100), -(stepCharge/beamCharge)/W_binCu); }
      if ( stepCharge != 0 && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("gainDepHistoKA");
          analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth, (stepCharge/beamCharge)/W_binKA); }
      if ( stepCharge != 0 && volumeNameVertex == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("gainDepHistoKA");
          analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth, -(stepCharge/beamCharge)/W_binKA); }

/* 2015-11-15 Suppress analyses and extra models
      // Copper/Kapton 2D Deposition Histogram
      if ( stepParticle == "e-" && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("eDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth); }
      else if ( stepParticle == "proton" && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("pDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth); }
      else if ( stepParticle == "neutron" && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("nDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth); }
      else if ( stepParticle == "gamma" && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("gDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth); }
      else if ( stepCharge != 0 && ( stepParticle != "e-" && stepParticle != "proton" ) && volumeName == "Cu_cyl" )
        { histoID = analysisManager->GetH2Id("oDepHistoCu"); analysisManager->FillH2(histoID, stepZCuDepth, stepRCuDepth); }

      else if ( stepParticle == "e-" && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("eDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth); }
      else if ( stepParticle == "proton" && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("pDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth); }
      else if ( stepParticle == "neutron" && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("nDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth); }
      else if ( stepParticle == "gamma" && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("gDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth); }
      else if ( stepCharge != 0 && ( stepParticle != "e-" && stepParticle != "proton" ) && volumeName == "Kapton_cyl1" )
        { histoID = analysisManager->GetH2Id("oDepHistoKA"); analysisManager->FillH2(histoID, stepZKADepth, stepRKADepth); }
*/
    }
  }
}
