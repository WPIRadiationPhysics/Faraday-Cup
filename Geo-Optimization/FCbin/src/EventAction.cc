#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "Randomize.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

// Useful module inserts
EventAction::EventAction() : G4UserEventAction() {}
EventAction::~EventAction() {}
void EventAction::BeginOfEventAction(const G4Event* /*event*/) {}

void EventAction::EndOfEventAction(const G4Event* event) {
  // Identify event file
  G4String data_dir = "data/";
  G4int eventID = event->GetEventID();
  std::ostringstream rawEventFileName;
  rawEventFileName << data_dir << "event" << eventID << "signals.txt";
  G4String eventFileName = rawEventFileName.str();
  
  // Acquire net differential gain from event
  std::ifstream eventFile(eventFileName);
  G4String fileVarGet;
  G4double pCuSignal = 0;
  G4double eCuSignal = 0;
  G4double otherCuSignal = 0;
  G4double pKASignal = 0;
  G4double eKASignal = 0;
  G4double otherKASignal = 0;
  G4double netSignal=0;
  while(eventFile.good()) {
	getline(eventFile, fileVarGet, ' '); pCuSignal += atof(fileVarGet);
    getline(eventFile, fileVarGet, ' '); eCuSignal += atof(fileVarGet);
    getline(eventFile, fileVarGet, ' '); otherCuSignal += atof(fileVarGet);
    getline(eventFile, fileVarGet, ' '); pKASignal += atof(fileVarGet);
    getline(eventFile, fileVarGet, ' '); eKASignal += atof(fileVarGet);
    getline(eventFile, fileVarGet, ' '); otherKASignal += atof(fileVarGet);
    getline(eventFile, fileVarGet); netSignal += atof(fileVarGet);
  }
  
  // Get current run, ID and beam info
  G4RunManager* runManager = G4RunManager::GetRunManager();
  G4int runID = runManager->GetCurrentRun()->GetRunID();
	
  // Add gain to runID's dataset
  std::ostringstream rawRunFileName;
  rawRunFileName << data_dir << "run" << runID << "signals.txt";
  G4String runFileName = rawRunFileName.str();
  std::ofstream runFile;
  runFile.open (runFileName, std::ios::app);
  runFile << pCuSignal << " " << eCuSignal << " " << otherCuSignal << " " << pKASignal << " " << eKASignal << " " << otherKASignal << " " << netSignal << "\n";
  runFile.close();
  
  // Remove event file
  remove(eventFileName);
}
