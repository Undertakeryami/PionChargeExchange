#include "DWCSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ios.hh"

DWCSensitiveDetector::DWCSensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name) {}

DWCSensitiveDetector::~DWCSensitiveDetector() {}

G4bool DWCSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    auto pre = step->GetPreStepPoint();
    auto pos = pre->GetPosition();

    G4cout << "DWC HIT: "
           << pos.x() << " "
           << pos.y() << " "
           << pos.z() << G4endl;

    return true;
}
