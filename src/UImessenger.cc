#include "UImessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

UImessenger::UImessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det) {

  fPFCDirectory = new G4UIdirectory("/PFC/");
  fPFCDirectory->SetGuidance("PFC UI settings");

  fStringCmd = new G4UIcmdWithAString("/PFC/setString",this);
  fStringCmd->SetGuidance("Set String to test UI");
  fStringCmd->SetParameterName("choice",false);
  fStringCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

UImessenger::~UImessenger() {
  delete fStringCmd;
  delete fPFCDirectory;
}

void UImessenger::SetNewValue(G4UIcommand* command,G4String newValue) {

  if( command == fStringCmd ) { G4cout << "Tested UI string is " << newValue << G4endl; }
}
