#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:

    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();

    virtual void SetNewValue(G4UIcommand*, G4String);

  private:

    DetectorConstruction*      fDetector;


    G4UIdirectory*             fULALAPDir;
    G4UIdirectory*             fDetDir;

    G4UIcmdWithAString*         commandSetWorldMaterial;
    G4UIcmdWithAnInteger*       commandSetDetectorType;
    G4UIcmdWithADoubleAndUnit*  commandSetLArDetectorLength;
    G4UIcmdWithADoubleAndUnit*  commandSetLArDetectorThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetLArDetectorWidth;
    G4UIcmdWithADoubleAndUnit*  commandSetshieldingThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetWaffleThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetcryostatThickness;
    G4UIcmdWithADoubleAndUnit*  commandSetShieldingWaffleNeutronAbsorberThickness;
    G4UIcmdWithAString*         commandSetTargetMaterial;
    G4UIcmdWithAString*         commandSetShieldingMaterial;
    G4UIcmdWithAString*         commandSetShieldingMaterialWaffle;
    G4UIcmdWithAString*         commandSetShieldingWaffleNeutronAbsorberMaterial;
    G4UIcmdWithAString*         commandSetDetectorName;
    G4UIcmdWithAString*         commandSetSetupName;
    G4UIcmdWithAString*         commandSetDataType;
    G4UIcmdWithAString*         commandSetOutputDirectory;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
