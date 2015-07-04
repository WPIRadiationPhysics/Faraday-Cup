#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Tubs.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

Analysis::Analysis() {}
Analysis::~Analysis() {}

void Analysis::appendGainFile() {

  // Acquire gain filename
  std::ostringstream gainFileNameStream;
  G4String gainFileName;
  gainFileNameStream << analysisDIR << "modelGain.csv";
  gainFileName = gainFileNameStream.str();

  // Open gain output file
  std::ofstream gainFileStream;
  gainFileStream.open (gainFileName, std::ios::app);
  
  // Print gain output to csv
  gainFileStream << runEnergy << "," << runGain << G4endl;
}

void Analysis::ntupleizeGainFile() {

  // Create analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create ntuple
  analysisManager->CreateNtuple("gainData", "Model Gain data");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->CreateNtupleDColumn("Gain");
  analysisManager->FinishNtuple();
  analysisManager->OpenFile(analysisDIR+"modelGain");

  // Acquire gain filename
  std::ostringstream gainFileNameStream;
  G4String gainFileName;
  gainFileNameStream << analysisDIR << "modelGain.csv";
  gainFileName = gainFileNameStream.str();

  // Open gain input file
  std::ifstream gainFileStream (gainFileName);
  G4String gainFileGet;
  while ( gainFileStream.good() ) {

    // read csv delimiters, fill ROOT ntuple
    getline(gainFileStream, gainFileGet, ',');
    analysisManager->FillNtupleDColumn(0, std::atof(gainFileGet));
    getline(gainFileStream, gainFileGet);
    analysisManager->FillNtupleDColumn(1, std::atof(gainFileGet));
    analysisManager->AddNtupleRow();
  }

  // Write and close ROOT file
  analysisManager->Write();
  analysisManager->CloseFile();

  // Destroy analysis manager
  delete G4AnalysisManager::Instance();
}

void Analysis::analyzeBranchingRatiosPN() {

  // Declare analysis vars
  G4double runBranchingRatios[7][7] = {{0}};
  G4int runBranchNormal = 0;

  // Obtain branching ratio values and append normalization factor
  for ( G4int npro = 0; npro < 7; npro++ ){
    for ( G4int nneu = 0; nneu < 7; nneu++ ){
      runBranchingRatios[npro][nneu] = recallRunBranchingPN(npro, nneu);
      runBranchNormal += runBranchingRatios[npro][nneu];
    }
  }

  // Create branching ratio output directory
  G4String syscmd = "mkdir -p " + analysisDIR + "branchRatios";
  system(syscmd);

  // Acquire branching ratio output filename
  std::ostringstream runBranchingFileNameStream;
  G4String runBranchingFileName;
  runBranchingFileNameStream << analysisDIR << "branchRatios/ratiosPN-" << RunID%nEnergies << ".csv";
  runBranchingFileName = runBranchingFileNameStream.str();

  // Open branching ratio output file
  std::ofstream runBranchingFileStream;
  runBranchingFileStream.open (runBranchingFileName);
  
  // Print normalized (p,NpMn) output to csv
  if ( runBranchNormal != 0 ) {
    for ( G4int npro = 0; npro < 7; npro++ ) {
      for ( G4int nneu = 0; nneu < 7; nneu++ ) {
        runBranchingRatios[npro][nneu] = runBranchingRatios[npro][nneu]/runBranchNormal;
        runBranchingFileStream << runBranchingRatios[npro][nneu];
        if ( nneu != 6 ) { runBranchingFileStream << ","; }
      }
      runBranchingFileStream << G4endl;  
    }
  // Otherwise, print pity message
  } else { runBranchingFileStream << "No tracked production events" << G4endl; }
}

void Analysis::analyzeCascade() {
  
  // Create cascade output directory
  G4String syscmd = "mkdir -p " + analysisDIR + "cascadeHistos";
  system(syscmd);

  // Output cascade data
  std::ostringstream cascadeFileNameStream;
  G4String cascadeFileName;
  for ( G4int particle_i = 0; particle_i < 5; particle_i++ ) {

    // Define data filename
    cascadeFileNameStream.str("");
    switch ( particle_i ) {
      case 0: cascadeFileNameStream << analysisDIR << "cascadeHistos/eCascade-" << RunID%nEnergies << ".dat"; break;
      case 1: cascadeFileNameStream << analysisDIR << "cascadeHistos/pCascade-" << RunID%nEnergies << ".dat"; break;
      case 2: cascadeFileNameStream << analysisDIR << "cascadeHistos/oCascade-" << RunID%nEnergies << ".dat"; break;
      case 3: cascadeFileNameStream << analysisDIR << "cascadeHistos/nCascade-" << RunID%nEnergies << ".dat"; break;
      case 4: cascadeFileNameStream << analysisDIR << "cascadeHistos/gCascade-" << RunID%nEnergies << ".dat"; break;
    }
    cascadeFileName = cascadeFileNameStream.str();

    // Open particle-energy's data file
    std::ofstream cascadeFile;
    cascadeFile.open (cascadeFileName);

    // Loop through particleQuiver histogram
    for ( G4int bin_ir = 0; bin_ir < 100; bin_ir++ ) {
      for ( G4int bin_iz = 0; bin_iz < 100; bin_iz++ ) {

        // Obtain enery deposition vector components
        G4double binEr, binEz;
        switch ( particle_i ) {
          case 0: binEr = recallECascade(bin_ir, bin_iz, 1); binEz = recallECascade(bin_ir, bin_iz, 2); break;
          case 1: binEr = recallPCascade(bin_ir, bin_iz, 1); binEz = recallPCascade(bin_ir, bin_iz, 2); break;
          case 2: binEr = recallOCascade(bin_ir, bin_iz, 1); binEz = recallOCascade(bin_ir, bin_iz, 2); break;
          case 3: binEr = recallNCascade(bin_ir, bin_iz, 1); binEz = recallNCascade(bin_ir, bin_iz, 2); break;
          case 5: binEr = recallGCascade(bin_ir, bin_iz, 1); binEz = recallGCascade(bin_ir, bin_iz, 2); break;
        }
        // Append file with non-null cascade data
        if ( ! ( binEr == 0 && binEz == 0 ) )
          { cascadeFile << bin_ir << " " << bin_iz << " " << binEr << " " << binEz << G4endl; }
      }
    }

    // Close particle-energy's data file
    cascadeFile.close();
  }
}
