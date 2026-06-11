#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {

    fParticleGun = new G4ParticleGun(1);

    G4ParticleTable* table =
        G4ParticleTable::GetParticleTable();

    // shoot pion+ like the T9 beam!
    // try also: "e-", "proton", "gamma", "pi-"
    fParticleGun->SetParticleDefinition(
        table->FindParticle("pi+"));

    // 5 GeV — typical test beam energy
    fParticleGun->SetParticleEnergy(5.0 * GeV);

    // along z axis = beam direction
    fParticleGun->SetParticleMomentumDirection(
        G4ThreeVector(0., 0., 1.));

    // start far left of beamline
    fParticleGun->SetParticlePosition(
        G4ThreeVector(0., 0., -450.*cm));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(
    G4Event* event) {
    fParticleGun->GeneratePrimaryVertex(event);
}
