#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4ProcessType.hh" // Geant4 Core Enum for Physics Types

SteppingAction::SteppingAction(EventAction* eventAction)
  : fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step) {
    G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetPhysicalVolume();
    if (!volume) return;

    G4String volName = volume->GetName();
    
    // 1. DWC Tracking (Matches "DWC_1", "DWC_2", etc. from your DetectorConstruction)
    if (volName.find("DWC") == 0) {
        if (step->IsFirstStepInVolume()) {
            G4int dwcID = volume->GetCopyNo(); 
            G4ThreeVector globalPos = step->GetPreStepPoint()->GetPosition();
            fEventAction->LogDWCHit(dwcID + 1, globalPos.x(), globalPos.y());
        }
    }

    // 2. Strict, Documented Charge Exchange Kinematic Filter
    G4Track* track = step->GetTrack();
    
    // ONLY track the primary beam pion
    if (track->GetTrackID() == 1) {
        
        // ONLY look for reactions inside your two explicitly named targets
        if (volName == "LithiumTarget" || volName == "GraphiteTarget") {
            
            // Catch the exact terminal step where the beam pion is destroyed
            if (track->GetTrackStatus() == fStopAndKill) {
                
                const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();
                
                // Verify the pion was destroyed by a Hadronic (nuclear) process
                if (process && process->GetProcessType() == fHadronic) {
                    
                    // Extract the specific debris created by THIS exact collision
                    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
                    
                    if (secondaries) {
                        G4bool producedPi0 = false;
                        G4bool producedChargedPion = false;

                        for (auto secTrack : *secondaries) {
                            G4String particleName = secTrack->GetDefinition()->GetParticleName();
                            
                            if (particleName == "pi0") {
                                producedPi0 = true;
                            }
                            if (particleName == "pi+" || particleName == "pi-") {
                                producedChargedPion = true;
                            }
                        }

                        // TRUE EXCLUSIVE CHARGE EXCHANGE:
                        // The primary pion died in the target via strong force, produced a pi0, and no extra charged pions.
                        if (producedPi0 && !producedChargedPion) {
                            fEventAction->SetTargetHasPi0();
                        }
                    }
                }
            }
        }
    }
}
