#include "SensitiveVolume.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

// 스레드별 독립 누적 변수들
thread_local G4int lastEventID = -1;
thread_local G4double accumulatedEdep = 0.0;
thread_local G4double currentDecayX = 0.0;
thread_local G4double currentDecayY = 0.0;
thread_local G4double currentDecayZ = 0.0;

// 1. 생성자 (정상 등록)
SensitiveVolume::SensitiveVolume(G4String name) : G4VSensitiveDetector(name) {}

// 2. 소멸자 (여기에 딱 하나만 남겨둡니다)
SensitiveVolume::~SensitiveVolume() {
    if (lastEventID != -1) {
        auto* analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleDColumn(0, accumulatedEdep / keV);
        analysisManager->FillNtupleIColumn(1, lastEventID);
        analysisManager->FillNtupleDColumn(2, currentDecayX);
        analysisManager->FillNtupleDColumn(3, currentDecayY);
        analysisManager->FillNtupleDColumn(4, currentDecayZ);
        analysisManager->AddNtupleRow();
    }
}

// 3. 프로세스 힛 (에너지가 없어도 무조건 기록하는 정석 로직)
G4bool SensitiveVolume::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto* track = step->GetTrack();
    
    // 입자가 태어난 첫 번째 스텝일 때 좌표 기억
    if (track->GetCurrentStepNumber() == 1) { 
        auto* creatorProcess = track->GetCreatorProcess();
        if (creatorProcess && creatorProcess->GetProcessName() == "RadioactiveDecay") {
            G4ThreeVector decayPos = track->GetPosition();
            currentDecayX = decayPos.x() / mm;
            currentDecayY = decayPos.y() / mm;
            currentDecayZ = decayPos.z() / mm;
        }
        else if (!creatorProcess) { // 일반 전자 총(GPS) 대비용
            G4ThreeVector primaryPos = track->GetPosition();
            currentDecayX = primaryPos.x() / mm;
            currentDecayY = primaryPos.y() / mm;
            currentDecayZ = primaryPos.z() / mm;
        }
    }

    G4int currentEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    auto* analysisManager = G4AnalysisManager::Instance();
    
    // 새로운 이벤트가 시작되면 이전 이벤트 내용을 무조건 한 줄 저장
    if (currentEventID != lastEventID) {
        if (lastEventID != -1) {
            analysisManager->FillNtupleDColumn(0, accumulatedEdep / keV); 
            analysisManager->FillNtupleIColumn(1, lastEventID);          
            analysisManager->FillNtupleDColumn(2, currentDecayX);        
            analysisManager->FillNtupleDColumn(3, currentDecayY);        
            analysisManager->FillNtupleDColumn(4, currentDecayZ);        
            analysisManager->AddNtupleRow();                             
        }
        // 변수 초기화 및 현재 이벤트 정보 갱신
        accumulatedEdep = 0.0;
        lastEventID = currentEventID;
        currentDecayX = 0.0; currentDecayY = 0.0; currentDecayZ = 0.0;
    }

    // 에너지는 들어올 때만 차곡차곡 누적
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0.) {
        accumulatedEdep += edep; 
    }

    return true;
}