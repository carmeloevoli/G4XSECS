#include "EventAction-XS4GCR.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"

#ifndef __WITHOUT_ROOT__
#include "G4HadronicProcessStore.hh"
#include "TreeManager-XS4GCR.hh"
#endif

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* evt) {
    G4int evtNb = evt->GetEventID();

    if (evtNb % 100 == 0 && evtNb != 0) {
        G4cout << "---> Begin of Event: " << evtNb << G4endl;
    }

#ifndef __WITHOUT_ROOT__
    TreeManager::Instance()->EventNumber = evtNb;

    // G4double E = TreeManager::Instance()->PrimaryE;
    // G4string particleName =
    // std::string(TreeManager::Instance()->PrimaryName);

    //   //store cross section from G4HadronicProcessStore
    // G4ProcessTable* processTable  = G4ProcessTable::GetProcessTable();
    // G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
    // G4Material* material = fDetector->GetMaterial();
    // const G4Element* element = material->GetElement(0);
    // G4String procName = "total";
    // G4VProcess* process = processTable->FindProcess("total", particleName);
    // TreeManager::Instance()->xsec =
    // store->GetCrossSectionPerAtom(particleName,E,process,element,material);
#endif
}

void EventAction::EndOfEventAction(const G4Event* evt) {
    // #ifndef __WITHOUT_ROOT__
    //   TreeManager::Instance()->Fill();
    //   TreeManager::Instance()->Clear();
    // #endif
}
