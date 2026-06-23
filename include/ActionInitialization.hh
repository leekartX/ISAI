#pragma once
#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;
    
    // 💡 기존의 비어있던 바디(const {})를 지우고, 아래처럼 세미콜론(;)으로 끝냅니다.
    void BuildForMaster() const override; 
    void Build() const override;
};