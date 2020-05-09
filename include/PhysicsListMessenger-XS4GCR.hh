#ifndef PHYSICSLISTMESSENGER_HH
#define PHYSICSLISTMESSENGER_HH

#include "G4UImessenger.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class PhysicsListMessenger : public G4UImessenger {
   public:
    PhysicsListMessenger(PhysicsList*);
    virtual ~PhysicsListMessenger();
    void SetNewValue(G4UIcommand*, G4String);

   private:
    PhysicsList* list;
    G4UIdirectory* listDir;
    G4UIcmdWithAString* selectIonInelasticProcess;
};

#endif
