#include "RunAction.hh"
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
  // Create data directory if none existing // feature: delete existing data directory
  G4String data_dir = "data/"; G4String dirCommand = "mkdir -p " + data_dir;
  system(dirCommand);
  
  G4int runID = run->GetRunID();
}

void RunAction::EndOfRunAction(const G4Run* run) {
  // Vars, data and file structures
  G4String fileVarGet;
  G4double netCharge = 0;
  G4int runID = run->GetRunID();
  std::ofstream dataFile;
  
  // Beam info
  G4int beamCharge = run->GetNumberOfEventToBeProcessed();

  // Construct filenames
  G4String data_dir = "data/";
  std::ostringstream rawTallyFileName;
  rawTallyFileName << data_dir << "tallies" << runID << ".txt";
  G4String tallyFileName = rawTallyFileName.str();
  G4String yieldFileName = data_dir + "yield.txt";
  G4String gnuplotFileName = data_dir + "histo.gnuplot";
    
  // Probe tallies of run
  std::ifstream tallyFile(tallyFileName);
      
  // Sum net charge from each transport (IN/OUT)
  while(tallyFile.good()) {
    getline(tallyFile, fileVarGet, ' '); G4double q_i = atof(fileVarGet);
    getline(tallyFile, fileVarGet, ' '); // particle name
    getline(tallyFile, fileVarGet); // particle volume
    netCharge += q_i;
  }
  
  // Perform gain calculation
  G4double faradayCupGain = netCharge/beamCharge;

  // Summarize particle transportation
  // Explicitly ignoring run#6+, not sure where they're coming from
  dataFile.open(yieldFileName, std::ios::app);
  dataFile << runID << " " << (faradayCupGain-1) << "\n";
  G4cout << "Run #" << runID << " produces SEY of " << (faradayCupGain-1) << G4endl;
  dataFile.close();
  
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
