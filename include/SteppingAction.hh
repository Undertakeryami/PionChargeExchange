#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"
class EventAction;

class SteppingAction : public G4UserSteppingAction {
  public:
    SteppingAction(EventAction* eventAction);
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step* step) override;

  private:
    EventAction* fEventAction;
};

#endif
