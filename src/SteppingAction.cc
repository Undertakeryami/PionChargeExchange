#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4VPhysicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetPhysicalVolume();
    if (!volume) return;

    G4String volName = volume->GetName();
    
    // Fixed string evaluation: Check if the string starts with "DWC"
    if (volName.substr(0, 3) == "DWC") {
        if (step->IsFirstStepInVolume()) {
            G4int dwcID = volume->GetCopyNo(); 
            G4ThreeVector globalPos = step->GetPreStepPoint()->GetPosition();
            
            fEventAction->LogDWCHit(dwcID + 1, globalPos.x(), globalPos.y());
        }
    }
}
