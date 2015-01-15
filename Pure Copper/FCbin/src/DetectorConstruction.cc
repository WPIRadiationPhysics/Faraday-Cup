#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetector(0),
   fCheckOverlaps(true) {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Define materials and volumes
  DefineMaterials();  
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials() { 
  // Materials defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Cu");
  nistManager->FindOrBuildMaterial("G4_AIR");
  
  // Geant4 conventional definition of a vacuum
  G4double density     = universe_mean_density;  //from PhysicalConstants.h
  G4double pressure    = 1.e-19*pascal;
  G4double temperature = 0.1*kelvin;
  new G4Material("Vacuum", 1., 1.01*g/mole, density,
                   kStateGas,temperature,pressure);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//// Geometry parameters
G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
  // Copper cylinder parameters (layer 0)
  G4double Cu_cyl_innerRadius = 0*cm;
  G4double Cu_cyl_outerRadius = 3*cm;
  G4double Cu_cyl_height = 10*cm;
  G4double Cu_cyl_startAngle = 0*deg;
  G4double Cu_cyl_spanningAngle = 360*deg;

  // World cylinder parameters
  G4double world_innerRadius = 0*cm;
  G4double world_outerRadius = 5*cm;
  G4double world_height = 1.5*Cu_cyl_height;
  G4double world_startAngle = 0*deg;
  G4double world_spanningAngle = 360*deg;

  // Get materials
  //G4Material* defaultMaterial = G4Material::GetMaterial("G4_AIR");
  G4Material* defaultMaterial = G4Material::GetMaterial("Vacuum");
  G4Material* copperMaterial = G4Material::GetMaterial("G4_Cu");

  // Throw exception to ensure material usability
  if ( ! defaultMaterial || ! copperMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  // World Volume
  G4VSolid* worldS 
    = new G4Tubs("World",            // its name
                 world_innerRadius,
                 world_outerRadius,
                 world_height/2,
                 world_startAngle,
                 world_spanningAngle);
                         
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(
                 worldS,           // its solid
                 defaultMaterial,  // its material
                 "World");         // its name
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(
                 0,                // no rotation
                 G4ThreeVector(),
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 


  // Copper Cylinder (layer 0)
  G4VSolid* Cu_cylS 
    = new G4Tubs("Cu_cyl",            // its name
                 Cu_cyl_innerRadius,
                 Cu_cyl_outerRadius,
                 Cu_cyl_height/2,
                 Cu_cyl_startAngle,
                 Cu_cyl_spanningAngle);
                         
  G4LogicalVolume* Cu_cylLV
    = new G4LogicalVolume(
                 Cu_cylS,             // its solid
                 copperMaterial,      // its material
                 "Cu_cyl");           // its name
                                   
  fDetector
    = new G4PVPlacement(
                 0,                   // no rotation
                 G4ThreeVector(),
                 Cu_cylLV,            // its logical volume                         
                 "Cu_cyl",            // its name
                 worldLV,       // its mother volume
                 false,               // no boolean operation
                 0,                   // copy number
                 fCheckOverlaps);     // checking overlaps


  // Visualization attributes
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  simpleBoxVisAtt->SetVisibility(true);
  Cu_cylLV->SetVisAttributes(simpleBoxVisAtt);

  // Always return the physical World
  return worldPV;
}
