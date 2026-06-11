#ifndef DWCSensitiveDetector_h
#define DWCSensitiveDetector_h

#include "G4VSensitiveDetector.hh"

class DWCSensitiveDetector : public G4VSensitiveDetector {
public:
    DWCSensitiveDetector(const G4String& name);
    virtual ~DWCSensitiveDetector();

    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
};

#endif
