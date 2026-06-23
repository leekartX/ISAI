#include "RunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() {
    // 💡 생성자(Constructor) 타이밍에 데이터의 '틀(Ntuple 구조)'을 딱 한 번만 짜둡니다.
    // 마스터 스레드와 워커 스레드가 각각 생성될 때 알아서 안전하게 이 구조를 생성합니다.
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
    auto* analysisManager = G4AnalysisManager::Instance();
    
    // 💡 [핵심] 시뮬레이션이 진짜 시작되어 파일이 열리기 직전에 병합 옵션을 활성화합니다.
    // 이렇게 분리하면 "Ntuple이 이미 존재한다"거나 "없다"는 에러가 원천 차단됩니다.
    analysisManager->SetNtupleMerging(true); 
    analysisManager->SetNtupleRowWise(false); // 단 하나의 정갈한 트리로 합쳐주는 치트키
    
    // 안전하게 출력 파일을 생성합니다.
    analysisManager->OpenFile("simulation_output.root");
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}