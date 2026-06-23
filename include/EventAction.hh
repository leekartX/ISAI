#pragma once
#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction() = default;
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    // 센서에서 호출하여 에너지를 누적할 함수
    void AddEdep(G4double edep) { fTotalEdep += edep; }

private:
    G4double fTotalEdep = 0.0;
};