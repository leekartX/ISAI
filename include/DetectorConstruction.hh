#pragma once
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField() override; // 💡 멀티스레드 센서 매칭을 위해 이 줄을 반드시 추가해야 합니다!
};