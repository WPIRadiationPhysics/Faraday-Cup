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

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* run) {
  // Create analysis manager
  G4String data_dir = "data/";
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  
  // Create data filenames
  std::ostringstream ROOTfileNameStream, ROOTgainFileNameStream;
  ROOTfileNameStream << data_dir << "rootData";
  ROOTgainFileNameStream << data_dir << "gainData";
  G4String ROOTfileName = ROOTfileNameStream.str();
  G4String ROOTgainFileName = ROOTgainFileNameStream.str();

  // Open ROOT data file
  analysisManager->OpenFile(ROOTfileName);

  // Creating data ntuple 
  analysisManager->CreateNtuple("trackDat", "Track Data");
  analysisManager->CreateNtupleIColumn("run");
  analysisManager->CreateNtupleIColumn("event");
  analysisManager->CreateNtupleDColumn("particleCharge");
  analysisManager->CreateNtupleDColumn("r");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("rVertex");
  analysisManager->CreateNtupleDColumn("zVertex");
  analysisManager->CreateNtupleDColumn("trackCharge");
  analysisManager->FinishNtuple();

  // Primary thread
  if ( G4Threading::G4GetThreadId() == 0 ) {
    // Primary energy (run) number
    G4int E_Num = run->GetRunID();
    G4cout << "Running energy #" << E_Num << G4endl;
  }
}

void RunAction::EndOfRunAction(const G4Run* run) {

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
  gainFile << energies[runID%12] << " " << pCuSignalAverage << " " << eCuSignalAverage << " " << otherCuSignalAverage << " " << pKASignalAverage << " " << eKASignalAverage << " " << otherKASignalAverage << " " << runGainAverage << " +/- " << runGainError << "\n";
  gainFile.close();

  G4int nofEvents = run->GetNumberOfEvent();
  if ( nofEvents == 0 ) return;

  // Save statistics
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // Aquire ROOT data files
  std::ostringstream ROOTfileNameStream, ROOTgainFileNameStream;
  ROOTfileNameStream << data_dir << "rootData";
  ROOTgainFileNameStream << data_dir << "gainData";
  G4String ROOTfileName = ROOTfileNameStream.str();
  G4String ROOTgainFileName = ROOTgainFileNameStream.str();

  // Analyze ntuple of track info
  G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
  analysisReader->SetFileName(ROOTfileName);
  
  // Read ntuples and do same analysis, keep redundancy to test for plausibility
  G4double ROOT_gain = 0;
  G4int ntupleId = analysisReader->GetNtuple("trackDat");
  if ( ntupleId >= 0 ) {

    // Set ROOT vars
    G4int ROOT_eventID;
    G4double ROOT_particleCharge, ROOT_trackCharge;
    G4double ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex;  
    analysisReader->SetNtupleIColumn(1, ROOT_eventID);
    analysisReader->SetNtupleDColumn(2, ROOT_particleCharge);
    analysisReader->SetNtupleDColumn(3, ROOT_r);
    analysisReader->SetNtupleDColumn(4, ROOT_z);
    analysisReader->SetNtupleDColumn(5, ROOT_rVertex);
    analysisReader->SetNtupleDColumn(6, ROOT_zVertex);
    analysisReader->SetNtupleDColumn(7, ROOT_trackCharge);

    // Loop through collected values
    while ( analysisReader->GetNtupleRow() ) { ROOT_gain += ROOT_trackCharge/beamCharge; }
  }

  // Delete analysis reader 
  delete G4AnalysisReader::Instance();

  // Creating data ntuple 
  analysisManager->OpenFile(ROOTgainFileName);
  analysisManager->CreateNtuple("gainDat", "Gain Data");
  analysisManager->CreateNtupleDColumn("beamEnergy");
  analysisManager->CreateNtupleDColumn("beamGain");
  analysisManager->FinishNtuple();
  
  // print gain statistics
  G4double ROOT_beamEnergy[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  analysisManager->FillNtupleDColumn(1, 0, ROOT_beamEnergy[runID]);
  analysisManager->FillNtupleDColumn(1, 1, ROOT_gain);
  analysisManager->AddNtupleRow(1);
  
  // save statistics
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // complete cleanup
  delete G4AnalysisManager::Instance();
}
