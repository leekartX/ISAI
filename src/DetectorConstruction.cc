#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SubtractionSolid.hh" 
#include "SensitiveVolume.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* silicon = nistManager->FindOrBuildMaterial("G4_Si");
  G4Material* pcbMaterial = nistManager->FindOrBuildMaterial("G4_POLYETHYLENE"); 

  G4double leng_X_World = 1.0*m;         
  G4double leng_Y_World = 1.0*m;         
  G4double leng_Z_World = 1.0*m;         
  
  G4Box* solid_World = new G4Box("Solid_World", leng_X_World/2.0, leng_Y_World/2.0, leng_Z_World/2.0);
  G4LogicalVolume* log_World = new G4LogicalVolume(solid_World, air, "Log_World");
  
  G4VPhysicalVolume* phys_World = new G4PVPlacement(
    0, G4ThreeVector(), log_World, "Phys_World", 0, false, 0, false
  );

  G4double pcb_X = 110.1*mm;
  G4double pcb_Y = 75.1*mm;
  G4double pcb_Z = 2.0*mm;

  G4double xrpix_X = 13.2*mm;
  G4double xrpix_Y = 21.3*mm;
  G4double xrpix_Z = 0.3*mm;

  G4Box* solid_PCB_Raw = new G4Box("Solid_PCB_Raw", pcb_X/2.0, pcb_Y/2.0, pcb_Z/2.0);
  G4Box* solid_Hole = new G4Box("Solid_Hole", xrpix_X/2.0, xrpix_Y/2.0, (pcb_Z + 0.2*mm)/2.0);

  G4double hole_OffsetX = - (pcb_X/2.0) + 48.0*mm + (xrpix_X/2.0);
  G4double hole_OffsetY = - (pcb_Y/2.0) + 13.0*mm + (xrpix_Y/2.0);
  G4ThreeVector holePosition(hole_OffsetX, hole_OffsetY, 0.0*mm);

  G4SubtractionSolid* solid_PCB_With_Hole = new G4SubtractionSolid(
    "PCB_With_Hole", solid_PCB_Raw, solid_Hole, 0, holePosition
  );

  G4LogicalVolume* log_PCB = new G4LogicalVolume(solid_PCB_With_Hole, pcbMaterial, "Log_PCB");
  G4VisAttributes* vis_PCB = new G4VisAttributes(G4Colour::Green());
  log_PCB->SetVisAttributes(vis_PCB);

  new G4PVPlacement(0, G4ThreeVector(0,0,0), log_PCB, "Phys_PCB", log_World, false, 0, false);

  G4Box* solid_Si = new G4Box("Solid_XRPIX", xrpix_X/2.0, xrpix_Y/2.0, xrpix_Z/2.0);
  G4LogicalVolume* log_Si = new G4LogicalVolume(solid_Si, silicon, "Log_SiDetector");
  G4VisAttributes* vis_Si = new G4VisAttributes(G4Colour::Red());
  log_Si->SetVisAttributes(vis_Si);

  G4ThreeVector xrpixPosition(hole_OffsetX, hole_OffsetY, 0.9*mm);
  new G4PVPlacement(0, xrpixPosition, log_Si, "Phys_SiDetector", log_World, false, 0, false);

  return phys_World;
}

// 💡 [핵심 추가] 모든 코어(스레드)들이 센서를 똑바로 인지하도록 분리된 정석 함수
void DetectorConstruction::ConstructSDandField()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String sdName = "/MySensitiveVolume";
  SensitiveVolume* sensitiveVol = new SensitiveVolume(sdName);
  SDman->AddNewDetector(sensitiveVol);
  
  SetSensitiveDetector("Log_SiDetector", sensitiveVol);
}