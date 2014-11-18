#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/types.h>

RunAction::RunAction() : G4UserRunAction() { 
  // set printing event number per each event
  //G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* /*run*/) {
  // Energies directly from run.mac
  G4int numRuns = 6; G4int i;
  
  // Create tally sheet in order for each energy
  G4String data_dir = "data/"; G4String dirCommand = "mkdir -p " + data_dir;
  system(dirCommand);
  std::ofstream dataFile;

  /*
   *  Problem is somewhere around here, will create tallies[0-5].txt and
   *  populate the charge file respectively.
  */
  for ( i=0; i<numRuns; i++) {
	// Construct filenames
	using namespace std;
	stringstream ss << data_dir << "tallies" << i << ".txt";
	G4String tallyFileName = ss.str();
    
    // Check if tally file made (in previous run)
    ifstream infile(tallyFileName);
    if ( infile.good() ) { continue; }
    else {      
      // Initialize tally file
      dataFile.open(tallyFileName); dataFile.close();
    }
  }
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {
  // Vars, data and file structures
  G4String data_dir = "data/";
  G4String gnuplotFileName = data_dir + "histo.gnuplot";
  std::ofstream dataFile;
  G4String fileVarGet, tallyFileHeader;
  G4double netCharge = 0;
  G4int i, numRuns = 6;
  G4double run_energies[numRuns] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06];

  // Skip to current analysis
  for ( i=0; i<numRuns; i++) {
	// Construct filenames
	using nsamespace std;
	stringstream ss << data_dir << "charge" << i << ".txt";
	GString chargeFileName = ss.str();
    
    // Check if charge file made (in previous analysis)
    ifstream infile(chargeFileName);
    if ( infile.good() ) { continue; }
    else {
	  // Probe tallies of run
	  ss << data_dir << "tallies" << i << ".txt";
      G4String tallyFileName = ss.str();
      std::ifstream tallyFile(tallyFileName);
      
      // Sum net charge from each transport (IN/OUT)
      while(tallyFile.good()) {
        getline(tallyFile, fileVarGet, ' '); G4double q_i = atof(fileVarGet);
        getline(tallyFile, fileVarGet, ' '); // particle name
        getline(tallyFile, fileVarGet); // particle volume
        netCharge += q_i;
      }

      // Summarize particle transportation
      dataFile.open(chargeFileName);
      dataFile << run_energies[i] << " " << netCharge << "e" << "\n";
      dataFile.close();
    }
    
    // Make gnuplot macro after final run
    //if ( i == 6 ) {
	//
    //}
  }



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
