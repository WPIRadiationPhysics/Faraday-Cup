#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

// Construct 'action' (whole routine) object
ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(), fDetConstruction(detConstruction) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  //RunAction* runAction = new RunAction();
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
  //RunAction* runAction = new RunAction();
  //runAction = new RunAction();
  SetUserAction(new RunAction);
  EventAction* eventAction = new EventAction;
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(fDetConstruction, eventAction));
}
