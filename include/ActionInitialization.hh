#ifndef ActionInitialization_h
#define ActionInitialization_h

#include "G4VUserActionInitialization.hh"

class ActionInitialization
    : public G4VUserActionInitialization {
  public:
    ActionInitialization();
    ~ActionInitialization();

    void Build() const override;
    void BuildForMaster() const override;
};

#endif

