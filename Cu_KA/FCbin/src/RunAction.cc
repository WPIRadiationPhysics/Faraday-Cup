#include "Analysis.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Analysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

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

  // Construct Analysis vars
  std::ostringstream ROOTfileNameStream; G4String ROOTfileName;

  if ( runID%7 == 0 ) {

    // Creating data ntuple 
    analysisManager->CreateNtuple("trackDat", "Track Data");
    analysisManager->CreateNtupleIColumn("run");
    analysisManager->CreateNtupleIColumn("event");
    analysisManager->CreateNtupleDColumn("particleCharge");
    analysisManager->CreateNtupleDColumn("r");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("rVertex");
    analysisManager->CreateNtupleDColumn("zVertex");
    analysisManager->CreateNtupleDColumn("netCharge");
    analysisManager->FinishNtuple();

    ROOTfileNameStream << data_dir << "rootData";
    ROOTfileName = ROOTfileNameStream.str();
    analysisManager->OpenFile(ROOTfileName);
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
