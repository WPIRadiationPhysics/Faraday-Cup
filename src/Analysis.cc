#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

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

void Analysis::measureCuChargePreload(G4int nEnergies) {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create Cu charge deposition/removal/transfer histograms
  // (1000 tenth-percentile bins of h_Cu inward and r_Cu outward) for every energy
  for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {
    
    // electrons, z
    analysisManager->CreateH2("eCuzr_in", "eCuzr_in", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eCuzr_out", "eCuzr_out", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eCuzr_inner", "eCuzr_inner", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eCuzr_outer", "eCuzr_outer", 100, 0., 1., 100, 0., 1.);

    // protons, z
    analysisManager->CreateH2("pCuzr_in", "pCuzr_in", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("pCuzr_out", "pCuzr_out", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("pCuzr_inner", "pCuzr_inner", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("pCuzr_outer", "pCuzr_outer", 1000, 0., 1., 1000, 0., 1.);

    // ions, z
    analysisManager->CreateH2("oCuzr_in", "oCuzr_in", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("oCuzr_out", "oCuzr_out", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("oCuzr_inner", "oCuzr_inner", 1000, 0., 1., 1000, 0., 1.);
    analysisManager->CreateH2("oCuzr_outer", "oCuzr_outer", 1000, 0., 1., 1000, 0., 1.);
  }
}

void Analysis::measureKAChargePreload(G4int nEnergies) {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create KA charge deposition/removal/transfer histograms
  // (100 percentile bins of KA_thickness inward and r_Ka outward) for every energy
  for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {
    
    // electrons, z
    analysisManager->CreateH2("eKAzr_in", "eKAzr_in", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eKAzr_out", "eKAzr_out", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eKAzr_inner", "eKAzr_inner", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("eKAzr_outer", "eKAzr_outer", 100, 0., 1., 100, 0., 1.);

    // protons, z
    analysisManager->CreateH2("pKAzr_in", "pKAzr_in", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("pKAzr_out", "pKAzr_out", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("pKAzr_inner", "pKAzr_inner", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("pKAzr_outer", "pKAzr_outer", 100, 0., 1., 100, 0., 1.);

    // ions, z
    analysisManager->CreateH2("oKAzr_in", "oKAzr_in", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("oKAzr_out", "oKAzr_out", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("oKAzr_inner", "oKAzr_inner", 100, 0., 1., 100, 0., 1.);
    analysisManager->CreateH2("oKAzr_outer", "oKAzr_outer", 100, 0., 1., 100, 0., 1.);
  }
}

void Analysis::analyzeTracks(G4int nThreads, G4int nEnergies) {

  // Set ROOT vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  std::ostringstream ROOTfileNameStream;
  std::ostringstream NtupleNameStream; G4String NtupleName, ROOTfileName;
  G4String data_dir = "data/";
  G4double ROOT_gain[7] = {0}; G4double ROOT_histoEntries[7*12] = {0}; 
  G4int ROOT_eventID, ROOT_runID, ntupleId, ROOT_signalType, ROOT_histoID;
  G4int CuDepthNumHistos = 12; G4int KADepthNumHistos = 12;
  G4double ROOT_particleCharge, ROOT_netCharge, ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex, 
                            ROOT_rCuDepth, ROOT_zCuDepth, ROOT_rCuDepthVertex, ROOT_zCuDepthVertex,
                            ROOT_rKADepth, ROOT_zKADepth, ROOT_rKADepthVertex, ROOT_zKADepthVertex;
  
  // Acquire analysis manager and object
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Create output file
  G4String ROOTAnalysisFileName = data_dir + "Analysis.root";
  analysisManager->SetFileName(ROOTAnalysisFileName);
  analysisManager->OpenFile();

  // Initialize analysis preloads
  if ( simulationAnalysis->isMeasureGain() == 1 ) { simulationAnalysis->measureGainPreload(); }
  if ( simulationAnalysis->isMeasureCuCharge() == 1 ) { simulationAnalysis->measureCuChargePreload(nEnergies); }
  if ( simulationAnalysis->isMeasureKACharge() == 1 ) { simulationAnalysis->measureKAChargePreload(nEnergies); }

  // Read through signalTracks for both workers
  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {

    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    
    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << data_dir << "signalTracks_t" << workerID;
    ROOTfileName = ROOTfileNameStream.str();
    NtupleNameStream << "trackData";
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

        // Histo id w.r.t. Cu runs lacking any KA histos
        if ( ROOT_runID < nEnergies ) { // model 0
          ROOT_histoID = (ROOT_runID%nEnergies)*CuDepthNumHistos + ROOT_signalType;
        } else { // models 1, 2
          ROOT_histoID = CuDepthNumHistos*nEnergies + ((ROOT_runID-nEnergies)%nEnergies)*(CuDepthNumHistos+KADepthNumHistos) + ROOT_signalType;
        }

        // Populate track origin/terminus depth in Cu histograms
        if ( ROOT_signalType != 99 && simulationAnalysis->isMeasureCuCharge() == 1 ) {

          // Depositions
          if ( ROOT_signalType == 0 || ROOT_signalType == 3 || ROOT_signalType == 6 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepth, ROOT_rCuDepth);
          }
          // Removals
          else if ( ROOT_signalType == 1 || ROOT_signalType == 4 || ROOT_signalType == 7 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepthVertex, ROOT_rCuDepthVertex);
          }
          // Inner/Outer
          else {
            analysisManager->FillH2(ROOT_histoID, ROOT_zCuDepth, ROOT_rCuDepth);
            analysisManager->FillH2(ROOT_histoID + 1, ROOT_zCuDepthVertex, ROOT_rCuDepthVertex);

            // Semi-duplicate histogram, redundant scaling command
            ROOT_histoEntries[ROOT_histoID + 1] += 1;
          }

          // Increment histo scaling factor
          ROOT_histoEntries[ROOT_histoID] += 1;
        }

        // Populate track origin/terminus depth in KA histograms
        if ( ROOT_signalType != 99 && simulationAnalysis->isMeasureKACharge() == 1 ) {

          // Depositions
          if ( ROOT_signalType == 12 || ROOT_signalType == 16 || ROOT_signalType == 20 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zKADepth, ROOT_rKADepth);
          }
          // Removals
          else if ( ROOT_signalType == 13 || ROOT_signalType == 17 || ROOT_signalType == 21 ) {
            analysisManager->FillH2(ROOT_histoID, ROOT_zKADepthVertex, ROOT_rKADepthVertex);
          }
          // Inner/Outer
          else {
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
  if ( simulationAnalysis->isMeasureCuCharge() == 1  || simulationAnalysis->isMeasureKACharge() == 1 ) {
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
  //analysisManager->CloseFile();
  delete G4AnalysisManager::Instance();
}
