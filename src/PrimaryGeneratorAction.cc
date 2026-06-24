#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4NuclideTable.hh"
#include "G4SystemOfUnits.hh"

// 강제 수명 제어 및 정확한 좌표 낚아채기를 위한 헤더들
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    // 1. 🌟 [순서 변경] GPS가 매크로 설정을 읽어 '무작위 위치'를 먼저 계산하고 버텍스를 생성하게 합니다.
    fParticleGun->GeneratePrimaryVertex(anEvent);

    // 2. 현재 GPS에 로드된 입자 정의(K-40)를 안전하게 가져옵니다.
    G4ParticleDefinition* ion = nullptr;
    if (fParticleGun->GetCurrentSource()) {
        ion = fParticleGun->GetCurrentSource()->GetParticleDefinition();
    }

    // 3. 만약 K-40 이온이고 불안정 핵종이라면 수명을 즉시 0초로 조집니다.
    if (ion && !ion->GetPDGStable()) {
        // 이 타이밍에 수명을 0으로 만들면, 방금 GPS가 계산해낸 '무작위 좌표'에 심어진 K-40이
        // 다음 Step을 밟자마자 그 자리에서 즉시 RadioactiveDecay를 일으킵니다.
        ion->SetPDGLifeTime(0.0 * ns);
        ion->SetPDGStable(false);
    }
}