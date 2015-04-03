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
  G4int ROOT_eventID, ROOT_runID, ntupleId;
  G4double ROOT_particleCharge, ROOT_netCharge;
  G4double ROOT_r, ROOT_z, ROOT_rVertex, ROOT_zVertex;

  // For each worker thread
  for ( G4int workerID = 0; workerID < nThreads; workerID++ ) {
    // Acquire analysis reader
    G4AnalysisReader* analysisReader = G4AnalysisReader::Instance();
    analysisReader->SetVerboseLevel(2);

    // Aquire ROOT data files
    ROOTfileNameStream.str(""); ROOTfileName = "";
    NtupleNameStream.str(""); NtupleName = "";
    ROOTfileNameStream << data_dir << "rootData_t" << workerID << ".root";
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
    
      // Loop through collected values of gain
      while ( analysisReader->GetNtupleRow(ntupleId) ) {
        ROOT_gain[ROOT_runID%7] += ROOT_netCharge;
      }
    }

    // Remove analysis reader
    delete G4AnalysisReader::Instance();
  }

  // Name ntuple by geometry iteration
  G4String ROOTgainFileName = data_dir + "gainData.root";

  // Creating data ntuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(10);
  analysisManager->SetFileName(ROOTgainFileName);
  analysisManager->CreateNtuple("gainDat", "Gain Data");
  analysisManager->CreateNtupleDColumn("beamEnergy");
  analysisManager->CreateNtupleDColumn("beamGain");
  analysisManager->FinishNtuple();
  analysisManager->OpenFile();

  // populate gain statistics
  for ( G4int run_i = 0; run_i < 7; run_i++ ) {
    G4cout << "energy: " << energies[run_i] << ", gain: " << ROOT_gain[run_i] << G4endl;
    analysisManager->FillNtupleDColumn(0, energies[run_i]);
    analysisManager->FillNtupleDColumn(1, ROOT_gain[run_i]);
    analysisManager->AddNtupleRow();
  }

  // Print output file
  analysisManager->Write();
  //analysisManager->CloseFile();
  delete G4AnalysisManager::Instance();
}
