#include "Analysis.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

RunAction::RunAction() : G4UserRunAction() {}
RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {

  // Get Analysis Manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //analysisManager->SetVerboseLevel(10);
  G4int runID = run->GetRunID();
  G4String data_dir = "data/";

  // Construct Track Data ntuple once per macro of runs
  if ( runID%7 == 0 ) {
	
	// Print geometry info
	G4Tubs* Kapton_cyl1Tubs = 0; G4Tubs* Cu_cylTubs = 0;
	G4double Kapton_cyl1ZHalfLength = 0, Cu_cylZHalfLength = 0;
	
	G4LogicalVolume* Kapton_cyl1LV = G4LogicalVolumeStore::GetInstance()->GetVolume("Kapton_cyl1");
	if ( Kapton_cyl1LV ) Kapton_cyl1Tubs = dynamic_cast< G4Tubs*>(Kapton_cyl1LV->GetSolid());
	if ( Kapton_cyl1Tubs ) { Kapton_cyl1ZHalfLength = Kapton_cyl1Tubs->GetZHalfLength(); }
	G4LogicalVolume* Cu_cylLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Cu_cyl");
	if ( Cu_cylLV ) Cu_cylTubs = dynamic_cast< G4Tubs*>(Cu_cylLV->GetSolid());
	if ( Cu_cylTubs ) { Cu_cylZHalfLength = Cu_cylTubs->GetZHalfLength(); }
	
	G4double KA_thickness = (Kapton_cyl1ZHalfLength - Cu_cylZHalfLength);
	G4cout << "KA thickness is " << KA_thickness << G4endl;
	
    // Creating Track info ntuple 
    analysisManager->CreateNtuple("trackDat", "Track Data");
    analysisManager->CreateNtupleIColumn(0, "run");
    analysisManager->CreateNtupleIColumn(0, "event");
    analysisManager->CreateNtupleDColumn(0, "particleCharge");
    analysisManager->CreateNtupleDColumn(0, "r");
    analysisManager->CreateNtupleDColumn(0, "z");
    analysisManager->CreateNtupleDColumn(0,"rVertex");
    analysisManager->CreateNtupleDColumn(0, "zVertex");
    analysisManager->CreateNtupleDColumn(0, "netCharge");
    analysisManager->FinishNtuple(0);
    
    // Open simulation data file for writing
    analysisManager->OpenFile(data_dir+"rootData");
  }

  // Create Kapton capture statistics ntuple every run
  std::ostringstream KAntupleNameStream; KAntupleNameStream << "transKA_IN" << (runID%7);
  G4String KAntupleName = KAntupleNameStream.str();
  analysisManager->CreateNtuple(KAntupleName, "transKA_IN Data");
  analysisManager->CreateNtupleDColumn((runID%7)+1, "trans_zDepthPercent");
  analysisManager->FinishNtuple((runID%7)+1);
  
  KAntupleNameStream.str(""); KAntupleNameStream << "transKA_OUT" << (runID%7);
  KAntupleName = ""; KAntupleName = KAntupleNameStream.str();
  analysisManager->CreateNtuple(KAntupleName, "transKA_OUT Data");
  analysisManager->CreateNtupleDColumn((runID%7)+2, "trans_zDepthPercent");
  analysisManager->FinishNtuple((runID%7)+2);
  
  KAntupleNameStream.str(""); KAntupleNameStream << "cisKA_IN" << (runID%7);
  KAntupleName = ""; KAntupleName = KAntupleNameStream.str();
  analysisManager->CreateNtuple(KAntupleName, "cisKA_IN Data");
  analysisManager->CreateNtupleDColumn((runID%7)+3, "cis_zDepthPercent");
  analysisManager->FinishNtuple((runID%7)+3);
  
  KAntupleNameStream.str(""); KAntupleNameStream << "cisKA_OUT" << (runID%7);
  KAntupleName = ""; KAntupleName = KAntupleNameStream.str();
  analysisManager->CreateNtuple(KAntupleName, "cisKA_OUT Data");
  analysisManager->CreateNtupleDColumn((runID%7)+4, "cis_zDepthPercent");
  analysisManager->FinishNtuple((runID%7)+4);
    
  // Primary thread
  if ( G4Threading::G4GetThreadId() == 0 ) {
    // Primary energy (run) number
    G4int E_Num = run->GetRunID();
    G4cout << "Running energy #" << E_Num << G4endl;
  }
}

void RunAction::EndOfRunAction(const G4Run* run) {

  // Vars, data and file structures
  G4String fileVarGet;
  G4int runID = run->GetRunID();

  // Save statistics per energy macro
  if ( (runID+1)%7 == 0 ) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    delete G4AnalysisManager::Instance();
  }
}
