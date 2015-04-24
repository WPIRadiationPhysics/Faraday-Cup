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
#include "G4RunManager.hh"

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetector(0),
   fCheckOverlaps(true) {
	   
     // default Kapton thickness
     fKaptonThicknessMM = 0.059;
     
     // Acquire material data
     DefineMaterials();
   }

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Define geometric volumes
  return DefineVolumes();
}

void DetectorConstruction::IterateKaptonThickness(G4int KA_i) {
  
  // Redefine Kapton thickness
  G4double Kapton_thicknesses[3] = {0.059, 0.100, 0.200};
  fKaptonThicknessMM = Kapton_thicknesses[KA_i];
  
  // Reconstruct geometry
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void DetectorConstruction::DefineMaterials() {
	
  // Materials defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Cu");
  //nistManager->FindOrBuildMaterial("G4_AIR");
  
  // Geant4 conventional definition of a vacuum
  G4double density     = universe_mean_density;  //from PhysicalConstants.h
  G4double pressure    = 1.e-19*pascal;
  G4double temperature = 0.1*kelvin;
  new G4Material("Vacuum", 1., 1.01*g/mole, density,
                   kStateGas,temperature,pressure);

  // Manually constructing Kapton, did not seem to find from DB correctly...
  G4Material* Kapton = new G4Material("Kapton",1.42*g/cm3, 4);
  G4Element* elH = new G4Element("Hydrogen","H2",1.,1.01*g/mole);
  G4Element* elC = new G4Element("Carbon","C",6.,12.01*g/mole);
  G4Element* elN = new G4Element("Nitrogen","N2",7.,14.01*g/mole);
  G4Element* elO = new G4Element("Oxygen","O2",8.,16.*g/mole);
  Kapton->AddElement(elH, 0.0273);
  Kapton->AddElement(elC, 0.7213);
  Kapton->AddElement(elN, 0.0765);
  Kapton->AddElement(elO, 0.1749);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//// Geometry parameters
G4VPhysicalVolume* DetectorConstruction::DefineVolumes() {
	
  // Copper cylinder parameters (layer 0)
  G4double Cu_cyl_innerRadius = 0*mm;
  G4double Cu_cyl_outerRadius = 30*mm;
  G4double Cu_cyl_height = 100*mm;
  G4double Cu_cyl_startAngle = 0*deg;
  G4double Cu_cyl_spanningAngle = 360*deg;

  // Kapton cylinder parameters (layer 1)
  G4double Kapton_cyl1_innerRadius = 0*cm;
  G4double Kapton_cyl1_outerRadius = Cu_cyl_outerRadius + fKaptonThicknessMM*mm;
  G4double Kapton_cyl1_height = Cu_cyl_height + 2*fKaptonThicknessMM*mm;
  G4double Kapton_cyl1_startAngle = 0*deg;
  G4double Kapton_cyl1_spanningAngle = 360*deg;
  
  // World cylinder parameters
  G4double world_innerRadius = 0*mm;
  G4double world_outerRadius = 1.5*Kapton_cyl1_outerRadius;
  G4double world_height = Kapton_cyl1_height + 200*mm;
  G4double world_startAngle = 0*deg;
  G4double world_spanningAngle = 360*deg;

  // Get materials
  G4Material* defaultMaterial = G4Material::GetMaterial("Vacuum");
  G4Material* copperMaterial = G4Material::GetMaterial("G4_Cu");
  G4Material* KaptonMaterial = G4Material::GetMaterial("Kapton");

  // Throw exception to ensure material usability
  if ( ! defaultMaterial || ! copperMaterial || ! KaptonMaterial ) {
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

  
  // Kapton Hollow Cylinder 1 (layer 1) 
  G4VSolid* Kapton_cyl1S 
    = new G4Tubs("Kapton_cyl1",       // its name
                 Kapton_cyl1_innerRadius,
                 Kapton_cyl1_outerRadius,
                 Kapton_cyl1_height/2,
                 Kapton_cyl1_startAngle,
                 Kapton_cyl1_spanningAngle);
                         
  G4LogicalVolume* Kapton_cyl1LV
    = new G4LogicalVolume(
                 Kapton_cyl1S,        // its solid
                 KaptonMaterial,      // its material
                 "Kapton_cyl1");      // its name
                                   
  fDetector
    = new G4PVPlacement(
                 0,                   // no rotation
                 G4ThreeVector(),
                 Kapton_cyl1LV,       // its logical volume                         
                 "Kapton_cyl1",       // its name
                 worldLV,             // its mother  volume
                 false,               // no boolean operation
                 0,                   // copy number
                 fCheckOverlaps);     // checking overlaps


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
                 Kapton_cyl1LV,             // its mother volume
                 false,               // no boolean operation
                 0,                   // copy number
                 fCheckOverlaps);     // checking overlaps


  // Visualization attributes
  worldLV->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  simpleBoxVisAtt->SetVisibility(true);
  Cu_cylLV->SetVisAttributes(simpleBoxVisAtt);
  Kapton_cyl1LV->SetVisAttributes(simpleBoxVisAtt);

  // Always return the physical World
  return worldPV;
}
