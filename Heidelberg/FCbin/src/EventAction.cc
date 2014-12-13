#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>

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
  G4String fileVarGet; G4double netSignal=0;
  while(eventFile.good()) {
    getline(eventFile, fileVarGet);
    netSignal += atof(fileVarGet);
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
  runFile << netSignal << "\n";
  runFile.close();
  
  // Remove event file
  remove(eventFileName);
}
