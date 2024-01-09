#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3Vector;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction* );
    virtual ~PrimaryGeneratorMessenger();

    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    PrimaryGeneratorAction* 	fAction;
    G4UIdirectory*              fGunDir;
    G4UIcmdWithAnInteger*	fSourceType;
    G4UIcmdWithAnInteger*	fSourceDirectionType;
    G4UIcmdWithAnInteger*	fSourceGeometry;
    G4UIcmdWithADoubleAndUnit*  fSourceEnergy;
    G4UIcmdWith3VectorAndUnit*  fSourcePosition;
    G4UIcmdWith3VectorAndUnit*  fSourceBoxXYZ;
    G4UIcmdWith3VectorAndUnit*  fSourceCentreCoords;
    G4UIcmdWith3Vector*		fSourceDirection;
    G4UIcmdWithAString* 	fSourceShape;
    G4UIcmdWithADoubleAndUnit* 	fSourceDiameter;
    G4UIcmdWithADoubleAndUnit* 	fSourceHalfZ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
