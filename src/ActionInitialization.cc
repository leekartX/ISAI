#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"   
#include "EventAction.hh" 

// 💡 [이 함수를 무조건 추가해야 파일이 저장됩니다!]
// 대장(마스터) 스레드에게 파일 병합 및 저장 임무를 줍니다.
void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction());
}

// 기존 워커 스레드용 등록 함수 (그대로 유지)
void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction());
    SetUserAction(new RunAction());
    
    auto* eventAction = new EventAction();
    SetUserAction(eventAction);
}