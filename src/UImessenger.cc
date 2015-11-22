#include "UImessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"

UImessenger::UImessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det) {

  fPFCDirectory = new G4UIdirectory("/PFC/");
  fPFCDirectory->SetGuidance("PFC UI settings");

  fModelCmd = new G4UIcmdWithAnInteger("/PFC/setModelNum", this);
  fModelCmd->SetGuidance("Model number (0 = Cu, 1 = CuKA, 2 = CuKAAgKA");
  fModelCmd->SetParameterName("choice", false);
  fModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  fKaptonCmd = new G4UIcmdWithADouble("/PFC/setKaptonThickness", this);
  fKaptonCmd->SetGuidance("Model Kapton thickness [micrometers] (ineffective w/ model 0)");
  fKaptonCmd->SetParameterName("choice", false);
  fKaptonCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

UImessenger::~UImessenger() {
  delete fModelCmd;
  delete fPFCDirectory;
}

void UImessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

  if( command == fModelCmd ) { fDetectorConstruction->ModelConfiguration(std::atoi(newValue)); }
  if( command == fKaptonCmd ) { fDetectorConstruction->IterateKaptonThickness(std::atof(newValue)); }
}
