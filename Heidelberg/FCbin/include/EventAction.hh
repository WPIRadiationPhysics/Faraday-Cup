#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

// Event action class
// Defines data members to hold the energy deposit and track lengths of charged particles

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
};

#endif

    
