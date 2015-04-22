#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

Analysis* Analysis::GetAnalysis() {
  static Analysis the_analysis;

  return &the_analysis;
}

Analysis::Analysis() {}
Analysis::~Analysis() {}

void Analysis::Analyze_Gain(G4int nThreads) {

  // Set ROOT vars
  G4double energies[7] = {70.03, 100.46, 130.52, 160.09, 190.48, 221.06, 250.00};
  std::ostringstream ROOTfileNameStream;
  std::ostringstream NtupleNameStream; G4String NtupleName, ROOTfileName;
  G4String data_dir = "data/";
  G4double ROOT_gain[7] = {0}; 
  G4int ROOT_eventID, ROOT_runID, ntupleId, ROOT_signalType;
  G4double ROOT_particleCharge, ROOT_netCharge, ROOT_trackDepth, ROOT_trackDepthVertex;
  G4double ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex;
  
  // Acquire analysis manager and create output file
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4String ROOTstatsFileName = data_dir + "stats.root";
  analysisManager->SetFileName(ROOTstatsFileName);

  // Create charge transfer histograms (100 bins, depth_min to depth_max)
  // for every energy
  for ( G4int energy_i = 0; energy_i < 7; energy_i++ ) {
  
    analysisManager->CreateH1("sigType0", "worldToKA", 100, 0., 1.);
    analysisManager->CreateH1("sigType1", "worldToCu", 100, 0., 1.);
    analysisManager->CreateH1("sigType2", "KAToCu", 100, 0., 1.);
    analysisManager->CreateH1("sigType3", "KAToWorld", 100, 0., 1.);
    analysisManager->CreateH1("sigType4", "CuToKA", 100, 0., 1.);
    analysisManager->CreateH1("sigType5", "CuToWorld", 100, 0., 1.);
    analysisManager->CreateH1("sigType6", "KAToKA", 100, 0., 1.);
  }

  // For each worker thread
  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {

    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    
    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << data_dir << "rootData_t" << workerID; //<< ".root";
    ROOTfileName = ROOTfileNameStream.str();
    NtupleNameStream << "trackDat";
    NtupleName = NtupleNameStream.str();
    
    // Read ntuples and do same analysis, keep redundancy to test for plausibility
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
      analysisReader->SetNtupleDColumn("netCharge", ROOT_netCharge);
      analysisReader->SetNtupleIColumn("signalType", ROOT_signalType);
      analysisReader->SetNtupleDColumn("trackDepth", ROOT_trackDepth);
      analysisReader->SetNtupleDColumn("trackDepthVertex", ROOT_trackDepthVertex);
    
      // Loop through collected values of gain
      while ( analysisReader->GetNtupleRow(ntupleId) ) {

        // Accumulate gain measurement per energy (run)
        ROOT_gain[ROOT_runID%7] += ROOT_netCharge;
        
        // Populate track origin/terminus depth histograms
        if ( ROOT_signalType == 2 || ROOT_signalType == 3 ) {
          analysisManager->FillH1((ROOT_runID%7)*7 + ROOT_signalType, ROOT_trackDepthVertex);
        } else {
          analysisManager->FillH1((ROOT_runID%7)*7 + ROOT_signalType, ROOT_trackDepth);
	    }
      }
    }

    // Remove analysis reader
    delete G4AnalysisReader::Instance();
  }

  // Creating data ntuple
  analysisManager->CreateNtuple("gainDat", "Gain Data");
  analysisManager->CreateNtupleDColumn("beamEnergy");
  analysisManager->CreateNtupleDColumn("beamGain");
  analysisManager->FinishNtuple();
  analysisManager->OpenFile();

  // populate gain statistics
  for ( G4int run_i = 0; run_i < 7; run_i++ ) {
    analysisManager->FillNtupleDColumn(0, energies[run_i]);
    analysisManager->FillNtupleDColumn(1, ROOT_gain[run_i]);
    analysisManager->AddNtupleRow();
  }

  // Print output file
  analysisManager->Write();
  //analysisManager->CloseFile();
  delete G4AnalysisManager::Instance();
}
