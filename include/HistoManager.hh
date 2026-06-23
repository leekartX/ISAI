//
/// \file HistoManager.hh
/// \brief Definition of the HistoManager class
//
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

class HistoManager {
public:
  HistoManager();
 ~HistoManager();

private:
  void Book();
};

#endif