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

void Analysis::measureKAAxialChargePreload(G4int nEnergies) {

  // Acquire analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Create KA charge deposition/removal/transfer histograms
  // (100 percentile bins of KA_thickness inward) for every energy
  for ( G4int energy_i = 0; energy_i < nEnergies; energy_i++ ) {
    
    // electrons, z
    analysisManager->CreateH1("eKAz_in", "eKAz_in", 100, 0., 1.);
    analysisManager->CreateH1("eKAz_out", "eKAz_out", 100, 0., 1.);
    analysisManager->CreateH1("eKAz_inner", "eKAz_inner", 100, 0., 1.);
    analysisManager->CreateH1("eKAz_outer", "eKAz_outer", 100, 0., 1.);

    // protons, z
    analysisManager->CreateH1("pKAz_in", "pKAz_in", 100, 0., 1.);
    analysisManager->CreateH1("pKAz_out", "pKAz_out", 100, 0., 1.);
    analysisManager->CreateH1("pKAz_inner", "pKAz_inner", 100, 0., 1.);
    analysisManager->CreateH1("pKAz_outer", "pKAz_outer", 100, 0., 1.);

    // ions, z
    analysisManager->CreateH1("oKAz_in", "oKAz_in", 100, 0., 1.);
    analysisManager->CreateH1("oKAz_out", "oKAz_out", 100, 0., 1.);
    analysisManager->CreateH1("oKAz_inner", "oKAz_inner", 100, 0., 1.);
    analysisManager->CreateH1("oKAz_outer", "oKAz_outer", 100, 0., 1.);

    // electrons, r
    analysisManager->CreateH1("eKAr_in", "eKAr_in", 100, 0., .1);
    analysisManager->CreateH1("eKAr_out", "eKAr_out", 100, 0., .1);
    analysisManager->CreateH1("eKAr_inner", "eKAr_inner", 100, 0., .1);
    analysisManager->CreateH1("eKAr_outer", "eKAr_outer", 100, 0., .1);

    // protons, r
    analysisManager->CreateH1("pKAr_in", "pKAr_in", 100, 0., .1);
    analysisManager->CreateH1("pKAr_out", "pKAr_out", 100, 0., .1);
    analysisManager->CreateH1("pKAr_inner", "pKAr_inner", 100, 0., .1);
    analysisManager->CreateH1("pKAr_outer", "pKAr_outer", 100, 0., .1);

    // ions, r
    analysisManager->CreateH1("oKAr_in", "oKAr_in", 100, 0., .1);
    analysisManager->CreateH1("oKAr_out", "oKAr_out", 100, 0., .1);
    analysisManager->CreateH1("oKAr_inner", "oKAr_inner", 100, 0., .1);
    analysisManager->CreateH1("oKAr_outer", "oKAr_outer", 100, 0., .1);
  }
}

void Analysis::analyzeTracks(G4int nThreads, G4int nEnergies) {

  // Set ROOT vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  std::ostringstream ROOTfileNameStream;
  std::ostringstream NtupleNameStream; G4String NtupleName, ROOTfileName;
  G4String data_dir = "data/";
  G4double ROOT_gain[7] = {0}; G4double ROOT_histoEntries[7*24] = {0}; 
  G4int ROOT_eventID, ROOT_runID, ntupleId, ROOT_signalType, ROOT_histoID;
  G4int fMeasureKAAxialChargeNumHistos = 24;
  G4double ROOT_particleCharge, ROOT_netCharge, ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex, 
                            ROOT_rDepth, ROOT_zDepth, ROOT_rDepthVertex, ROOT_zDepthVertex;
  
  // Acquire analysis manager and object
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Create output file
  G4String ROOTAnalysisFileName = data_dir + "Analysis.root";
  analysisManager->SetFileName(ROOTAnalysisFileName);
  analysisManager->OpenFile();

  // Initialize analysis preloads
  if ( simulationAnalysis->isMeasureGain() == 1 ) { simulationAnalysis->measureGainPreload(); }
  if ( simulationAnalysis->isMeasureKAAxialCharge() == 1 ) { simulationAnalysis->measureKAAxialChargePreload(nEnergies); }

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
      analysisReader->SetNtupleDColumn("rDepth", ROOT_rDepth);
      analysisReader->SetNtupleDColumn("zDepth", ROOT_zDepth);
      analysisReader->SetNtupleDColumn("rDepthVertex", ROOT_rDepthVertex);
      analysisReader->SetNtupleDColumn("zDepthVertex", ROOT_zDepthVertex);
      analysisReader->SetNtupleDColumn("netCharge", ROOT_netCharge);
      analysisReader->SetNtupleIColumn("signalType", ROOT_signalType);


      // Loop through collected values of gain for contributions to gain
      // (note: null events here are charged particles tracks [hence filling ntuple] which don't contribute in Kapton)
      while ( analysisReader->GetNtupleRow(ntupleId) ) {

        // Accumulate gain measurement per energy (run)
        if ( simulationAnalysis->isMeasureGain() == 1 ) {
          ROOT_gain[ROOT_runID%nEnergies] += ROOT_netCharge;
        }

        // Populate track origin/terminus depth in KA histograms
        if ( ROOT_signalType != 99 && simulationAnalysis->isMeasureKAAxialCharge() == 1 ) {

          // Histo id w.r.t. Cu runs lacking any histos
          ROOT_histoID = ((ROOT_runID-nEnergies)%nEnergies)*fMeasureKAAxialChargeNumHistos + ROOT_signalType;

          // Depositions
          if ( ROOT_signalType == 0 || ROOT_signalType == 3 || ROOT_signalType == 6 ) {
            analysisManager->FillH1(ROOT_histoID, ROOT_zDepth);
            analysisManager->FillH1(ROOT_histoID+12, ROOT_rDepth);
          }
          // Removals
          else if ( ROOT_signalType == 1 || ROOT_signalType == 4 || ROOT_signalType == 7 ) {
            analysisManager->FillH1(ROOT_histoID, ROOT_zDepthVertex);
            analysisManager->FillH1(ROOT_histoID+12, ROOT_rDepthVertex);
          }
          // Inner/Outer
          else {
            analysisManager->FillH1(ROOT_histoID, ROOT_zDepth);
            analysisManager->FillH1(ROOT_histoID+12, ROOT_rDepth);
            analysisManager->FillH1(ROOT_histoID + 1, ROOT_zDepthVertex);
            analysisManager->FillH1(ROOT_histoID+12 + 1, ROOT_rDepthVertex);

            // Semi-duplicate histogram, redundant scaling command
            ROOT_histoEntries[ROOT_histoID + 1] += 1;
            ROOT_histoEntries[ROOT_histoID+12 + 1] += 1;
          }

          // Increment histo scaling factor
          ROOT_histoEntries[ROOT_histoID] += 1;
          ROOT_histoEntries[ROOT_histoID+12] += 1;
        }
      }
    }

    // Remove analysis reader
    delete G4AnalysisReader::Instance();
  }

  // Rescale histos by num entries
  if ( simulationAnalysis->isMeasureKAAxialCharge() == 1 ) {
    for ( G4int histo_i = 0; histo_i < (nEnergies*fMeasureKAAxialChargeNumHistos); histo_i++) {

      // Don't break null histos
      if ( ROOT_histoEntries[histo_i] != 0 ) {
        analysisManager->ScaleH1(histo_i, (1/ROOT_histoEntries[histo_i]));
      }
    }
  }

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
