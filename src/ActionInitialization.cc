/*#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction());
}*/

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"    // Links your data tree manager
#include "SteppingAction.hh" // Links your tracking scanner

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {}

void ActionInitialization::Build() const {
    // 1. Register the particle beam gun
    SetUserAction(new PrimaryGeneratorAction());

    // 2. Register the ROOT file manager
    EventAction* eventAction = new EventAction();
    SetUserAction(eventAction);

    // 3. Register the step interceptor (tracks DWC positions)
    SetUserAction(new SteppingAction(eventAction));
}
