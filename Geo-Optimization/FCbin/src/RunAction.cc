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
void RunAction::BeginOfRunAction(const G4Run* /*run*/) {}

void RunAction::EndOfRunAction(const G4Run* run) {
  // Vars, data and file structures
  G4String fileVarGet;
  G4double runGain = 0; // Sum of event gains...
  G4double runGainAverage = 0; // ... averaged
  G4int beamCharge = run->GetNumberOfEventToBeProcessed();
  G4int runID = run->GetRunID();
  std::ofstream dataFile;

  // Identify run file
  G4String data_dir = "data/";
  std::ostringstream rawRunFileName;
  rawRunFileName << data_dir << "run" << runID << "signals.txt";
  G4String runFileName = rawRunFileName.str();
  //G4String gnuplotFileName = data_dir + "histo.gnuplot";
    
  // Acquire net gain for run (already normalized)
  std::ifstream runFile(runFileName);
  while(runFile.good()) {
    getline(runFile, fileVarGet);
    runGain += atof(fileVarGet);
  }
  runGainAverage = runGain/beamCharge;

  // Signal error calculation
  G4double signal_error = (1 - runGainAverage)*100.000;

  // Run summary
  std::ostringstream rawGainFileName;
  rawGainFileName << data_dir << "error.txt";
  G4String gainFileName = rawGainFileName.str();
  std::ofstream gainFile;
  gainFile.open (gainFileName, std::ios::app);
  gainFile << signal_error << "\n";
  gainFile.close();

  //G4cout << "Run #" << runID << " produces differential Gain (I/B) of " << runGainAverage << " +/- " << runGainError << G4endl;
}
