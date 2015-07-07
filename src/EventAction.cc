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

void EventAction::BeginOfEventAction(const G4Event* /*event*/) {

  // Acquire analysis object
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Acquire multithreaded worker id
  G4int workerID = G4Threading::G4GetThreadId();

  // Nullify event's branching ratio
  simulationAnalysis->nullEventBranchingPN(workerID);
}

void EventAction::EndOfEventAction(const G4Event* /*event*/) {

  // Acquire analysis object
  Analysis* simulationAnalysis = Analysis::GetAnalysis();

  // Acquire multithreaded worker id
  G4int workerID = G4Threading::G4GetThreadId();

  // Acquire event protons/neutrons
  G4int npros = simulationAnalysis->recallEventBranchingPN(1, workerID),
        nneus = simulationAnalysis->recallEventBranchingPN(2, workerID);

  // Append event protons/neutrons to run proton/neutron statistics (ignore null for emphasized statistics)
  if ( ! ( npros == 0 && nneus == 0 )) { simulationAnalysis->appendRunBranchingPN(npros, nneus); }
}
