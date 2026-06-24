#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

// 🌟 [K-40 해방용 핵심 헤더 추가]
#include "G4NuclideTable.hh"
#include "G4DeexPrecoParameters.hh"
#include "G4NuclearLevelData.hh"

RunAction::RunAction() {
    // 💡 생성자(Constructor) 타이밍에 데이터의 '틀(Ntuple 구조)'을 딱 한 번만 짜둡니다.
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    
    analysisManager->CreateNtuple("BackgroundData", "PCB Intrinsic Background Hits");
    analysisManager->CreateNtupleDColumn("Edep");     // Column 0
    analysisManager->CreateNtupleIColumn("EventID");  // Column 1
    analysisManager->CreateNtupleDColumn("DecayX");   // Column 2
    analysisManager->CreateNtupleDColumn("DecayY");   // Column 3
    analysisManager->CreateNtupleDColumn("DecayZ");   // Column 4
    analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    // 🔥 [최종 결전 치트키 삽입]
    // 시뮬레이션 버튼(beamOn)이 눌리고, 첫 K-40 입자가 튀어나오기 직전인 바로 '지금'
    // Geant4 전역 커널의 시간 장벽을 100경 년으로 고정해 버립니다.
    G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(1.0e+18 * CLHEP::year);
    
    G4DeexPrecoParameters* deexParams = G4NuclearLevelData::GetInstance()->GetParameters();
    if (deexParams) {
        deexParams->SetMaxLifeTime(1.0e+18 * CLHEP::year);
    }

    // -------------------------------------------------------------
    // 원래 있던 정갈한 기존 코드 로직 유지
    auto* analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->SetNtupleMerging(true); 
    analysisManager->SetNtupleRowWise(false); 
    
    analysisManager->OpenFile("simulation_output.root");
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}