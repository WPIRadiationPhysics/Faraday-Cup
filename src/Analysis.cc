#include "Analysis.hh"
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

void Analysis::measureGainPreload() {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Creating gain ntuple
  analysisManager->CreateNtuple("gainDat", "Gain Data");
  analysisManager->CreateNtupleDColumn("beamEnergy");
  analysisManager->CreateNtupleDColumn("beamGain");
  analysisManager->FinishNtuple();
}

void Analysis::measureGainHistoCuPreload() {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create Cu charge deposition/removal/transfer histograms
  // (1000 tenth-percentile bins of h_Cu inward and r_Cu outward) for every energy
  // electrons, z
  analysisManager->CreateH2("eCuzr_in", "eCuzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eCuzr_out", "eCuzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eCuzr_inner", "eCuzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eCuzr_outer", "eCuzr_outer", 100, 0., 1., 100, 0., 1.);

  // protons, z
  analysisManager->CreateH2("pCuzr_in", "pCuzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pCuzr_out", "pCuzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pCuzr_inner", "pCuzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pCuzr_outer", "pCuzr_outer", 100, 0., 1., 100, 0., 1.);

  // ions, z
  analysisManager->CreateH2("oCuzr_in", "oCuzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oCuzr_out", "oCuzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oCuzr_inner", "oCuzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oCuzr_outer", "oCuzr_outer", 100, 0., 1., 100, 0., 1.);

  // neutrons
  analysisManager->CreateH2("nCuzr_in", "nCuzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nCuzr_out", "nCuzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nCuzr_inner", "nCuzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nCuzr_outer", "nCuzr_outer", 100, 0., 1., 100, 0., 1.);

  // gammas
  analysisManager->CreateH2("gCuzr_in", "gCuzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gCuzr_out", "gCuzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gCuzr_inner", "gCuzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gCuzr_outer", "gCuzr_outer", 100, 0., 1., 100, 0., 1.);
}

void Analysis::measureGainHistoKAPreload() {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create KA charge deposition/removal/transfer histograms
  // (100 percentile bins of KA_thickness inward and r_Ka outward) for every energy
  // electrons
  analysisManager->CreateH2("eKAzr_in", "eKAzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eKAzr_out", "eKAzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eKAzr_inner", "eKAzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("eKAzr_outer", "eKAzr_outer", 100, 0., 1., 100, 0., 1.);

  // protons
  analysisManager->CreateH2("pKAzr_in", "pKAzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pKAzr_out", "pKAzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pKAzr_inner", "pKAzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("pKAzr_outer", "pKAzr_outer", 100, 0., 1., 100, 0., 1.);

  // ions
  analysisManager->CreateH2("oKAzr_in", "oKAzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oKAzr_out", "oKAzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oKAzr_inner", "oKAzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("oKAzr_outer", "oKAzr_outer", 100, 0., 1., 100, 0., 1.);

  // neutrons
  analysisManager->CreateH2("nKAzr_in", "nKAzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nKAzr_out", "nKAzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nKAzr_inner", "nKAzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("nKAzr_outer", "nKAzr_outer", 100, 0., 1., 100, 0., 1.);

  // gammas
  analysisManager->CreateH2("gKAzr_in", "gKAzr_in", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gKAzr_out", "gKAzr_out", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gKAzr_inner", "gKAzr_inner", 100, 0., 1., 100, 0., 1.);
  analysisManager->CreateH2("gKAzr_outer", "gKAzr_outer", 100, 0., 1., 100, 0., 1.);
}

void Analysis::analyzeCascadeTracks() {

  // Create H3 quiver histogram structure
  struct quiverHisto {

    // E_n n-direction deposition histos in 3D (r,z) space
    G4double E_r[100][100] = {{0}},
             E_z[100][100] = {{0}};
  };

  // Acquire analysis instance
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Set ROOT vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  std::ostringstream ROOTfileNameStream, NtupleNameStream, cascadeFileNameStream;
  G4String cascadeFileName, NtupleName, ROOTfileName, data_dir = simulationAnalysis->GetAnalysisDIR();
  G4int ntupleId, ROOT_runID, ROOT_particleType;
  G4double ROOT_r, ROOT_z, ROOT_Er, ROOT_Ez;
  G4int threadID = G4Threading::G4GetThreadId();

  // Declare 3D particle energyDeposition quiver histos per energy and particle
  struct quiverHisto particleQuiver[5];

  // Acquire world logical volume dimensions
  G4LogicalVolume* worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Tubs* worldTubs = 0;
  worldTubs = dynamic_cast< G4Tubs*>(worldLV->GetSolid()); 
  G4double worldZHalfLength = worldTubs->GetZHalfLength(),
           worldOuterRadius = worldTubs->GetOuterRadius();

  // Read through signalTracks for both workers
  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {

    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    
    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << data_dir << "trackData_t" << workerID << ".root";
    ROOTfileName = ROOTfileNameStream.str();
    NtupleNameStream << "cascadeData";
    NtupleName = NtupleNameStream.str();
    
    // Read ntuples and do preloaded analyses
    analysisReader->SetFileName(ROOTfileName);
    ntupleId = analysisReader->GetNtuple(NtupleName);

    if ( ntupleId >= 0 ) {
      
      // Get relevant values
      //analysisReader->SetNtupleIColumn("run", ROOT_runID);
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

    // Remove analysis reader
    delete G4AnalysisReader::Instance();
  }

  // Create cascade output directory
  G4String syscmd = "mkdir -p " + data_dir + "cascadeHistos";
  system(syscmd);

  // Output cascade data
  for ( G4int particle_i = 0; particle_i < 5; particle_i++ ) {
    //for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {

      // Define data filename
      cascadeFileNameStream.str("");
      switch ( particle_i ) {
        case 0: cascadeFileNameStream << data_dir << "cascadeHistos/eCascade_" << runEnergy << "MeV.dat"; break; // energies[ROOT_runID]
        case 1: cascadeFileNameStream << data_dir << "cascadeHistos/pCascade_" << runEnergy << "MeV.dat"; break;
        case 2: cascadeFileNameStream << data_dir << "cascadeHistos/oCascade_" << runEnergy << "MeV.dat"; break;
        case 3: cascadeFileNameStream << data_dir << "cascadeHistos/nCascade_" << runEnergy << "MeV.dat"; break;
        case 4: cascadeFileNameStream << data_dir << "cascadeHistos/gCascade_" << runEnergy << "MeV.dat"; break;
      }
      cascadeFileName = cascadeFileNameStream.str();

      // Open particle-energy's data file
      std::ofstream cascadeFile;
      cascadeFile.open (cascadeFileName);

      // Loop through particleQuiver histogram
      for ( G4int bin_ir = 0; bin_ir < 100; bin_ir++ ) {
      for ( G4int bin_iz = 0; bin_iz < 100; bin_iz++ ) {

        // Aquire bin's vector, append file with non-zeros
        G4double binEr = particleQuiver[particle_i].E_r[bin_ir][bin_iz],
                 binEz = particleQuiver[particle_i].E_z[bin_ir][bin_iz];

        if ( ! ( binEr == 0 && binEz == 0 ) ) {

          // Append file and close
          cascadeFile << bin_ir << " " << bin_iz << " " << binEr << " " << binEz << "\n";
        }
      }}

      // Close particle-energy's data file
      cascadeFile.close();
    //}
  }
}

void Analysis::analyzeGainTracks() {

  // Acquire analysis instance and object
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Set ROOT vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  std::ostringstream ROOTfileNameStream, NtupleNameStream;
  G4String NtupleName, ROOTfileName, data_dir = simulationAnalysis->GetAnalysisDIR();
  G4int CuDepthNumHistos = 20, KADepthNumHistos = 20, numDepthHistosPerRun;
  G4double ROOT_gain[7] = {0}, ROOT_histoEntries[7*(20+20)] = {0}; 
  G4int ROOT_eventID, ROOT_runID, ntupleId, ROOT_signalType, ROOT_histoID;
  G4double ROOT_particleCharge, ROOT_netCharge, ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex, 
                            ROOT_rCuDepth, ROOT_zCuDepth, ROOT_rCuDepthVertex, ROOT_zCuDepthVertex,
                            ROOT_rKADepth, ROOT_zKADepth, ROOT_rKADepthVertex, ROOT_zKADepthVertex;

  // Create output file
  G4String ROOTAnalysisFileName = data_dir + "gainAnalysis.root";
  analysisManager->SetFileName(ROOTAnalysisFileName);
  analysisManager->OpenFile();

  // Initialize analysis preloads
  if ( simulationAnalysis->isMeasureGain() == 1 ) { simulationAnalysis->measureGainPreload(); }
  for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {
    if ( simulationAnalysis->isMeasureGainHistoCu() == 1 ) { simulationAnalysis->measureGainHistoCuPreload(); }
    if ( simulationAnalysis->isMeasureGainHistoKA() == 1 ) { simulationAnalysis->measureGainHistoKAPreload(); }
  }

  // Read through signalTracks for both workers
  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {

    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    
    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << data_dir << "trackData_t" << workerID;
    ROOTfileName = ROOTfileNameStream.str();
    NtupleNameStream << "gainData";
    NtupleName = NtupleNameStream.str();
    
    // Read ntuples and do preloaded analyses
    analysisReader->SetFileName(ROOTfileName);
    ntupleId = analysisReader->GetNtuple(NtupleName);

    if ( ntupleId >= 0 ) {
      
      // Get relevant values
      analysisReader->SetNtupleIColumn("run", ROOT_runID);
      analysisReader->SetNtupleIColumn("event", ROOT_eventID);
      analysisReader->SetNtupleDColumn("particleCharge", ROOT_particleCharge);
      analysisReader->SetNtupleDColumn("r", ROOT_r);
      analysisReader->SetNtupleDColumn("z", ROOT_z);
      analysisReader->SetNtupleDColumn("rVertex", ROOT_rVertex);
      analysisReader->SetNtupleDColumn("zVertex", ROOT_zVertex);
      analysisReader->SetNtupleDColumn("rCuDepth", ROOT_rCuDepth);
      analysisReader->SetNtupleDColumn("zCuDepth", ROOT_zCuDepth);
      analysisReader->SetNtupleDColumn("rCuDepthVertex", ROOT_rCuDepthVertex);
      analysisReader->SetNtupleDColumn("zCuDepthVertex", ROOT_zCuDepthVertex);
      analysisReader->SetNtupleDColumn("rKADepth", ROOT_rKADepth);
      analysisReader->SetNtupleDColumn("zKADepth", ROOT_zKADepth);
      analysisReader->SetNtupleDColumn("rKADepthVertex", ROOT_rKADepthVertex);
      analysisReader->SetNtupleDColumn("zKADepthVertex", ROOT_zKADepthVertex);
      analysisReader->SetNtupleDColumn("netCharge", ROOT_netCharge);
      analysisReader->SetNtupleIColumn("signalType", ROOT_signalType);

      // Loop through collected values of gain for contributions to gain
      // (note: null events here are charged particles tracks [hence filling ntuple] which don't contribute in Kapton)
      while ( analysisReader->GetNtupleRow(ntupleId) ) {

        // Accumulate gain measurement per energy (run)
        if ( simulationAnalysis->isMeasureGain() == 1 ) {
          ROOT_gain[ROOT_runID%nEnergies] += ROOT_netCharge;
        }

        // Acquire histo ID with a lack of KA histos on Cu run
        if ( simulationAnalysis->isMeasureGainHistoKA() != 1 ) { numDepthHistosPerRun = CuDepthNumHistos; }
        else { numDepthHistosPerRun = (CuDepthNumHistos + KADepthNumHistos); }
        ROOT_histoID = (ROOT_runID%nEnergies)*numDepthHistosPerRun + ROOT_signalType;

        // Populate track origin/terminus depth in Cu histograms
        if ( ROOT_signalType != 99 && simulationAnalysis->isMeasureGainHistoCu() == 1 ) {

          // Depositions
          if ( ROOT_signalType == 0 || ROOT_signalType == 4 || ROOT_signalType == 8 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepth, ROOT_rCuDepth);
          }
          // Removals
          else if ( ROOT_signalType == 1 || ROOT_signalType == 5 || ROOT_signalType == 9 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepthVertex, ROOT_rCuDepthVertex);
          }
          // Inner/Outer
          else if ( ROOT_signalType == 2 || ROOT_signalType == 6 || ROOT_signalType == 10 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepth, ROOT_rCuDepth);
            analysisManager->FillH2(ROOT_histoID + 1, ROOT_zCuDepthVertex, ROOT_rCuDepthVertex);

            // Semi-duplicate histogram, redundant scaling command
            ROOT_histoEntries[ROOT_histoID + 1] += 1;
          }

          // Increment histo scaling factor
          ROOT_histoEntries[ROOT_histoID] += 1;
        }

        // Populate track origin/terminus depth in KA histograms
        if ( ROOT_signalType != 99 && simulationAnalysis->isMeasureGainHistoKA() == 1 ) {

          // Depositions
          if ( ROOT_signalType == 12 || ROOT_signalType == 16 || ROOT_signalType == 20 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zKADepth, ROOT_rKADepth);
          }
          // Removals
          else if ( ROOT_signalType == 13 || ROOT_signalType == 17 || ROOT_signalType == 21 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zKADepthVertex, ROOT_rKADepthVertex);
          }
          // Inner/Outer
          else if ( ROOT_signalType == 14 || ROOT_signalType == 18 || ROOT_signalType == 22 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zKADepth, ROOT_rKADepth);
            analysisManager->FillH2(ROOT_histoID + 1, ROOT_zKADepthVertex, ROOT_rKADepthVertex);

            // Semi-duplicate histogram, redundant scaling command
            ROOT_histoEntries[ROOT_histoID + 1] += 1;
          }

          // Increment histo scaling factor
          ROOT_histoEntries[ROOT_histoID] += 1;
        }
      }
    }

    // Remove analysis reader
    delete G4AnalysisReader::Instance();
  }

  /*
  // Rescale histos by num entries // TEMPORARILY DISABLED
  if ( simulationAnalysis->isMeasureGainHistoCu() == 1  || simulationAnalysis->isMeasureGainHistoKA() == 1 ) {
    for ( G4int histo_i = 0; histo_i < (nEnergies*KADepthNumHistos); histo_i++) {

      // Don't break null histos
      if ( ROOT_histoEntries[histo_i] != 0 ) {
        analysisManager->ScaleH2(histo_i, (1/ROOT_histoEntries[histo_i]));
      }
    }
  }
  */

  // populate gain statistics
  if ( simulationAnalysis->isMeasureGain() == 1 ) {
    for ( G4int run_i = 0; run_i < nEnergies; run_i++ ) {
      analysisManager->FillNtupleDColumn(0, energies[run_i]);
      analysisManager->FillNtupleDColumn(1, ROOT_gain[run_i]);
      analysisManager->AddNtupleRow();
    }
  }

  // Print output file
  analysisManager->Write();
  analysisManager->CloseFile();
}
