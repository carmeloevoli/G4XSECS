#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

class EventAction : public G4UserEventAction {
   private:
   public:
    EventAction();
    ~EventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
};

#endif
