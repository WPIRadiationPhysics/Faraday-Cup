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

void Analysis::analyzeGain() {

  // Acquire gain filename
  std::ostringstream gainFileNameStream;
  G4String gainFileName;
  gainFileNameStream << analysisDIR << "modelGain.dat";
  gainFileName = gainFileNameStream.str();

  // Open gain output file
  std::ofstream gainFileStream;
  gainFileStream.open (gainFileName, std::ios::app);
  
  // Print gain output to csv
  gainFileStream << runEnergy << "," << runGain << G4endl;
}

void Analysis::analyzeBranchingRatiosPN() {

  // Declare analysis vars
  G4double runBranchingRatios[6][6] = {{0}};
  G4int runBranchNormal = 0;

  // Obtain branching ratio values and append normalization factor
  for ( G4int npro = 0; npro < 6; npro++ ){
    for ( G4int nneu = 0; nneu < 6; nneu++ ){
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
  for ( G4int npro = 0; npro < 6; npro++ ){
    for ( G4int nneu = 0; nneu < 6; nneu++ ){
      runBranchingRatios[npro][nneu] = runBranchingRatios[npro][nneu]/runBranchNormal;
      runBranchingFileStream << runBranchingRatios[npro][nneu];
      if ( nneu != 5 ) { runBranchingFileStream << ","; }
    }
    runBranchingFileStream << G4endl;
  }
}

void Analysis::analyzeCascade() {

  // Create H3 quiver histogram structure
  //struct quiverHisto {
  //
  //// E_n n-direction deposition histos in 3D (r,z) space
  //  G4double E_r[100][100] = {{0}},
  //           E_z[100][100] = {{0}};
  //};

  /*

  // Set ROOT vars
  std::ostringstream ROOTfileNameStream, NtupleNameStream, cascadeFileNameStream, syscmdStream;
  G4String cascadeFileName, NtupleName, ROOTfileName;
  G4int ntupleId, ROOT_particleType;
  G4double ROOT_r, ROOT_z, ROOT_Er, ROOT_Ez;

  // Declare 3D particle energyDeposition quiver histos per energy and particle
  struct quiverHisto particleQuiver[5];

  // Create cascade output directory
  G4String syscmd = "mkdir -p " + analysisDIR + "cascadeHistos";
  system(syscmd);

  // Acquire world logical volume dimensions
  G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Tubs* worldTubs = 0;
  worldTubs = dynamic_cast< G4Tubs*>(worldLV->GetSolid()); 
  G4double worldZHalfLength = worldTubs->GetZHalfLength(),
           worldOuterRadius = worldTubs->GetOuterRadius();

  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {

    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    
    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << analysisDIR << "trackData-" << RunID%nEnergies << "_t" << workerID << ".root";
    ROOTfileName = ROOTfileNameStream.str();
    NtupleNameStream << "cascadeData";
    NtupleName = NtupleNameStream.str();
    
    // Read ntuples and do preloaded analyses
    analysisReader->SetFileName(ROOTfileName);
    ntupleId = analysisReader->GetNtuple(NtupleName);

    if ( ntupleId >= 0 ) {

      // Get relevant values
      analysisReader->SetNtupleIColumn("particleType", ROOT_particleType);
      analysisReader->SetNtupleDColumn("r", ROOT_r);
      analysisReader->SetNtupleDColumn("z", ROOT_z);
      analysisReader->SetNtupleDColumn("eDep_r", ROOT_Er);
      analysisReader->SetNtupleDColumn("eDep_z", ROOT_Ez);

      // Loop through collected cascade fragments
      while ( analysisReader->GetNtupleRow(ntupleId) ) {

        // Acquire position as bin in 100^2 grid
        G4int rBin = floor((ROOT_r/worldOuterRadius)*100),
              zBin = floor(0.5*(1+(ROOT_z/worldZHalfLength))*100);
              // Adjust slight functional anomaly
              if ( rBin >= 100 ) { rBin = 99; }
              if ( zBin >= 100 ) { zBin = 99; }

        // Fill particle histogram structures by energy run number and particle type number
        particleQuiver[ROOT_particleType].E_r[rBin][zBin] += ROOT_Er;
        particleQuiver[ROOT_particleType].E_z[rBin][zBin] += ROOT_Ez;
      }
    }
  }

  // Remove analysis reader
  delete G4AnalysisReader::Instance();

  //Remove data files
  syscmdStream.str(""); syscmdStream << "rm " << analysisDIR << "trackData-" << RunID%nEnergies << "_t*";
  syscmd = syscmdStream.str(); system(syscmd);

  */

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

        // Aquire bin's vector, append file with non-zeros
        //G4double binEr = particleQuiver[particle_i].E_r[bin_ir][bin_iz],
        //         binEz = particleQuiver[particle_i].E_z[bin_ir][bin_iz];

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
