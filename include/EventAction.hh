#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

class EventAction : public G4UserEventAction {
  public:
    EventAction();
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void LogDWCHit(G4int id, G4double x, G4double y);
    void SetTargetHasPi0() { fTargetHasPi0 = true; }

  private:
    std::vector<G4int>    fDWC_ID;
    std::vector<G4double> fDWC_X;
    std::vector<G4double> fDWC_Y;
    G4bool                fTargetHasPi0;
};

#endif
