// Copyright [2020] TBA
#include "PhysicsListMessenger-XS4GCR.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"
#include "PhysicsList.hh"
#ifndef __WITHOUT_ROOT__
#include "TreeManager-XS4GCR.hh"
#endif

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* alist) : list(alist) {
    listDir = new G4UIdirectory("/physics/");

    selectIonInelasticProcess =
        new G4UIcmdWithAString("/physics/setIonInelasticProcess", this);
    selectIonInelasticProcess->SetGuidance(
        "Select one of the model for Ion inelastic interaction");
    selectIonInelasticProcess->SetParameterName("setIonInelasticProcess",
                                                false);
    selectIonInelasticProcess->AvailableForStates(G4State_PreInit,
                                                  G4State_Idle);
    selectIonInelasticProcess->SetToBeBroadcasted(false);
}

PhysicsListMessenger::~PhysicsListMessenger() {
    delete selectIonInelasticProcess;
    delete listDir;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                       G4String newValue) {
    if (command == selectIonInelasticProcess) {
        G4cout << "PhysicsListMessenger: activating " << newValue << G4endl;
        list->AddPhysicsList(newValue);
#ifndef __WITHOUT_ROOT__
        TreeManager::Instance()->PhysicsList = newValue;
#endif
    } else {
        G4cerr << "***** Command is not found !!! " << newValue << G4endl;
    }
}
