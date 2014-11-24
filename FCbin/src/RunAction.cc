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

RunAction::RunAction() : G4UserRunAction() { 
  // set printing event number per each event
  //G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* /*run*/) {
  // Vars and structures
  G4String data_dir = "data/"; G4String dirCommand = "mkdir -p " + data_dir;
  G4String tallyFileName = data_dir + "tallies.txt";
  system(dirCommand);
  std::ofstream dataFile;
    
  // Check if tally file made (in previous run)
  std::ifstream infile(tallyFileName);
  if ( infile.good() ) { remove(tallyFileName); }
  // Initialize
  dataFile.open(tallyFileName); dataFile.close();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {
  // Vars, data and file structures
  G4String fileVarGet; char c;
  G4double netCharge = 0;
  G4int analysisNum;
  std::ofstream dataFile;
  
  // Beam info (directly from macro file)
  G4double run_energies[9] = {5, 7, 9, 11, 13, 15, 17, 17.5, 18};
  G4double beamCharge = 100000.0;

  // Construct filenames
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  G4String chargeFileName = data_dir + "charge.txt";
  G4String gnuplotFileName = data_dir + "histo.gnuplot";
    
  // Find analysis number by seeing how many lines made it into charge.txt
  G4int numlines = 0;
  std::ifstream infile(chargeFileName);
  infile.get(c);
  // Primitive line-counter
  while (infile) {
    while (infile && c != '\n') { infile.get(c); }
    numlines = numlines + 1;
    infile.get(c);
  }
  analysisNum = numlines;
    
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
  dataFile.open(chargeFileName, std::ios::app);
  if ( analysisNum < 9 ) {
	std::ostringstream o;
	o << run_energies[analysisNum];
    dataFile << o.str() << " " << faradayCupGain << "\n";
  }
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
