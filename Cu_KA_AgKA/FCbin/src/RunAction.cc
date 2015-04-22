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
    analysisManager->CreateNtupleDColumn(0, "rVertex");
    analysisManager->CreateNtupleDColumn(0, "zVertex");
    analysisManager->CreateNtupleDColumn(0, "netCharge");
    analysisManager->CreateNtupleIColumn(0, "signalType");
    analysisManager->CreateNtupleDColumn(0, "trackDepth");
    analysisManager->CreateNtupleDColumn(0, "trackDepthVertex");
    analysisManager->FinishNtuple(0);
    
    // Open simulation data file for writing
    analysisManager->OpenFile(data_dir+"rootData");
  }

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
