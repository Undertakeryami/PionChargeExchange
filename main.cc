#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {

    // interactive or batch mode?
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // run manager — brain of Geant4
    auto* runManager =
        G4RunManagerFactory::CreateRunManager();

    // register your 3 mandatory classes
    runManager->SetUserInitialization(
        new DetectorConstruction());
    runManager->SetUserInitialization(
        new PhysicsList());
    runManager->SetUserInitialization(
        new ActionInitialization());

    // initialize
    runManager->Initialize();

    // visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* UImanager =
        G4UImanager::GetUIpointer();

    if (ui) {
        // interactive mode
        UImanager->ApplyCommand("/vis/open OGL");
        UImanager->ApplyCommand("/vis/drawVolume");
        UImanager->ApplyCommand(
            "/vis/viewer/set/autoRefresh true");
        UImanager->ApplyCommand(
            "/vis/scene/add/trajectories smooth");
        UImanager->ApplyCommand("/run/beamOn 1");
        ui->SessionStart();
        delete ui;
    } else {
        // batch mode
        G4String command  = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    delete visManager;
    delete runManager;
    return 0;
}

