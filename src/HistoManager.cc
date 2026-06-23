//
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class tailored for PCB Background Sim
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4AnalysisManager.hh"
#include "HistoManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
{
  // 생성 시점에 자동으로 Ntuple 구조와 분석 옵션을 예약(Book)합니다.
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // 1. Geant4 Generic Analysis Manager 인스턴스 확보
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  
  // 💡 [정갈함 세팅] 멀티스레드(MT) 환경에서 파일 파편화(_t0, _t1...)를 막고 
  // 단 하나의 깨끗한 메인 트리로 데이터를 자동 병합하도록 설정합니다.
  analysisManager->SetNtupleMerging(true);
  analysisManager->SetNtupleRowWise(false);

  // 2. Ntuple(데이터 테이블) 구조 정의
  // 트리 이름: "BackgroundData", 설명: "PCB Intrinsic Background Hits"
  analysisManager->CreateNtuple("BackgroundData", "PCB Intrinsic Background Hits");
  
  // 0번째 열: 실리콘 센서에 누적된 총 데미지 에너지 (단위: keV 명시)
  analysisManager->CreateNtupleDColumn("Edep_keV"); // Column ID = 0
  
  // 1번째 열: 방사선이 몇 번째 발사(Event)에서 충돌했는지 식별 번호
  analysisManager->CreateNtupleIColumn("EventID");  // Column ID = 1
  
  // Ntuple 구조 선언 마감
  analysisManager->FinishNtuple();
  
  G4cout << "----> HistoManager: Ntuple 'BackgroundData' has been booked successfully." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......