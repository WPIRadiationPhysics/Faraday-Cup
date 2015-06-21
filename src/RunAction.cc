#include "Analysis.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

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

  // Construct data ntuples once per macro of runs
  if ( runID%7 == 0 ) {
	
    // Creating track info ntuple 
    analysisManager->CreateNtuple("trackData", "Track Data");
    analysisManager->CreateNtupleIColumn(0, "run");
    analysisManager->CreateNtupleIColumn(0, "event");
    analysisManager->CreateNtupleDColumn(0, "particleCharge");
    analysisManager->CreateNtupleDColumn(0, "r");
    analysisManager->CreateNtupleDColumn(0, "z");
    analysisManager->CreateNtupleDColumn(0, "rVertex");
    analysisManager->CreateNtupleDColumn(0, "zVertex");
    analysisManager->CreateNtupleDColumn(0, "rCuDepth");
    analysisManager->CreateNtupleDColumn(0, "zCuDepth");
    analysisManager->CreateNtupleDColumn(0, "rCuDepthVertex");
    analysisManager->CreateNtupleDColumn(0, "zCuDepthVertex");
    analysisManager->CreateNtupleDColumn(0, "rKADepth");
    analysisManager->CreateNtupleDColumn(0, "zKADepth");
    analysisManager->CreateNtupleDColumn(0, "rKADepthVertex");
    analysisManager->CreateNtupleDColumn(0, "zKADepthVertex");
    analysisManager->CreateNtupleDColumn(0, "netCharge");
    analysisManager->CreateNtupleIColumn(0, "signalType");
    analysisManager->FinishNtuple(0);

    // Creating cascade info ntuple
    analysisManager->CreateNtuple("cascadeData", "Cascade Data");
    analysisManager->CreateNtupleIColumn(1, "run");
    analysisManager->CreateNtupleIColumn(1, "particleType");
    analysisManager->CreateNtupleDColumn(1, "x");
    analysisManager->CreateNtupleDColumn(1, "y");
    analysisManager->CreateNtupleDColumn(1, "z");
    analysisManager->CreateNtupleDColumn(1, "p_x");
    analysisManager->CreateNtupleDColumn(1, "p_y");
    analysisManager->CreateNtupleDColumn(1, "p_z");
    analysisManager->FinishNtuple(1);

    // Open simulation data file for writing
    analysisManager->OpenFile(data_dir+"signalTracks");
  }

  // Primary thread
  if ( G4Threading::G4GetThreadId() == 0 ) {

    // Primary energy (run) number
    G4cout << "Running energy #" << runID << G4endl;
  }
}

void RunAction::EndOfRunAction(const G4Run* run) {

  // Vars, data and file structures
  G4int runID = run->GetRunID();

  // Save statistics per energy macro
  if ( (runID+1)%7 == 0 ) {

    // Acquire Analysis Manager, write and delete
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    delete G4AnalysisManager::Instance();
  }
}
