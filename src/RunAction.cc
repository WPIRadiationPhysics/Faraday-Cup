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

RunAction::RunAction() : G4UserRunAction() {

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create percentile particle gain histograms once per energy
  analysisManager->CreateH2("eGainHistoCu", "eGainHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pGainHistoCu", "pGainHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oGainHistoCu", "oGainHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nGainHistoCu", "nGainHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gGainHistoCu", "gGainHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eGainHistoKA", "eGainHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pGainHistoKA", "pGainHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oGainHistoKA", "oGainHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nGainHistoKA", "nGainHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gGainHistoKA", "gGainHistoKA", 100, 0., 1., 100, 0., 1.);

  // Creating cascade cascade info ntuple
  analysisManager->CreateNtuple("cascadeData", "Cascade Data");
  analysisManager->CreateNtupleIColumn(0, "particleType");
  analysisManager->CreateNtupleDColumn(0, "r");
  analysisManager->CreateNtupleDColumn(0, "z");
  analysisManager->CreateNtupleDColumn(0, "eDep_r");
  analysisManager->CreateNtupleDColumn(0, "eDep_z");
  analysisManager->FinishNtuple(0);


}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* run) {

  G4int runID = run->GetRunID();
  G4int threadID = G4Threading::G4GetThreadId();

  // Get Analysis manager and instance
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  G4double runEnergy = simulationAnalysis->GetRunEnergy();

  // Acquire and create model's analysis directory
  G4String data_dir = simulationAnalysis->GetAnalysisDIR();
  G4String syscmd = "mkdir -p " + data_dir; system(syscmd);

  /*
  // Construct data ntuples once per macro of runs
  if ( runID%7 == 0 ) {
	
    // Creating gain track info ntuple 
    analysisManager->CreateNtuple("gainData", "Gain Data");
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

    // Creating cascade cascade info ntuple
    analysisManager->CreateNtuple("cascadeData", "Cascade Data");
    analysisManager->CreateNtupleIColumn(1, "run");
    analysisManager->CreateNtupleIColumn(1, "particleType");
    analysisManager->CreateNtupleDColumn(1, "r");
    analysisManager->CreateNtupleDColumn(1, "z");
    analysisManager->CreateNtupleDColumn(1, "eDep_r");
    analysisManager->CreateNtupleDColumn(1, "eDep_z");
    analysisManager->FinishNtuple(1);
  }
  */

  // Open simulation data file for writing
  analysisManager->OpenFile(data_dir+"trackData");

  // Primary thread
  if ( threadID == 0 ) {

    // Primary energy (run) number
    G4cout << "Running proton beam at " << runEnergy << " MeV..." << G4endl;
  }
}

void RunAction::EndOfRunAction(const G4Run* run) {

  // Acquire runID and analysis object
  G4int runID = run->GetRunID();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  G4double runEnergy = simulationAnalysis->GetRunEnergy();

  // Constant vars
  G4String data_dir = simulationAnalysis->GetAnalysisDIR(),
           syscmd;
  G4int threadID = G4Threading::G4GetThreadId();

  //if ( threadID == -1 ) {

  // Acquire Analysis Manager, write and delete
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // Acquire current gain
  G4double runGain = simulationAnalysis->recallRunGain();

  // Print run gain to output
  G4cout << "Gain measurement for beam energy of " << runEnergy << " MeV: " << runGain << G4endl;

  // Save statistics per energy macro
  //if ( (runID+1)%7 == 0 ) {

  //}

  //}
}
