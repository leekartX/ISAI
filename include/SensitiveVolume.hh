#pragma once
#include "G4VSensitiveDetector.hh"

class SensitiveVolume : public G4VSensitiveDetector {
public:
    SensitiveVolume(G4String name);
    
    // 💡 [수정] = default; 를 지우고 컴파일러에게 .cc 파일에 진짜 소멸자 코드가 있다고 알려줍니다.
    ~SensitiveVolume() override; 
    
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
};