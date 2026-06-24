#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();

  // 기성품 라이브러리 방식에서 필수적으로 구현하는 가상 함수
  virtual void SetCuts();
};

#endif