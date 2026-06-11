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

    // Helper functions to log DWC hits from the SteppingAction
    void LogDWCHit(G4int id, G4double x, G4double y);

  private:
    // Vectors to hold DWC hits per event
    std::vector<G4int>    fDWC_ID;
    std::vector<G4double> fDWC_X;
    std::vector<G4double> fDWC_Y;
};

#endif
