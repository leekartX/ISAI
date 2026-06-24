#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

// 🌟 [K-40 해방용 핵심 헤더]
#include "G4NuclideTable.hh"
#include "G4DeexPrecoParameters.hh"
#include "G4NuclearLevelData.hh"

RunAction::RunAction() {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    
    // 🚀 머징 옵션 활성화
    analysisManager->SetNtupleMerging(true); 

    analysisManager->CreateNtuple("BackgroundData", "PCB Intrinsic Background Hits");
    analysisManager->CreateNtupleDColumn("Edep");     // Column 0
    analysisManager->CreateNtupleIColumn("EventID");  // Column 1
    analysisManager->CreateNtupleDColumn("DecayX");   // Column 2
    analysisManager->CreateNtupleDColumn("DecayY");   // Column 3
    analysisManager->CreateNtupleDColumn("DecayZ");   // Column 4
    analysisManager->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run* run) {
    // 🔥 [K-40 치트키]
    G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(1.0e+18 * CLHEP::year);
    G4DeexPrecoParameters* deexParams = G4NuclearLevelData::GetInstance()->GetParameters();
    if (deexParams) {
        deexParams->SetMaxLifeTime(1.0e+18 * CLHEP::year);
    }

    // -------------------------------------------------------------
    auto* analysisManager = G4AnalysisManager::Instance();
    
    // 🚀 [수정] IsMaster() 조건문을 지우고 모든 스레드가 이 로직을 통과하게 합니다.
    // Geant4 분석 매니저가 내부적으로 마스터/워커를 알아서 분기 처리합니다.
    analysisManager->OpenFile("simulation_output.root");
}

void RunAction::EndOfRunAction(const G4Run* run) {
    auto* analysisManager = G4AnalysisManager::Instance();
    
    // 🚀 [수정] 마찬가지로 IsMaster()를 지우고 순정 상태로 둡니다.
    analysisManager->Write();
    analysisManager->CloseFile();
}