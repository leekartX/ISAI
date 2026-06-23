#include "EventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"     // 💡 mm 단위를 쓰기 위해 추가
#include "G4PrimaryVertex.hh"    // 💡 붕괴 위치(Vertex)를 가져오기 위해 추가

void EventAction::BeginOfEventAction(const G4Event*) {
    fTotalEdep = 0.0;
}

void EventAction::EndOfEventAction(const G4Event* event) {
    auto* analysisManager = G4AnalysisManager::Instance();
    
    // 1. 🎯 [핵심] 입자(K-40)가 최초로 붕괴하여 태어난 위치 좌표 가져오기
    G4double decayX = 0.0, decayY = 0.0, decayZ = 0.0;
    
    if (event->GetNumberOfPrimaryVertex() > 0) {
        G4PrimaryVertex* vertex = event->GetPrimaryVertex(0);
        G4ThreeVector pos = vertex->GetPosition();
        decayX = pos.x() / mm;  // mm 단위로 변환
        decayY = pos.y() / mm;
        decayZ = pos.z() / mm;
    }

    // 2. 🎯 장벽 해제! 에너지가 0이어도 무조건 파일에 한 줄 적습니다.
    analysisManager->FillNtupleDColumn(0, fTotalEdep);           // 에너지가 없으면 0.0으로 찍힘
    analysisManager->FillNtupleIColumn(1, event->GetEventID()); // 이벤트 번호
    
    // 💡 [주의] RunAction.cc에서 설정한 Column ID 번호에 맞게 아래 번호(2, 3, 4)를 확인해 주세요.
    analysisManager->FillNtupleDColumn(2, decayX);              // 붕괴 X 좌표
    analysisManager->FillNtupleDColumn(3, decayY);              // 붕괴 Y 좌표
    analysisManager->FillNtupleDColumn(4, decayZ);              // 붕괴 Z 좌표
    
    analysisManager->AddNtupleRow(); // 🚀 행 확정 저장!
}