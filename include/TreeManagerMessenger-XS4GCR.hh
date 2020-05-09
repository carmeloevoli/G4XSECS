#ifndef TreeManagerMessenger_h
#define TreeManagerMessenger_h

#include "G4UImessenger.hh"

class TreeManager;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;

class TreeManagerMessenger : public G4UImessenger {
   public:
    TreeManagerMessenger(TreeManager*);
    virtual ~TreeManagerMessenger();
    void SetNewValue(G4UIcommand*, G4String);

   private:
    TreeManager* treeManager;
    G4UIdirectory* treeManagerDir;
    G4UIcmdWithAString* selectFileName;
};

#endif
