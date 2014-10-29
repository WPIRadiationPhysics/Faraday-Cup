// $Id: RunAction.cc 75215 2013-10-29 16:07:06Z gcosmo $
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>

RunAction::RunAction() : G4UserRunAction() { 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run* /*run*/) {
  // Create tally and transport files
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  std::ofstream dataFile;

  // Initialize tally file
  dataFile.open(tallyFileName);
  dataFile << "# Variables aquired from steps of tracks" << "\n" << "# track particle charge step volume z energy edep" << "\n";
  dataFile.close();
}

void RunAction::EndOfRunAction(const G4Run* /*run*/) {
  G4cout << "Categorizing data movement..." << "\n";

  // Vars, data and file structures
  G4String data_dir = "data/";
  G4String tallyFileName = data_dir + "tallies.txt";
  G4String transFileName = data_dir + "transports.txt";
  G4String gnuplotFileName = data_dir + "histo.gnuplot";
  std::ifstream tallyFile(tallyFileName);
  std::ofstream dataFile;
  G4String fileVarGet, tallyFileHeader;
  G4String volumeInit[10000];
  G4double zInit[10000];
  char delim = ' ';

  const G4int num_bins = 150;
  G4double bin_charge[num_bins] = {0};
  G4int bin_initial, bin_final;

  //G4double netCharge = 0;

  // Initialize transport entry file
  dataFile.open(transFileName); dataFile.close();

  // Skip headers, gather run variables from data file
  getline(tallyFile, tallyFileHeader); getline(tallyFile, tallyFileHeader);
  while(tallyFile.good()) {
    getline(tallyFile, fileVarGet, delim); G4int trackID = atoi(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4String stepParticle = fileVarGet;
    getline(tallyFile, fileVarGet, delim); G4double stepCharge = atof(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4int stepNum = atoi(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4String volumeName = fileVarGet;
    getline(tallyFile, fileVarGet, delim); G4double stepZ = atof(fileVarGet);
    getline(tallyFile, fileVarGet, delim); G4double kinEnergy = atof(fileVarGet);
    getline(tallyFile, fileVarGet); G4double edep = atof(fileVarGet);

    //// THIS SETUP CREATES BRAGG PEAK DATA HISTO DATA
    //// 1 mm spacing
    // Initial state
    if ( stepNum == 1 ) {
      volumeInit[trackID] = volumeName;
      zInit[trackID] = stepZ;
    }
    // Final state
    if ( kinEnergy == 0 ) { //100 eV
      // particle enters cylinder, add charge at location
      if ( volumeName == "Cu_cyl" && volumeInit[trackID] != "Cu_cyl" ) {
        for ( int k=0; k<num_bins; k++) {
          if ( stepZ > k - 150 )
            bin_final = k;
        }
        bin_charge[bin_final] += stepCharge;
      }
      // particle exits cylinder, subtract charge
      if ( volumeName != "Cu_cyl" && volumeInit[trackID] == "Cu_cyl" ) {
        for ( int k=0; k<num_bins; k++) {
          if ( zInit[trackID] > k - 150 )
            bin_initial = k;
        }
        bin_charge[bin_initial] -= stepCharge;
      }
      // particle moves from one bin to another within copper
      if ( volumeName == "Cu_cyl" && volumeInit[trackID] == "Cu_cyl" ) {
        for ( int k=0; k<num_bins; k++) {
          if ( zInit[trackID] > k - 150 )
            bin_initial = k;
          if ( stepZ > k - 150 )
            bin_final = k;
        }
        if ( bin_initial != bin_final ) {
          bin_charge[bin_initial] -= stepCharge;
          bin_charge[bin_final] += stepCharge;
        }
      }
    }

    ////// THIS SETUP CREATES GENERAL TRANSPORT FILE
    //// Initial state
    //if ( stepNum == 1 )
    //  volumeInit[trackID] = volumeName;
    //// Final state
    //if ( kinEnergy == 0 ) {
    //  // particle enters cylinder, add charge
    //  if ( volumeName == "Cu_cyl" && volumeInit[trackID] != "Cu_cyl" ) {
    //    netCharge += stepCharge;
    //    dataFile.open(transFileName, std::ios::app);
    //    dataFile << stepParticle << " (" << stepCharge << ") enters copper."  
    //             << " Net charge: " << netCharge << "\n";
    //    dataFile.close();
    //  }
    //  // particle exits cylinder, subtract charge
    //  if ( volumeName != "Cu_cyl" && volumeInit[trackID] == "Cu_cyl" ) {
    //    netCharge -= stepCharge;
    //    dataFile.open(transFileName, std::ios::app);
    //    dataFile << stepParticle << "(" << stepCharge << ") exits copper." 
    //             << " Net charge: " << netCharge << "\n";
    //    dataFile.close();
    //  }
    //}
  }

  // Summarize particle transportation (FOR SETUP 1)
  dataFile.open(transFileName, std::ios::app);
  for ( int k=0; k<num_bins; k++ ) {
    dataFile << k << " " << bin_charge[k] << "e" << "\n";
  }
  dataFile.close();

  //// Summarize particle transportation (FOR SETUP 2)
  //dataFile.open(transFileName, std::ios::app);
  //dataFile << "The net charge of the irradiated copper is e" << "\n";
  //dataFile.close();

  // Create gnuplot file for analysis
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
  dataFile.close();
}
