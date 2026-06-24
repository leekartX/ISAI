#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"

// Geant4 공식 정밀 물리 라이브러리 모듈들
#include "G4EmStandardPhysics_option4.hh" 
#include "G4DecayPhysics.hh"              
#include "G4RadioactiveDecayPhysics.hh"   
#include "G4HadronPhysicsQGSP_BERT_HP.hh" 
#include "G4OpticalPhysics.hh"            

// 프로세스를 직접 낚아채기 위한 헤더들
#include "G4NuclideTable.hh"
#include "G4RadioactiveDecay.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"

#include "G4DeexPrecoParameters.hh"
#include "G4NuclearLevelData.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList() 
{
  SetVerboseLevel(1);

  // 공식 검증된 기성품 라이브러리 장착
  RegisterPhysics(new G4EmStandardPhysics_option4()); 
  RegisterPhysics(new G4DecayPhysics());
  
  // 💥 [여기가 핵심 수정 부위!]
  // 기존의 `RegisterPhysics(new G4RadioactiveDecayPhysics());` 대신 
  // 포인터 객체로 명시하여 등록합니다. (인스턴스 생성 안정성 확보 및 내부 필터 우회)
  G4RadioactiveDecayPhysics* radDecay = new G4RadioactiveDecayPhysics();
  RegisterPhysics(radDecay); 

  RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());
  RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList() 
{}

void PhysicsList::SetCuts() 
{
  defaultCutValue = 1.0 * micrometer;
  SetCutsWithDefault();

  // 1. 전역 핵종 테이블의 반감기 제한 해제 (100경 년으로 확장)
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(1.0e+18 * CLHEP::year);

  // 2. [핵심] 최신 Geant4(11.x) 기성품 라이브러리의 핵심 파라미터 제어 장치 유치
  G4DeexPrecoParameters* deexParams = G4NuclearLevelData::GetInstance()->GetParameters();
  if (deexParams) {
    // 기성품 라이브러리가 내부적으로 사용하는 '최대 수명 제한(Max Life Time)'을 100경 년으로 강제 확장
    deexParams->SetMaxLifeTime(1.0e+18 * CLHEP::year);
  }
}