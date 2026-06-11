#include "DetectorConstruction.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh" 

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

DetectorConstruction::DetectorConstruction()
  : fWorldLogical(nullptr)
{}

DetectorConstruction::~DetectorConstruction() {}

//====================================================================
// SENSITIVE DETECTORS & FIELD INTEGRATION
//====================================================================
void DetectorConstruction::ConstructSDandField() {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    // Make the Upgraded Lead Glass ECAL sensitive to Energy Deposit
    G4MultiFunctionalDetector* ecalSD = new G4MultiFunctionalDetector("ECAL_SD");
    G4VPrimitiveScorer* energyScorer = new G4PSEnergyDeposit("EDep");
    ecalSD->RegisterPrimitive(energyScorer);
    sdManager->AddNewDetector(ecalSD);
    
    // Attach to our newly updated Lead Glass crystal logical volume
    G4LogicalVolume* crystalLog = G4LogicalVolumeStore::GetInstance()->GetVolume("LeadGlass_Crystal_logical");
    if(crystalLog) {
        SetSensitiveDetector(crystalLog, ecalSD); 
    }
}

//====================================================================
// GEOMETRY CONSTRUCT
//====================================================================
G4VPhysicalVolume* DetectorConstruction::Construct(){

    G4NistManager* nist = G4NistManager::Instance();
    G4Material* vacuum = nist->FindOrBuildMaterial("G4_AIR");

    //===========================================
    // WORLD (Untouched)
    //===========================================
    G4Box* worldSolid = new G4Box("World_solid", 2.*m, 2.*m, 8.*m);

    fWorldLogical = new G4LogicalVolume(worldSolid, vacuum, "World_logical");
    fWorldLogical->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4VPhysicalVolume* worldPhysical =
        new G4PVPlacement(
            0, G4ThreeVector(),
            fWorldLogical, "World",
            0, false, 0, true);

    //===========================================
    // BUILD ALL DETECTORS
    //===========================================
    BuildCherenkovDetectors();
    BuildDWCs();
    BuildScintillators();
    BuildTarget();
    BuildECAL();

    return worldPhysical;
}

//===========================================
// CHERENKOV DETECTORS (Untouched)
//===========================================
void DetectorConstruction::BuildCherenkovDetectors(){
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* co2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    if (!co2) {
        G4Exception("BuildCherenkovDetectors", "BadMaterial", FatalException, "CO2 material not found");
    }

    G4Box* gasCherSolid = new G4Box("GasCher_solid", 15.*cm, 15.*cm, 15.*cm);
    G4LogicalVolume* gasCherLog = new G4LogicalVolume(gasCherSolid, co2, "GasCher_logical");
    gasCherLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.8, 0.4, 0.5)));

    new G4PVPlacement(0, G4ThreeVector(0, 0, 15.*cm), gasCherLog, "GasCherenkov", fWorldLogical, false, 0, true);

    G4Material* aerogel = new G4Material("Aerogel", 0.2*g/cm3, 3);
    aerogel->AddMaterial(nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE"), 0.625);
    aerogel->AddMaterial(nist->FindOrBuildMaterial("G4_WATER"), 0.374);
    aerogel->AddElement(nist->FindOrBuildElement("H"), 0.001);

    G4Box* aeroSolid = new G4Box("Aerogel_solid", 15.*cm, 15.*cm, 15.*cm);
    G4LogicalVolume* aeroLog = new G4LogicalVolume(aeroSolid, aerogel, "Aerogel_logical");
    aeroLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.8, 0.4, 0.7)));

    new G4PVPlacement(0, G4ThreeVector(0, 0, 55.*cm), aeroLog, "AerogelCherenkov", fWorldLogical, false, 0, true);

    G4cout << ">> Cherenkov detectors built" << G4endl;
}

//===========================================
// 4 DRIFT WIRE CHAMBERS (Untouched)
//===========================================
void DetectorConstruction::BuildDWCs() {
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* argon = nist->FindOrBuildMaterial("G4_Ar");
    G4Material* co2 = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

    G4Material* ArCO2 = new G4Material("ArCO2", 1.8*mg/cm3, 2);
    ArCO2->AddMaterial(argon, 70.*perCent);
    ArCO2->AddMaterial(co2, 30.*perCent);

    G4Box* dwcSolid = new G4Box("DWC_solid", 15.*cm, 15.*cm, 0.5*cm);

    std::vector<G4double> dwcZ = {80.5*cm, 111.5*cm, 195.5*cm, 221.5*cm};

    for (int i = 0; i < 4; i++) {
        G4String name = "DWC_" + std::to_string(i+1);
        G4LogicalVolume* dwcLog = new G4LogicalVolume(dwcSolid, ArCO2, name + "_logical");
        dwcLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.6, 0.2, 0.9, 0.6)));

        new G4PVPlacement(0, G4ThreeVector(0, 0, dwcZ[i]), dwcLog, name, fWorldLogical, false, i, true);
        fDWCLogicals.push_back(dwcLog);
    }
    G4cout << ">> 4 DWCs built" << G4endl;
}

//===========================================
// SCINTILLATORS (Untouched)
//===========================================
void DetectorConstruction::BuildScintillators() {
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* scint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Tubs* haloSolid = new G4Tubs("Halo_solid", 2.*cm, 10.*cm, 0.5*cm, 0.*deg, 360.*deg);
    G4LogicalVolume* haloLog = new G4LogicalVolume(haloSolid, scint, "Halo_logical");
    haloLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.2, 0.4, 0.9, 0.7)));
    new G4PVPlacement(0, G4ThreeVector(0,0,120.5*cm), haloLog, "HaloScintillator", fWorldLogical, false, 0, true);

    G4Box* s1Solid = new G4Box("S1_solid", 2.*cm, 2.*cm, 0.25*cm);
    G4LogicalVolume* s1Log = new G4LogicalVolume(s1Solid, scint, "S1_logical");
    s1Log->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.7, 0.1, 0.8)));
    new G4PVPlacement(0, G4ThreeVector(0,0,130.25*cm), s1Log, "S1", fWorldLogical, false, 0, true);

    G4Box* s2Solid = new G4Box("S2_solid", 2.*cm, 2.*cm, 0.25*cm);
    G4LogicalVolume* s2Log = new G4LogicalVolume(s2Solid, scint, "S2_logical");
    s2Log->SetVisAttributes(new G4VisAttributes(G4Colour(0.9, 0.5, 0.1, 0.8)));
    new G4PVPlacement(0, G4ThreeVector(0,0,180.25*cm), s2Log, "S2", fWorldLogical, false, 0, true);

    G4Tubs* vetoSolid = new G4Tubs("Veto_solid", 2.*cm, 10.*cm, 1.*cm, 0.*deg, 360.*deg);
    G4LogicalVolume* vetoLog = new G4LogicalVolume(vetoSolid, scint, "Veto_logical");
    vetoLog->SetVisAttributes(new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.8)));
    new G4PVPlacement(0, G4ThreeVector(0,0,186.*cm), vetoLog, "Veto", fWorldLogical, false, 0, true);

    G4cout << ">> Scintillators built" << G4endl;
}

//===========================================
// LITHIUM + GRAPHITE TARGET (Untouched)
//===========================================
void DetectorConstruction::BuildTarget() {
    G4NistManager* nist = G4NistManager::Instance();

    G4Material* lithium = nist->FindOrBuildMaterial("G4_Li");
    G4Box* liSolid = new G4Box("Li_solid", 2.*cm, 2.*cm, 5.*cm);
    G4LogicalVolume* liLog = new G4LogicalVolume(liSolid, lithium, "Li_logical");
    liLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.2, 0.7, 0.6, 0.9)));
    new G4PVPlacement(0, G4ThreeVector(0, 0, 165.*cm), liLog, "LithiumTarget", fWorldLogical, false, 0, true);

    G4Material* graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
    G4Box* grSolid = new G4Box("Gr_solid", 2.*cm, 2.*cm, 2.5*cm);
    G4LogicalVolume* grLog = new G4LogicalVolume(grSolid, graphite, "Gr_logical");
    grLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.2, 0.6, 0.5, 0.9)));
    new G4PVPlacement(0, G4ThreeVector(0, 0, 172.5*cm), grLog, "GraphiteTarget", fWorldLogical, false, 0, true);

    G4cout << ">> Targets built" << G4endl;
}

//====================================================================
// OFFICIAL CERN BL4S ECAL — 4x4 Lead Glass Matrix (OPAL Experiment Specs)
//====================================================================
void DetectorConstruction::BuildECAL() {
    G4NistManager* nist = G4NistManager::Instance();
    
    // 1. Define CERN Lead Glass Composition (SF5 Glass: 55% PbO, 45% SiO2)
    G4Material* SiO2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* PbO = nist->FindOrBuildMaterial("G4_LEAD_OXIDE");
    G4double density = 3.86 * g/cm3;
    G4Material* mat_LeadGlass = new G4Material("LeadGlass", density, 2);
    mat_LeadGlass->AddMaterial(PbO, 0.55);
    mat_LeadGlass->AddMaterial(SiO2, 0.45);

    // 2. Official BL4S Block Dimensions (10x10x37 cm³ per module)
    G4double cX = 10.0*cm; 
    G4double cY = 10.0*cm;
    G4double cZ = 37.0*cm; 

    // 3. Keep front face precisely at 422 cm mark from your layout template
    G4double ecalZFront = 422.0*cm;
    G4double ecalZCenter = ecalZFront + (cZ / 2.0); // 440.5 cm position

    G4Box* crystalSolid = new G4Box("LeadGlass_Crystal_solid", cX/2.0, cY/2.0, cZ/2.0);
    G4LogicalVolume* crystalLog = new G4LogicalVolume(crystalSolid, mat_LeadGlass, "LeadGlass_Crystal_logical");
    
    crystalLog->SetVisAttributes(new G4VisAttributes(G4Colour(0.8, 0.3, 0.3, 0.75)));

    // 4. Arrange into the standard 4x4 grid supplied for BL4S runs (16 blocks total)
    int n = 4; 
    for (int ix = 0; ix < n; ix++) {
        for (int iy = 0; iy < n; iy++) {
            // Centers the matrix around the beam axis line (0,0)
            G4double x = (ix - (n - 1)/2.0) * cX;
            G4double y = (iy - (n - 1)/2.0) * cY;

            new G4PVPlacement(0, 
                              G4ThreeVector(x, y, ecalZCenter), 
                              crystalLog, 
                              "ECAL_LeadGlass_crystal", 
                              fWorldLogical, 
                              false, 
                              ix*n + iy, 
                              true);
        }
    }
    G4cout << ">> Official CERN BL4S Lead Glass Calorimeter Built (" << n*n << " blocks)" << G4endl;
}
