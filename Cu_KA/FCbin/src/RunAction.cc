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
  G4int runID = run->GetRunID();
  G4String data_dir = "data/";

  // Construct Track Data ntuple once per macro of runs
  if ( runID%7 == 0 ) {
	
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
