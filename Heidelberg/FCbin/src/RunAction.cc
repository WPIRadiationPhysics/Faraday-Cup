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
  G4double runGainVar = 0; // variance of events gains
  G4double runGainError = 0; // SD of events gains
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
  // Acquire standard deviation N events (reread run output)
  std::ifstream rerunFile(runFileName);
  while(rerunFile.good()) {
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
  gainFile << runID << " " << runGainAverage << " +/- " << runGainError << "\n";
  gainFile.close();

  G4cout << "Run #" << runID << " produces differential Gain (I/B) of " << runGainAverage << " +/- " << runGainError << G4endl;
   
  /*// Create gnuplot file for analysis after final run
  dataFile.open(gnuplotFileName);
  dataFile << "set term png" << "\n"
           << "set output \"histo.png\"" << "\n\n"

           << "#set style data histogram" << "\n"
           << "#set style histogram cluster gap 1" << "\n"
           << "set boxwidth 0.75 absolute" << "\n"
           << "set style fill solid border -1" << "\n\n"

           << "set xrange [0:80]" << "\n"
           << "set xlabel \"Depth (mm)\"" << "\n"
           << "set yrange [-100:]" << "\n"
           << "set ylabel \"# x e (Coulombs)\"" << "\n\n"

           << "plot \"transports.txt\" u 1:2 w boxes notitle";
  dataFile.close(); */
}
