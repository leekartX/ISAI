#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics_option4.hh" // 💡 저에너지 전자/감마선 정밀 물리 추가
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "QGSP_BERT_HP.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    // UI 세션 감지 (인자가 없으면 GUI 켬)
    G4UIExecutive* ui = nullptr;
    if (argc == 1) { ui = new G4UIExecutive(argc, argv); }

    // RunManager 생성
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // 💡 [순서 1] 진짜 Detector 기하학 등록 (오타 수정 완료!)
    runManager->SetUserInitialization(new DetectorConstruction());

    // 💡 [순서 2] Physics List 등록 (핵붕괴 + 저에너지 표준 물리 조합)
    G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());      // K-40 방사성 붕괴 물리
    physicsList->RegisterPhysics(new G4EmStandardPhysics_option4());   // 전자의 실리콘 센서 정밀 충돌 물리
    runManager->SetUserInitialization(physicsList);

    // 💡 [순서 3] Action Initialization 등록 (HistoManager가 안전하게 켜지는 타이밍)
    runManager->SetUserInitialization(new ActionInitialization());

    // 시각화 매니저 초기화
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UIman = G4UImanager::GetUIpointer();

    if (!ui) {
        // 배치 모드 (ex: ./PCB_Background_Sim run.mac)
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UIman->ApplyCommand(command + fileName);
    } else {
        // 인터랙티브 모드 (GUI 화면 켜짐)
        UIman->ApplyCommand("/control/execute init_vis.mac"); 
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
}