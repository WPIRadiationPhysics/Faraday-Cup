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

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4int runID = run->GetRunID();
  G4String data_dir = "data/";

  // Construct Analysis (to-do: move to Analysis.cc)
  if ( runID%7 == 0 ) {
      // Create analysis manager
      analysisManager->SetVerboseLevel(1);

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
      analysisManager->FinishNtuple(0);
    }

  std::ostringstream ROOTfileNameStream;
  ROOTfileNameStream << data_dir << "rootData";
  G4String ROOTfileName = ROOTfileNameStream.str();
  analysisManager->OpenFile(ROOTfileName);

  // Primary thread
  if ( G4Threading::G4GetThreadId() == 0 ) {
    // Primary energy (run) number
    G4int E_Num = run->GetRunID();
    G4cout << "Running energy #" << E_Num << G4endl;
  }
}

void RunAction::EndOfRunAction(const G4Run* run) {

  // Save statistics
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // Vars, data and file structures
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  G4String fileVarGet;
  G4double runGain = 0; // Sum of event gains...
  G4double runGainAverage = 0; // ... averaged
  G4double runGainVar = 0; // variance of events gains
  G4double runGainError = 0; // SD of events gains
  G4int beamCharge = run->GetNumberOfEventToBeProcessed();
  G4int runID = run->GetRunID();
  std::ofstream dataFile;
  G4String data_dir = "data/";
  
  G4double pCuSignal = 0;
  G4double eCuSignal = 0;
  G4double otherCuSignal = 0;
  G4double pKASignal = 0;
  G4double eKASignal = 0;
  G4double otherKASignal = 0;
  G4double pCuSignalAverage = 0;
  G4double eCuSignalAverage = 0;
  G4double otherCuSignalAverage = 0;
  G4double pKASignalAverage = 0;
  G4double eKASignalAverage = 0;
  G4double otherKASignalAverage = 0;

  // Identify run file
  std::ostringstream rawRunFileName;
  rawRunFileName << data_dir << "run" << runID << "signals.txt";
  G4String runFileName = rawRunFileName.str();
    
  // Acquire net gain for run (already normalized)
  std::ifstream runFile(runFileName);
  while(runFile.good()) {
    getline(runFile, fileVarGet, ' '); pCuSignal += atof(fileVarGet);
    getline(runFile, fileVarGet, ' '); eCuSignal += atof(fileVarGet);
    getline(runFile, fileVarGet, ' '); otherCuSignal += atof(fileVarGet);
    getline(runFile, fileVarGet, ' '); pKASignal += atof(fileVarGet);
    getline(runFile, fileVarGet, ' '); eKASignal += atof(fileVarGet);
    getline(runFile, fileVarGet, ' '); otherKASignal += atof(fileVarGet);
    getline(runFile, fileVarGet); runGain += atof(fileVarGet);
  }
  
  runGainAverage = runGain/beamCharge;
  pCuSignalAverage = pCuSignal/beamCharge;
  eCuSignalAverage = eCuSignal/beamCharge;
  otherCuSignalAverage = otherCuSignal/beamCharge;
  pKASignalAverage = pKASignal/beamCharge;
  eKASignalAverage = eKASignal/beamCharge;
  otherKASignalAverage = otherKASignal/beamCharge;
  
  
  // Acquire standard deviation N events (reread run output)
  std::ifstream rerunFile(runFileName);
  while(rerunFile.good()) {
    for ( G4int signal_i=0; signal_i<6; signal_i++ ) { getline(rerunFile, fileVarGet, ' '); }
    getline(rerunFile, fileVarGet);
    runGainVar += pow(atof(fileVarGet) - runGainAverage, 2)/beamCharge;
  }
  runGainError = pow(runGainVar, 0.5);

  // Run summary
  std::ostringstream rawGainFileName;
  rawGainFileName << data_dir << "gain.txt";
  G4String gainFileName = rawGainFileName.str();
  std::ofstream gainFile;
  gainFile.open (gainFileName, std::ios::app);
  gainFile << energies[runID%7] << " " << pCuSignalAverage << " " << eCuSignalAverage << " " << otherCuSignalAverage << " " << pKASignalAverage << " " << eKASignalAverage << " " << otherKASignalAverage << " " << runGainAverage << " +/- " << runGainError << "\n";
  gainFile.close();

  if ( (runID+1)%7 ) {

    // Aquire ROOT data files
    std::ostringstream ROOTfileNameStream, ROOTgainFileNameStream;
    ROOTfileNameStream << data_dir << "rootData_t" << G4Threading::G4GetThreadId() << ".root";
    G4String ROOTfileName = ROOTfileNameStream.str();
    G4String ROOTgainFileName = ROOTgainFileNameStream.str();

    // Read ntuples and do same analysis, keep redundancy to test for plausibility
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    G4int ntupleId = analysisReader->GetNtuple("trackDat", ROOTfileName);

    G4double ROOT_gain[7] = {0};
    if ( ntupleId >= 0 ) {

      // Set ROOT vars
      G4int ROOT_eventID, ROOT_runID;
      G4double ROOT_particleCharge, ROOT_netCharge;
      G4double ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex;  
      analysisReader->SetNtupleIColumn("run", ROOT_runID);
      analysisReader->SetNtupleIColumn("event", ROOT_eventID);
      analysisReader->SetNtupleDColumn("particleCharge", ROOT_particleCharge);
      analysisReader->SetNtupleDColumn("r", ROOT_r);
      analysisReader->SetNtupleDColumn("z", ROOT_z);
      analysisReader->SetNtupleDColumn("rVertex", ROOT_rVertex);
      analysisReader->SetNtupleDColumn("zVertex", ROOT_zVertex);
      analysisReader->SetNtupleDColumn("netCharge", ROOT_netCharge);

      // Loop through collected values
      while ( analysisReader->GetNtupleRow(ntupleId) ) {
        ROOT_gain[ROOT_runID%7] += ROOT_netCharge;
      }
    }
    
    // Delete analysis reader 
    delete G4AnalysisReader::Instance();

    // Creating data ntuple 
    ROOTgainFileNameStream << data_dir << "gainData";
    ROOTgainFileName = ROOTgainFileNameStream.str();
    analysisManager->OpenFile(ROOTgainFileName);

    analysisManager->CreateNtuple("gainDat", "Gain Data");
    analysisManager->CreateNtupleDColumn("beamEnergy");
    analysisManager->CreateNtupleDColumn("beamGain");
    analysisManager->FinishNtuple(1);
  
    // print gain statistics
    for ( G4int run_i = 0; run_i < 7; run_i++ ) {
      analysisManager->FillNtupleDColumn(1, 0, energies[run_i]);
      analysisManager->FillNtupleDColumn(1, 1, ROOT_gain[run_i]);
      analysisManager->AddNtupleRow(1);
    }
    analysisManager->Write();
    analysisManager->CloseFile();
  }
}
