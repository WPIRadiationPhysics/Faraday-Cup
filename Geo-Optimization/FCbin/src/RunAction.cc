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
  std::ostringstream NtupleNameStream, ROOTfileNameStream;
  G4String NtupleName, ROOTfileName;

  if ( runID%7 == 0 ) {
    // Create analysis manager
    analysisManager->SetVerboseLevel(2);

    NtupleNameStream << "trackDat"; // << floor(runID/7);
    NtupleName = NtupleNameStream.str();
    // Creating data ntuple 
    analysisManager->CreateNtuple(NtupleName, "Track Data");
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

  if ( (runID+1)%7 == 0 ) {
    // Save statistics
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    delete G4AnalysisManager::Instance();

  }

  // Final thread: any thread can finish first, so check run state file for nThreads of '#'
  // Add '#' per thread
  G4String threadString, threadFileName;
  std::ostringstream threadFileNameStream;
  std::ofstream threadFileOutStream; std::ifstream threadFileInStream;
  threadFileNameStream << data_dir << ".thread";
  threadFileName = threadFileNameStream.str();
  threadFileOutStream.open(threadFileName, std::ios::app);
  threadFileOutStream << "#";
  threadFileOutStream.close();
  // Reopen, get total length
  threadFileInStream.open(threadFileName);
  threadFileInStream >> threadString;
  threadFileInStream.close();

  // Acquire nThreadsString
  std::ostringstream nThreadsFileNameStream; G4String nThreadsFileName, nThreadsString;
  nThreadsFileNameStream << data_dir << ".nThreads"; nThreadsFileName = nThreadsFileNameStream.str();
  std::ifstream nThreadsFileStream (nThreadsFileName);
  nThreadsFileStream >> nThreadsString; nThreadsFileStream.close();

  // If final worker thread (not workerID==nThreads, but the final being completed [future work: thread merge class via G4MTRunManager])
  if ( threadString == nThreadsString ) {

    // reset threads file
    threadFileOutStream.open(threadFileName);
    threadFileOutStream << "";
    threadFileOutStream.close();
  
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

    // If final run in execution
    if ( (runID+1)%7 == 0 ) {

    }
  }
}
