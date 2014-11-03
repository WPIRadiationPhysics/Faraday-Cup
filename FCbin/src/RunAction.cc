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
  G4RunManager::GetRunManager()->SetPrintProgress(1);
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
  dataFile.open(tallyFileName);
  dataFile << "# Variables aquired from steps of tracks" << "\n" << "charge state volume r z" << "\n";
  dataFile.close();
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
  char delim = ' ';
  G4double netCharge = 0;

  // Skip headers, gather run variables from data file
  getline(tallyFile, tallyFileHeader); getline(tallyFile, tallyFileHeader);
  while(tallyFile.good()) {
    getline(tallyFile, fileVarGet, delim); G4double stepCharge = atof(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4String stepState = fileVarGet;
    getline(tallyFile, fileVarGet, delim); G4String volumeName = fileVarGet;
    getline(tallyFile, fileVarGet, delim); G4double stepR = atof(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4double stepZ = atof(fileVarGet);

    /* Add/Remove charges which transport into/out-of the Cu
      Charges which are transported into/out-of a distance
      (del_r, del_z) into Kapton from cylinder have charge equivalence
      * 
            q*[1 - max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))]
    */
      
    // Initial state
    if ( stepState == "FROM" ) {
      // particle exits cylinder, -q_i
      if ( volumeName = "Cu_cyl" ) {
        netCharge -= stepCharge;
      }
      // particle exits Kapton, -q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
      if ( volumeName == "Kapton_cyl1" ) {
		// Something similar, account for all relevant cases!
        G4double percentR = (stepR - 500)/(550 - 500); // manually from DetectorConstruction.cc
        G4double percentZ = ((-stepZ) - 150)/(160 - 150); // cylinders are upside-down
	    G4double proportionalityCharge = ((percentR<percentZ)?percentZ:percentR); // concise maximum function
        netCharge -= stepCharge*proportionalityCharge;
      }
    }
    // Final state
    if ( stepState == "TO" ) {
      // particle enters cylinder, +q_i
      if ( volumeName == "Cu_cyl" ) {
        netCharge += stepCharge;
      }
      // particle enters Kapton, +q_i*max(del_r/(r_KA - r_Cu), del_z/(h_KA - h_Cu))
      if ( volumeName == "Kapton_cyl1" ) {
        G4double percentR = (stepR - 500)/(550 - 500); // manually from DetectorConstruction.cc
        G4double percentZ = (stepZ - (-150))/((-160) - (-150)); // cylinders are upside-down
	    G4double proportionalityCharge = ((percentR<percentZ)?percentZ:percentR); // concise maximum function
        netCharge += stepCharge*proportionalityCharge;
      }
    }
  }

  // Summarize particle transportation
  dataFile.open(chargeFileName, std::ios::app);
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
