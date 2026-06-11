#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include <vector>

class DetectorConstruction
    : public G4VUserDetectorConstruction {

  public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

  private:
    // helper functions — one per detector
    void BuildCherenkovDetectors();
    void BuildDWCs();
    void BuildScintillators();
    void BuildTarget();
    void BuildECAL();

    // world logical volume
    // all other volumes are daughters of this
    G4LogicalVolume* fWorldLogical;

    // store DWC logical volumes for SD attachment
    std::vector<G4LogicalVolume*> fDWCLogicals;
};

#endif
