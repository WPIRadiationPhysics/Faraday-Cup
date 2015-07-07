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

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create percentile particle deposition histograms
  analysisManager->CreateH2("eDepHistoCu", "eDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pDepHistoCu", "pDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oDepHistoCu", "oDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nDepHistoCu", "nDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gDepHistoCu", "gDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eDepHistoKA", "eDepHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pDepHistoKA", "pDepHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oDepHistoKA", "oDepHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nDepHistoKA", "nDepHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gDepHistoKA", "gDepHistoKA", 100, 0., 1., 100, 0., 1.);
  // Create percentile particle total gain and energy histograms
  analysisManager->CreateH2("gainDepHistoCu", "gainDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gainDepHistoKA", "gainDepHistoKA", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("energyDepHistoCu", "energyDepHistoCu", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("energyDepHistoKA", "energyDepHistoKA", 100, 0., 1., 100, 0., 1.);

  // Create particle energy Spectra histograms
  analysisManager->CreateH1("eSpectra", "eSpectra", 100, 0., 1*MeV);
  analysisManager->CreateH1("pSpectra", "pSpectra", 100, 0., 100*MeV);
  analysisManager->CreateH1("oSpectra", "oSpectra", 100, 0., 100*MeV);
  analysisManager->CreateH1("nSpectra", "nSpectra", 100, 0., 100*MeV);
  analysisManager->CreateH1("gSpectra", "gSpectra", 100, 0., 10*MeV);
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* run) {

  // Acquire runID, and declare vars
  G4int runID = run->GetRunID();
  G4String trackDataFileName; std::ostringstream trackDataFileNameStream;

  // Get Analysis manager and instance
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();
  G4double runEnergy = simulationAnalysis->GetRunEnergy();
  simulationAnalysis->SetRunID(runID);

  // Acquire and create model's analysis directory
  G4String data_dir = simulationAnalysis->GetAnalysisDIR();
  G4String syscmd = "mkdir -p " + data_dir; system(syscmd);
  
  // Open simulation data file for writing
  G4int nEnergies = simulationAnalysis->GetNEnergies();
  trackDataFileNameStream << data_dir << "trackData-" << runID%nEnergies;
  trackDataFileName = trackDataFileNameStream.str();
  analysisManager->SetFileName(trackDataFileName);
  analysisManager->OpenFile();

  // Primary energy
  if ( isMaster ) { G4cout << "Running proton beam at " << runEnergy << " MeV..." << G4endl; }
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {

  // Acquire Analysis Manager, write and close
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
