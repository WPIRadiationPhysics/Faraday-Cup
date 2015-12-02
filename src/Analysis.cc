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

void Analysis::analyze(G4int energy_i) {

  // Move plot scripts to data directory, create csv directory if necessary
  G4String syscmd = "cp -r analyze.sh plotScripts " + analysisDIR; system(syscmd);
           syscmd = "mkdir -p " + analysisDIR + "csv"; system(syscmd);

  // Gain measurement output functions
  if ( measuregain ) {

    appendGainFile();
    writeGainProfileCu(energy_i);
    writeLossProfileCu(energy_i);
    writeGainProfileKA(energy_i);
  }
}

void Analysis::appendGainFile() {

  // Acquire gain filename
  std::ostringstream gainFileNameStream;
  G4String gainFileName;
  gainFileNameStream << analysisDIR << "csv/modelGain.csv";
  gainFileName = gainFileNameStream.str();

  // Open gain output file
  std::ofstream gainFileStream;
  gainFileStream.open (gainFileName, std::ios::app);
  
  // Print gain output to csv
  gainFileStream << runEnergy << ", " << runGain << G4endl;
}

void Analysis::writeGainProfileCu(G4int energy_i) {

  // Acquire gain profile filename
  std::ostringstream gainProfileFileNameStream;
  G4String gainProfileFileName;
  gainProfileFileNameStream << analysisDIR << "csv/gainProfileCu-" << energy_i << ".csv";
  gainProfileFileName = gainProfileFileNameStream.str();

  // Open gain profile output file
  std::ofstream gainProfileFileStream;
  gainProfileFileStream.open (gainProfileFileName, std::ios::app);
  
  // Print gain profile values to csv
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      gainProfileFileStream << gainprofilecu[nprofilez][29-nprofiler] << ", "; // (29-nprofiler) for vertical graphical mapping
    }
    gainProfileFileStream << G4endl;
  }

  // Acquire log(gain) profile filename
  std::ostringstream logGainProfileFileNameStream;
  G4String logGainProfileFileName;
  logGainProfileFileNameStream << analysisDIR << "csv/logGainProfileCu-" << energy_i << ".csv";
  logGainProfileFileName = logGainProfileFileNameStream.str();

  // Open log(gain) profile output file
  std::ofstream logGainProfileFileStream;
  logGainProfileFileStream.open (logGainProfileFileName, std::ios::app);
  
  // Print log(gain) profile values to csv
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      logGainProfileFileStream << log10(gainprofilecu[nprofilez][29-nprofiler]) << ", "; // (29-nprofiler) for vertical graphical mapping
    }
    logGainProfileFileStream << G4endl;
  }

  // Acquire log(gain relative error) profile filename
  std::ostringstream gainErrorProfileFileNameStream;
  G4String gainErrorProfileFileName;
  gainErrorProfileFileNameStream << analysisDIR << "csv/gainErrorProfileCu-" << energy_i << ".csv";
  gainErrorProfileFileName = gainErrorProfileFileNameStream.str();

  // Open log(gain relative error) profile output file
  std::ofstream gainErrorProfileFileStream;
  gainErrorProfileFileStream.open (gainErrorProfileFileName, std::ios::app);
  
  // Calculate log(gain relative error) from gain and gain-square averages
  G4double gainEntries, gainMean, gainMeanSquare, gainVariance, gainStandardDeviation, gainError;
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {

      // Obtain statistics, ignore nulls
      gainEntries = gainentriesprofilecu[nprofilez][29-nprofiler]; // (29-nprofiler) for vertical graphical mapping
      gainMean = 0, gainMeanSquare = 0, gainVariance = 0, gainError = 0;
      if ( gainEntries != 0 ) {

        gainMean = gainprofilecu[nprofilez][29-nprofiler]/gainEntries;
        gainMeanSquare = gainsquareprofilecu[nprofilez][29-nprofiler]/gainEntries;
        gainVariance = std::abs(gainMean - gainMeanSquare);
        gainStandardDeviation = pow(gainVariance, 0.5);

        // Standard Error Definition
        gainError = gainStandardDeviation/(std::abs(gainprofilecu[nprofilez][29-nprofiler])/gainentriesprofilecu[nprofilez][29-nprofiler]);
      }

      // LOGify value
      if ( gainError != 0 ) { gainError = log(gainError); }

      // Print values to csv
      gainErrorProfileFileStream << gainError << ", ";
    }
    gainErrorProfileFileStream << G4endl;
  }
}

void Analysis::writeLossProfileCu(G4int energy_i) {

  // Acquire loss profile filename
  std::ostringstream lossProfileFileNameStream;
  G4String lossProfileFileName;
  lossProfileFileNameStream << analysisDIR << "csv/lossProfileCu-" << energy_i << ".csv";
  lossProfileFileName = lossProfileFileNameStream.str();

  // Open loss profile output file
  std::ofstream lossProfileFileStream;
  lossProfileFileStream.open (lossProfileFileName, std::ios::app);
  
  // Print loss profile values to csv
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      lossProfileFileStream << lossprofilecu[nprofilez][29-nprofiler] << ", "; // (29-nprofiler) for vertical graphical mapping
    }
    lossProfileFileStream << G4endl;
  }

  // Acquire log(loss) profile filename
  std::ostringstream logLossProfileFileNameStream;
  G4String logLossProfileFileName;
  logLossProfileFileNameStream << analysisDIR << "csv/logLossProfileCu-" << energy_i << ".csv";
  logLossProfileFileName = logLossProfileFileNameStream.str();

  // Open log(loss) profile output file
  std::ofstream logLossProfileFileStream;
  logLossProfileFileStream.open (logLossProfileFileName, std::ios::app);
  
  // Print log(loss) profile values to csv
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {
      logLossProfileFileStream << log10(lossprofilecu[nprofilez][29-nprofiler]) << ", "; // (29-nprofiler) for vertical graphical mapping
    }
    logLossProfileFileStream << G4endl;
  }

  // Acquire log(loss relative error) profile filename
  std::ostringstream lossErrorProfileFileNameStream;
  G4String lossErrorProfileFileName;
  lossErrorProfileFileNameStream << analysisDIR << "csv/lossErrorProfileCu-" << energy_i << ".csv";
  lossErrorProfileFileName = lossErrorProfileFileNameStream.str();

  // Open log(loss relative error) profile output file
  std::ofstream lossErrorProfileFileStream;
  lossErrorProfileFileStream.open (lossErrorProfileFileName, std::ios::app);
  
  // Calculate log(loss relative error) from loss and loss-square averages
  G4double lossEntries, lossMean, lossMeanSquare, lossVariance, lossStandardDeviation, lossError;
  for ( G4int nprofiler = 0; nprofiler < 30; nprofiler++ ) {
    for ( G4int nprofilez = 0; nprofilez < 100; nprofilez++ ) {

      // Obtain statistics, ignore nulls
      lossEntries = lossentriesprofilecu[nprofilez][29-nprofiler]; // (29-nprofiler) for vertical graphical mapping
      lossMean = 0, lossMeanSquare = 0, lossVariance = 0, lossError = 0;
      if ( lossEntries != 0 ) {

        lossMean = lossprofilecu[nprofilez][29-nprofiler]/lossEntries;
        lossMeanSquare = losssquareprofilecu[nprofilez][29-nprofiler]/lossEntries;
        lossVariance = std::abs(lossMean - lossMeanSquare);
        lossStandardDeviation = pow(lossVariance, 0.5);

        // Standard Error Definition
        lossError = lossStandardDeviation/(std::abs(lossprofilecu[nprofilez][29-nprofiler])/lossentriesprofilecu[nprofilez][29-nprofiler]);
      }

      // LOGify value
      if ( lossError != 0 ) { lossError = log(lossError); }

      // Print values to csv
      lossErrorProfileFileStream << lossError << ", ";
    }
    lossErrorProfileFileStream << G4endl;
  }
}

void Analysis::writeGainProfileKA(G4int energy_i) {

  // Acquire gain profile filename
  std::ostringstream gainProfileFileNameStream;
  G4String gainProfileFileName;
  gainProfileFileNameStream << analysisDIR << "csv/gainProfileKA-" << energy_i << ".csv";
  gainProfileFileName = gainProfileFileNameStream.str();

  // Open gain profile output file
  std::ofstream gainProfileFileStream;
  gainProfileFileStream.open (gainProfileFileName, std::ios::app);
  
  // Print gain profile values to csv
  for ( G4int ngainr = 0; ngainr < 31; ngainr++ ) {
    for ( G4int ngainz = 0; ngainz < 100; ngainz++ ) {
      gainProfileFileStream << gainprofileka[ngainz][30-ngainr] << ", "; // (30-ngainr) for vertical graphical mapping
    }
    gainProfileFileStream << G4endl;
  }

  // Acquire log(gain) profile filename
  std::ostringstream logGainProfileFileNameStream;
  G4String logGainProfileFileName;
  logGainProfileFileNameStream << analysisDIR << "csv/logGainProfileKA-" << energy_i << ".csv";
  logGainProfileFileName = logGainProfileFileNameStream.str();

  // Open log(gain) profile output file
  std::ofstream logGainProfileFileStream;
  logGainProfileFileStream.open (logGainProfileFileName, std::ios::app);
  
  // Print log(gain) profile values to csv
  for ( G4int ngainr = 0; ngainr < 31; ngainr++ ) {
    for ( G4int ngainz = 0; ngainz < 100; ngainz++ ) {

      // LOGify value
      G4double gainProfile = 0;
      if ( gainprofileka[ngainz][30-ngainr] > 0 ) { gainProfile = log10(gainprofileka[ngainz][30-ngainr]); }
      if ( gainprofileka[ngainz][30-ngainr] < 0 ) { gainProfile = -log10(-gainprofileka[ngainz][30-ngainr]); }
      logGainProfileFileStream << gainProfile << ", "; // (30-ngainr) for vertical graphical mapping
    }
    logGainProfileFileStream << G4endl;
  }

  // Acquire gain error profile filename
  std::ostringstream gainErrorProfileFileNameStream;
  G4String gainErrorProfileFileName;
  gainErrorProfileFileNameStream << analysisDIR << "csv/gainErrorProfileKA-" << energy_i << ".csv";
  gainErrorProfileFileName = gainErrorProfileFileNameStream.str();

  // Open gain error profile output file
  std::ofstream gainErrorProfileFileStream;
  gainErrorProfileFileStream.open (gainErrorProfileFileName, std::ios::app);
  
  // Calculate gain error from gain and gain-square averages
  G4double gainMeanSquare, gainStandardDeviation;
  for ( G4int ngainr = 0; ngainr < 31; ngainr++ ) {
    for ( G4int ngainz = 0; ngainz < 100; ngainz++ ) {

      // Obtain statistics
      gainMeanSquare = pow(gainprofileka[ngainz][30-ngainr], 2.0); // (30-ngainr) for vertical graphical mapping
      G4double gainVariance = 0;
      if ( gainentriesprofileka[ngainz][30-ngainr] != 0 )
        { gainVariance = std::abs(gainsquareprofileka[ngainz][30-ngainr] - gainMeanSquare)/gainentriesprofileka[ngainz][30-ngainr]; }
      gainStandardDeviation = pow(gainVariance, 0.5);

      // Standard Error Definition
      G4double gainError = 0;
      if ( gainentriesprofileka[ngainz][30-ngainr] != 0 )
        { gainError = gainStandardDeviation/(std::abs(gainprofileka[ngainz][30-ngainr])/gainentriesprofileka[ngainz][30-ngainr]); }

      // LOGify value
      if ( gainError != 0 ) { gainError = log(gainError); }

      // Print values to csv
      gainErrorProfileFileStream << gainError << ", ";
    }
    gainErrorProfileFileStream << G4endl;
  }
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
