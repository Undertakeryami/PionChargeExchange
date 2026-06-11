#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh" // Fixed: Added missing hits map template header

EventAction::EventAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile("Pion_Data.root");

    analysisManager->CreateNtuple("SimulationData", "Pion Charge Exchange Data");
    analysisManager->CreateNtupleIColumn("DWC_ID", fDWC_ID);
    analysisManager->CreateNtupleDColumn("DWC_X", fDWC_X);
    analysisManager->CreateNtupleDColumn("DWC_Y", fDWC_Y);
    analysisManager->CreateNtupleDColumn("ECAL_TotalEdep"); 
    analysisManager->FinishNtuple();
}

EventAction::~EventAction() {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

void EventAction::BeginOfEventAction(const G4Event*) {
    fDWC_ID.clear();
    fDWC_X.clear();
    fDWC_Y.clear();
}

void EventAction::EndOfEventAction(const G4Event* event) {
    auto analysisManager = G4AnalysisManager::Instance();

    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    G4double totalEcalEnergy = 0.;

    if (hce) {
        G4int ecalID = G4SDManager::GetSDMpointer()->GetCollectionID("ECAL_SD/EDep");
        if (ecalID >= 0) { // Safety check to verify collection index
            auto hitsMap = static_cast<G4THitsMap<G4double>*>(hce->GetHC(ecalID));
            if (hitsMap) {
                for (auto itr : *hitsMap->GetMap()) {
                    totalEcalEnergy += *(itr.second);
                }
            }
        }
    }

    analysisManager->FillNtupleDColumn(3, totalEcalEnergy / MeV);
    analysisManager->AddNtupleRow();
}

void EventAction::LogDWCHit(G4int id, G4double x, G4double y) {
    fDWC_ID.push_back(id);
    fDWC_X.push_back(x / mm);
    fDWC_Y.push_back(y / mm);
}
