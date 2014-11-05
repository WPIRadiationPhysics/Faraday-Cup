#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>

RunAction::RunAction() : G4UserRunAction() { 
  // set printing event number per each event
  //G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* /*run*/) {
  // Create tally and transport files
  G4String data_dir = "data/"; G4String dirCommand = "mkdir -p " + data_dir;
  system(dirCommand);
  G4String tallyFileName = data_dir + "tallies.txt";
  G4String chargeFileName = data_dir + "charge.txt";
  std::ofstream dataFile;

  // Initialize tally file
  dataFile.open(tallyFileName); dataFile.close();
  // Initialize net charge file
  dataFile.open(chargeFileName); dataFile.close();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {
  // Vars, data and file structures
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  G4String chargeFileName = data_dir + "charge.txt";
  G4String gnuplotFileName = data_dir + "histo.gnuplot";
  std::ifstream tallyFile(tallyFileName);
  std::ofstream dataFile;
  G4String fileVarGet, tallyFileHeader;
  G4double netCharge = 0;

  // Sum net charge from each transport (IN/OUT)
  while(tallyFile.good()) {
    getline(tallyFile, fileVarGet, ' '); G4double q_i = atof(fileVarGet);
    getline(tallyFile, fileVarGet, ' '); // particle name
    getline(tallyFile, fileVarGet); // particle volume
    netCharge += q_i;
  }

  // Summarize particle transportation
  dataFile.open(chargeFileName);
  dataFile << "The net charge of the irradiated copper is " << netCharge << " e" << "\n";
  dataFile.close();

  /*// Create gnuplot file for analysis
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
