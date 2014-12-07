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

#include "G4UniformMagField.hh"
#include "G4UniformElectricField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fAbsorberPV(0),
   fCheckOverlaps(true) {}

DetectorConstruction::~DetectorConstruction() { }

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

void DetectorConstruction::DefineMaterials() { 
  // Materials defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Au");
  //nistManager->FindOrBuildMaterial("G4_AIR");
  
  // Geant4 conventional definition of a vacuum
  G4double density     = universe_mean_density;  //from PhysicalConstants.h
  G4double pressure    = 2.7e-8*bar;
  G4double temperature = 0.1*kelvin;
  new G4Material("Vacuum", 1., 1.01*g/mole, density,
                   kStateGas,temperature,pressure);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//// Geometry parameters
G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
  // Gold target parameters (thin cylinder)
  G4double Au_cyl_innerRadius = 0*cm;
  G4double Au_cyl_outerRadius = 0.5*cm;
  G4double Au_cyl_height = 0.079*cm;
  G4double Au_cyl_startAngle = 0*deg;
  G4double Au_cyl_spanningAngle = 360*deg;

  // World cylinder parameters
  G4double world_innerRadius = 0*cm;
  G4double world_outerRadius = 5*Au_cyl_outerRadius;
  G4double world_height = Au_cyl_height + 19*cm;
  G4double world_startAngle = 0*deg;
  G4double world_spanningAngle = 360*deg;

  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Vacuum");
  //G4Material* defaultMaterial = G4Material::GetMaterial("G4_AIR");
  G4Material* goldMaterial = G4Material::GetMaterial("G4_Au");

  // Throw exception to ensure material usability
  if ( ! defaultMaterial || ! goldMaterial ) {
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
                 world_height,
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
                 G4ThreeVector(),  // at (0,0,0)
                 worldLV,          // its logical volume                         
                 "World",          // its name
                 0,                // its mother  volume
                 false,            // no boolean operation
                 0,                // copy number
                 fCheckOverlaps);  // checking overlaps 
                          
  // Gold Cylinder
  G4VSolid* Au_cylS 
    = new G4Tubs("Au_cyl",            // its name
                 Au_cyl_innerRadius,
                 Au_cyl_outerRadius,
                 Au_cyl_height,
                 Au_cyl_startAngle,
                 Au_cyl_spanningAngle);
                         
  G4LogicalVolume* Au_cylLV
    = new G4LogicalVolume(
                 Au_cylS,             // its solid
                 goldMaterial,      // its material
                 "Au_cyl");           // its name
                                   
  fAbsorberPV
    = new G4PVPlacement(
                 0,                   // no rotation
                 G4ThreeVector(),     // its position
                 Au_cylLV,            // its logical volume                         
                 "Au_cyl",            // its name
                 worldLV,       // its mother volume
                 false,               // no boolean operation
                 0,                   // copy number
                 fCheckOverlaps);     // checking overlaps


  // Visualization attributes
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  Au_cylLV->SetVisAttributes(simpleBoxVisAtt);
  
  // Always return the physical World
  return worldPV;
}

void DetectorConstruction::ConstructSDandField() {
  // Uniform electric and magnetic fields
  G4ThreeVector magFieldValue = G4ThreeVector(0.0, 0.2*gauss, 0.0);
  G4ThreeVector EMFieldValue = G4ThreeVector(0.0, 0.0, (300*volt)/(19*cm));
  
  G4MagneticField* magField= new G4UniformMagField(magFieldValue);
  G4FieldManager* globalFieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  globalFieldMgr->SetDetectorField(magField);
  G4ElectricField* fEMfield = new G4UniformElectricField(EMFieldValue);
  globalFieldMgr->SetDetectorField(fEMfield);
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(magFieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  G4AutoDelete::Register(fMagFieldMessenger);
}
