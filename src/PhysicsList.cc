#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4DecayPhysics.hh"

PhysicsList::PhysicsList() {
    // electromagnetic physics
    // handles: ionization, Cherenkov, bremsstrahlung
    RegisterPhysics(new G4EmStandardPhysics());

    // hadronic physics
    // handles: pion/proton interactions with matter
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT());

    // decay physics
    RegisterPhysics(new G4DecayPhysics());
}

PhysicsList::~PhysicsList() {}
